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



#include "GainMode/GainMode.hpp"

#include "adsp.template/ADSPHelpers.h"
#include "adsp.template/include/client.h"
#include "adsp.template/Addon/Process/AddonProcessManager.hpp"
#include "EnumStrIDs.hpp"

#include <math.h>

using namespace ADDON;


// Helper function prototypes
static float dB_to_Gain(float dB);


const std::string CGainModeName::ModeName = CADSPModeIDs::ToString(CADSPModeIDs::PostProcessingModeGain);


CGainMode::CGainMode() :
  IView(CDispatcherIDs::ToString(CDispatcherIDs::GainMode),
        CDispatcherIDs::GainMode,
        CADSPModeIDs::PostProcessingModeGain)
{
}


CGainMode::~CGainMode()
{
}


AE_DSP_ERROR CGainMode::ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties)
{
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_Gain[ii] = 1.0f;
  }

  m_InChannels            = Settings.iInChannels;
  m_InChannelPresentFlags = Settings.lInChannelPresentFlags;

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

  if (!CGainModeMessages::Create(this))
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to create message dispachter %s", __FUNCTION__, __LINE__, Name.c_str());
    return AE_DSP_ERROR_FAILED;
  }

  if (CAddonProcessManager::ConnectObject(this) != 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to connect message dispachter %s", __FUNCTION__, __LINE__, this->Name.c_str());
    return AE_DSP_ERROR_FAILED;
  }

  if (!this->SendMsg(nullptr, 0, CSocketGainModeIDs::RequestModelState))
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed send \"%s\" from dispatcher %s to compressor controller", __FUNCTION__, __LINE__, CSocketGainModeIDs::ToString(CSocketGainModeIDs::RequestModelState), this->Name.c_str());
    return AE_DSP_ERROR_FAILED;
  }

  return AE_DSP_ERROR_NO_ERROR;
}


void CGainMode::ModeDestroy()
{
}


// Requiered Processing Methods
unsigned int CGainMode::ModeProcess(float **ArrayIn, float **ArrayOut, unsigned int Samples)
{
  this->ProcessMessages();

  for (int ch = 0; ch < m_InChannels; ch++)
  { 
    float *out = ArrayOut[m_ChannelMappingIdx[ch]];
    const float *in  = ArrayIn[m_ChannelMappingIdx[ch]];
    for (unsigned int ii = 0; ii < Samples; ii++)
    {
      out[ii] = m_Gain[ch]*in[ii];
    }
  }

  return Samples;
}


// message method callbacks
int CGainMode::SetGain(Message &Msg)
{
  if (Msg.size != sizeof(float) || Msg.signal < CSocketGainModeIDs::UpdateGain_FL || Msg.signal > CSocketGainModeIDs::UpdateGain_BROC)
  {
    // TODO: error code
    return -1;
  }

  int index = Msg.signal - CSocketGainModeIDs::UpdateGain_FL;
  m_Gain[index] = dB_to_Gain(*((float*)(Msg.data)));

  return 0;
}


// Helper functions
static float dB_to_Gain(float dB)
{
  return powf(10, dB / 20.0f);
}
