/*
 *      Copyright (C) 2005-2016 Team Kodi
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */



#include "CompressorMode/CompressorMode.hpp"

#include "adsp.template/ADSPHelpers.h"
#include "adsp.template/include/client.h"
#include "adsp.template/Addon/Process/AddonProcessManager.hpp"

#include "asplib/Dynamics/asplib_DynamicsFactory.hpp"

#include <math.h>

using namespace ADDON;
using namespace asplib;


const std::string CCompressorModeName::ModeName = CADSPModeIDs::ToString(CADSPModeIDs::PostProcessingModeCompressor);


CCompressorMode::CCompressorMode() :
  IView(CDispatcherIDs::ToString(CDispatcherIDs::CompressorMode),
        CDispatcherIDs::CompressorMode,
        CADSPModeIDs::PostProcessingModeCompressor)
{
  m_MainCompressor = 1.0f;
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_Compressors[ii] = nullptr;
  }
}


CCompressorMode::~CCompressorMode()
{
}


AE_DSP_ERROR CCompressorMode::ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties)
{
  if (Settings.iOutFrames <= 0 || Settings.iOutSamplerate <= 0)
  {
    return AE_DSP_ERROR_FAILED;
  }

  m_InChannels            = Settings.iInChannels;
  m_InChannelPresentFlags = Settings.lInChannelPresentFlags;
  m_FrameSize             = (uint32_t)Settings.iOutFrames;
  m_SampleFrequency       = (uint32_t)Settings.iOutSamplerate;

  // reset used channel mapping array
  for (int ch = 0; ch < AE_DSP_CH_MAX; ch++)
  {
    m_ChannelMappingIdx[ch] = -1;
    m_ChannelMappingFlags[ch] = 0;
  }

  // add used channels to channel mapping array
  int lastAudioChannel = 0;
  for (int ch = 0; ch < m_InChannels; ch++)
  {
    unsigned long tempChannelFlag = 1 << lastAudioChannel;
    m_ChannelMappingIdx[ch] = CADSPHelpers::GetNextChID(m_InChannelPresentFlags,
                                                        CADSPHelpers::Translate_ChFlag_TO_ChID((AE_DSP_CHANNEL_PRESENT)tempChannelFlag));
    m_ChannelMappingFlags[ch] = CADSPHelpers::Translate_ChID_TO_ChFlag((AE_DSP_CHANNEL)m_ChannelMappingIdx[ch]);
    if (m_ChannelMappingIdx[ch] == AE_DSP_CH_INVALID)
    {
      KODI->Log(LOG_ERROR, "%s, %i, Tried to create invalid channel mappings!", __FUNCTION__, __LINE__);
      return AE_DSP_ERROR_FAILED;
    }
    lastAudioChannel = m_ChannelMappingIdx[ch] + 1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    ASPLIB_ERR asplibErr = ASPLIB_FACTORY_DYNAMICS.Create(ASPLIB_DYNAMICS_Compressor, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, m_Compressors[ii]);
    if (asplibErr != ASPLIB_ERR_NO_ERROR)
    {
      // todo log error!
      return AE_DSP_ERROR_FAILED;
    }
    
    asplibErr = m_Compressors[ii]->Create(m_FrameSize, m_SampleFrequency, static_cast<void*>(&m_CompressorOptions[ii]));
    if (asplibErr != ASPLIB_ERR_NO_ERROR)
    {
      // todo log error!
      return AE_DSP_ERROR_FAILED;
    }
  }

  ConfigureCompressors();

  if (!CCompressorModeMessages::Create(this))
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to create message dispachter %s", __FUNCTION__, __LINE__, Name.c_str());
    return AE_DSP_ERROR_FAILED;
  }

  if (CAddonProcessManager::ConnectObject(this) != 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to connect message dispachter %s", __FUNCTION__, __LINE__, this->Name.c_str());
    return AE_DSP_ERROR_FAILED;
  }

  if (!this->SendMsg(nullptr, 0, CSocketCompressorModeIDs::RequestModelState))
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed send \"%s\" from dispatcher %s to compressor controller", __FUNCTION__, __LINE__, CSocketCompressorModeIDs::ToString(CSocketCompressorModeIDs::RequestModelState), this->Name.c_str());
    return AE_DSP_ERROR_FAILED;
  }

  return AE_DSP_ERROR_NO_ERROR;
}


void CCompressorMode::ModeDestroy()
{
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    // todo log asplib error
    if (m_Compressors[ii])
    {
      ASPLIB_ERR asplibErr = m_Compressors[ii]->Destroy();
      asplibErr = ASPLIB_FACTORY_DYNAMICS.Destroy(m_Compressors[ii]);
    }
  }
}


// Requiered Processing Methods
unsigned int CCompressorMode::ModeProcess(float **ArrayIn, float **ArrayOut, unsigned int Samples)
{
  this->ProcessMessages();
  // todo error checking in debug mode (Samples and m_Compressors[ii]

  for (int ch = 0; ch < m_InChannels; ch++)
  { 
    float *out  = ArrayOut[m_ChannelMappingIdx[ch]];
    float *in   = ArrayIn[m_ChannelMappingIdx[ch]];
    m_Compressors[m_ChannelMappingIdx[ch]]->Process(in, out);
  }

  return Samples;
}


// message method callbacks
int CCompressorMode::SetTauRelease(Message &Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_CompressorOptions[ii].tauRelease = (long double)(*((float*)Msg.data));
  }

  return ConfigureCompressors();
}

int CCompressorMode::SetTauAttack(Message &Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_CompressorOptions[ii].tauAttack = (long double)(*((float*)Msg.data));
  }

  return ConfigureCompressors();
}

int CCompressorMode::SetThreshold(Message &Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_CompressorOptions[ii].threshold = (long double)(*((float*)Msg.data));
  }

  return ConfigureCompressors();
}

int CCompressorMode::SetCompressionRation(Message &Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_CompressorOptions[ii].compressionRatio = (long double)(*((float*)Msg.data));
  }

  return ConfigureCompressors();
}

int CCompressorMode::SetKneeWidth(Message &Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_CompressorOptions[ii].kneeWidth = (long double)(*((float*)Msg.data));
  }

  return ConfigureCompressors();
}

int CCompressorMode::SetGainCurve(Message &Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_CompressorOptions[ii].gainCurve = (CompressorOptions::eGainCurve_t)(*((int*)Msg.data));
  }

  return ConfigureCompressors();
}

int CCompressorMode::ConfigureCompressors()
{
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    if (m_Compressors[ii])
    {
      ASPLIB_ERR asplibErr = m_Compressors[ii]->Create(m_FrameSize, m_SampleFrequency, static_cast<void*>(&m_CompressorOptions[ii]));
    }
  }
  return 0;
}
