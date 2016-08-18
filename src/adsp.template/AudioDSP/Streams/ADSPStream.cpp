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



#include "ADSPStream.hpp"
#include "AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp"

#include "Addon/MessageSystem/Communication/MessageDispatcher.hpp"

#include "Addon/Process/AddonProcessManager.hpp"

#include "include/client.h"

using namespace ADDON;


CADSPStream::CADSPStream()
{
  m_MaxADSPModes = 0;
  m_ADSPModes    = NULL;

  m_CurrentMasterMode         = NULL;
  m_CurrentInputResampleMode  = NULL;
  m_CurrentOutputResampleMode = NULL;
}


CADSPStream::~CADSPStream()
{
}


AE_DSP_ERROR CADSPStream::Create(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties)
{
  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStream::Destroy()
{
  for (ADSPModeVector_t::iterator iter = m_ADSPModeVector.begin(); iter != m_ADSPModeVector.end(); ++iter)
  {
    IADSPMode *mode = *iter;
    if (mode)
    {
      CMessageDispatcher *messageDispatcher = dynamic_cast<CMessageDispatcher*>(*iter);
      if (messageDispatcher)
      { // when this mode can be casted to CMessageDispatcher
        // notify all created addon processes about this new dispatcher
        CAddonProcessManager::DisconnectDispatcher(messageDispatcher);
      }

      mode->ModeDestroy();
      CFactoryADSPModes::Destroy(mode);
      *iter = NULL;
    }
  }

  m_ADSPModeVector.clear();
  m_MaxADSPModes              = 0;
  m_ADSPModes                 = NULL;
  m_CurrentMasterMode         = NULL;
  m_CurrentInputResampleMode  = NULL;
  m_CurrentOutputResampleMode = NULL;

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStream::StreamIsModeSupported(AE_DSP_MODE_TYPE ModeType, unsigned int ModeID, int UniqueDBModeID)
{
  if (ModeID >= m_MaxADSPModes)
  {
    AE_DSP_ERROR_FAILED;
  }

  return m_ADSPModes[ModeID]->StreamIsModeSupported(ModeType, ModeID, UniqueDBModeID);
}


unsigned int CADSPStream::ProcessMode(unsigned int ModeID, float **ArrayIn, float **ArrayOut, unsigned int Samples)
{
  if (ModeID >= m_MaxADSPModes)
  {
    return 0;
  }

  return m_ADSPModes[ModeID]->ModeProcess(ArrayIn, ArrayOut, Samples);
}

unsigned int CADSPStream::ProcessMode(AE_DSP_MODE_TYPE ModeType, float **ArrayIn, float **ArrayOut, unsigned int Samples)
{
  IADSPMode *mode = GetSingleMode(ModeType);
  if (!mode)
  {
    return 0;
  }

  return mode->ModeProcess(ArrayIn, ArrayOut, Samples);
}

// TODO: implement common buffer for input process
unsigned int CADSPStream::ProcessMode(AE_DSP_MODE_TYPE ModeType, const float **Array_in, unsigned int Samples)
{
  return Samples;
}

unsigned int CADSPStream::NeededSamplesize(unsigned int ModeID)
{
  if (ModeID >= m_MaxADSPModes)
  {
    return 0;
  }

  return m_ADSPModes[ModeID]->NeededSamplesize();
}

unsigned int CADSPStream::NeededSamplesize(AE_DSP_MODE_TYPE ModeType)
{
  IADSPMode *mode = GetSingleMode(ModeType);
  if (!mode)
  {
    return 0;
  }

  return mode->NeededSamplesize();
}

float CADSPStream::GetDelay(unsigned int ModeID)
{
  if (ModeID >= m_MaxADSPModes)
  {
    return 0;
  }

  return m_ADSPModes[ModeID]->GetDelay();
}

float CADSPStream::GetDelay(AE_DSP_MODE_TYPE ModeType)
{
  IADSPMode *mode = GetSingleMode(ModeType);
  if (!mode)
  {
    return 0;
  }

  return mode->GetDelay();
}


int CADSPStream::MasterProcessGetOutChannels(unsigned long &OutChannelFlags)
{
  if (m_CurrentMasterMode)
  {
    return m_CurrentMasterMode->MasterProcessGetOutChannels(OutChannelFlags);
  }

  OutChannelFlags = AE_DSP_PRSNT_CH_UNDEFINED;
  return 0;
}

const char* CADSPStream::MasterProcessGetStreamInfoString()
{
  if (m_CurrentMasterMode)
  {
    return m_CurrentMasterMode->MasterProcessGetStreamInfoString();
  }

  return NULL;
}

AE_DSP_ERROR CADSPStream::MasterProcessSetMode(AE_DSP_STREAMTYPE StreamType, unsigned int ModeID, int UniqueDBModeID)
{
  if (ModeID >= m_MaxADSPModes)
  {
    return AE_DSP_ERROR_FAILED;
  }

  return AE_DSP_ERROR_NO_ERROR;
}


int CADSPStream::ResamplingRate(AE_DSP_MODE_TYPE ModeType)
{
  switch (ModeType)
  {
    case AE_DSP_MODE_TYPE_INPUT_RESAMPLE:
    {
      if (m_CurrentInputResampleMode)
      {
        return m_CurrentInputResampleMode->ResamplingRate();
      }
    }
    break;
    
    case AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE:
      if (m_CurrentInputResampleMode)
      {
        return m_CurrentOutputResampleMode->ResamplingRate();
      }
    break;
  }

  return 0;
}
