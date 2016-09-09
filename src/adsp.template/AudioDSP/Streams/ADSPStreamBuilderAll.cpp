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



#include "AudioDSP/Streams/ADSPStreamBuilderAll.hpp"
#include "AudioDSP/Streams/ADSPStream.hpp"

#include "AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp"

#include "Addon/MessageSystem/Communication/MessageDispatcher.hpp"

#include "Addon/Process//AddonProcessManager.hpp"

#include "include/client.h"

using namespace ADDON;


CADSPStreamBuilderAll::CADSPStreamBuilderAll()
{
}

CADSPStreamBuilderAll::~CADSPStreamBuilderAll()
{
}


AE_DSP_ERROR CADSPStreamBuilderAll::ConstructStream(CADSPStream &ADSPStream, const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties)
{
  CADSPStream::ADSPModeVector_t& modeVector = CADSPStreamAccessor::m_ADSPModeVector(ADSPStream);

  CFactoryADSPModes::ADSPModeInfoVector_t modeInfos;
  CFactoryADSPModes::GetAvailableModes(modeInfos);

  for (CFactoryADSPModes::ADSPModeInfoVector_t::iterator iter = modeInfos.begin(); iter != modeInfos.end(); ++iter)
  {
    IADSPMode* mode = NULL;
    AE_DSP_ERROR err = CFactoryADSPModes::Create(iter->ModeInfo.ModeID, iter->ModeInfo.ModeType, mode);

    if (err == AE_DSP_ERROR_NO_ERROR)
    {
      err = mode->Create(Settings, pProperties);
      if (err == AE_DSP_ERROR_NO_ERROR)
      {
        modeVector.push_back(mode);
        // TODO implement functionality to send messages before dispatcher gets connected to other dispatchers
        //MVCObject *object = dynamic_cast<MVCObject*>(mode);
        //if (object)
        //{ // when this mode can be casted to CMessageDispatcher
        //  // notify all created addon processes about this new dispatcher
        //  CAddonProcessManager::ConnectObject(object); // TODO: check return value
        //}
      }
      else
      {
        KODI->Log(LOG_ERROR, "%s, %i, Failed to create AudioDSP mode \"%s\"", __FUNCTION__, __LINE__, iter->ModeName.c_str());
      }
    }
    else
    {
      KODI->Log(LOG_ERROR, "%s, %i, Failed to get AudioDSP mode \"%s\"", __FUNCTION__, __LINE__, iter->ModeName.c_str());
    }
  }

  if (modeVector.size() <= 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to create entire Stream!", __FUNCTION__, __LINE__);
    return AE_DSP_ERROR_FAILED;
  }

  CADSPStreamAccessor::m_MaxADSPModes(ADSPStream) = modeVector.size();
  CADSPStreamAccessor::m_ADSPModes(ADSPStream)    = modeVector.data();

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStreamBuilderAll::InitializeStream(CADSPStream &ADSPStream, const AE_DSP_SETTINGS *Settings)
{
  CADSPStream::ADSPModeVector_t& modeVector = CADSPStreamAccessor::m_ADSPModeVector(ADSPStream);
  if (modeVector.size() <= 0)
  {
    return AE_DSP_ERROR_FAILED;
  }

  for (CADSPStream::ADSPModeVector_t::iterator iter = modeVector.begin(); iter != modeVector.end(); ++iter)
  {
    AE_DSP_ERROR err = (*iter)->Initialize(Settings);
    if (err != AE_DSP_ERROR_NO_ERROR)
    {
      return err;
    }
  }

  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStreamBuilderAll::AddMode(CADSPStream &ADSPStream, IADSPMode *ADSPMode)
{
  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CADSPStreamBuilderAll::AddMode(CADSPStream &ADSPStream, unsigned int ModeID)
{
  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CADSPStreamBuilderAll::RemoveMode(CADSPStream &ADSPStream, IADSPMode *ADSPMode)
{
  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CADSPStreamBuilderAll::RemoveMode(CADSPStream &ADSPStream, unsigned int ModeID)
{
  return AE_DSP_ERROR_NO_ERROR;
}
