/*
 *      Copyright (C) 2005-2014 Team XBMC
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


#include "ADSPHelpers.h"
#include <math.h>
using namespace std;

#if !defined(TARGET_WINDOWS)
  inline float abs(float Val)
  {
    if(Val >= 0.0f)
    {
      return Val;
    }
    else
    {
      return -1.0f*Val;
    }
  }
#endif

string CADSPHelpers::Translate_ChID_TO_String(AUDIODSP_ADDON_CHANNEL ChID)
{
  string chStr = "";
  switch(ChID)
  {
    case AUDIODSP_ADDON_CH_FL:
      chStr = "front left";
    break;

    case AUDIODSP_ADDON_CH_FR:
      chStr = "front right";
    break;

    case AUDIODSP_ADDON_CH_FC:
      chStr = "front center";
    break;

    case AUDIODSP_ADDON_CH_LFE:
      chStr = "low frequency";
    break;

    case AUDIODSP_ADDON_CH_BL:
      chStr = "back left";
    break;

    case AUDIODSP_ADDON_CH_BR:
      chStr = "back right";
    break;

    case AUDIODSP_ADDON_CH_FLOC:
      chStr = "front left-of-center";
    break;

    case AUDIODSP_ADDON_CH_FROC:
      chStr = "front right-of-center";
    break;

    case AUDIODSP_ADDON_CH_BC:
      chStr = "back center";
    break;

    case AUDIODSP_ADDON_CH_SL:
      chStr = "side left";
    break;

    case AUDIODSP_ADDON_CH_SR:
      chStr = "side right";
    break;

    case AUDIODSP_ADDON_CH_TFL:
      chStr = "top front left";
    break;

    case AUDIODSP_ADDON_CH_TFR:
      chStr = "top front right";
    break;

    case AUDIODSP_ADDON_CH_TFC:
      chStr = "top front center";
    break;

    case AUDIODSP_ADDON_CH_TC:
      chStr = "top center";
    break;

    case AUDIODSP_ADDON_CH_TBL:
      chStr = "top back left";
    break;

    case AUDIODSP_ADDON_CH_TBR:
      chStr = "top back right";
    break;

    case AUDIODSP_ADDON_CH_TBC:
      chStr = "top back center";
    break;

    case AUDIODSP_ADDON_CH_BLOC:
      chStr = "back left of center";
    break;

    case AUDIODSP_ADDON_CH_BROC:
      chStr = "back right of center";
    break;

    default:
      chStr = "undefined";
    break;
  }

  return chStr;
}


float CADSPHelpers::Convert_dB_TO_Value(float dB)
{
  return powf(10.0f, dB/20.0f);
}

float CADSPHelpers::Convert_Value_TO_dB(float Scale)
{
  return 20.0f*log10f(abs(Scale));
}

bool CADSPHelpers::CmpStreamSettings(const AUDIODSP_ADDON_AUDIO_FORMAT& Lhs, const AUDIODSP_ADDON_AUDIO_FORMAT& Rhs)
{
  return true;
  //return  Settings1.iStreamID               == Settings2.iStreamID                &&
  //        Settings1.iStreamType             == Settings2.iStreamType              &&
  //        Settings1.iInChannels             == Settings2.iInChannels              &&
  //        Settings1.lInChannelPresentFlags  == Settings2.lInChannelPresentFlags   &&
  //        Settings1.iInFrames               == Settings2.iInFrames                &&
  //        Settings1.iInSamplerate           == Settings2.iInSamplerate            &&
  //        Settings1.iProcessFrames          == Settings2.iProcessFrames           &&
  //        Settings1.iProcessSamplerate      == Settings2.iProcessSamplerate       &&
  //        Settings1.iOutChannels            == Settings2.iOutChannels             &&
  //        Settings1.lOutChannelPresentFlags == Settings2.lOutChannelPresentFlags  &&
  //        Settings1.iOutFrames              == Settings2.iOutFrames               &&
  //        Settings1.iOutSamplerate          == Settings2.iOutSamplerate           &&
  //        Settings1.bInputResamplingActive  == Settings2.bInputResamplingActive   &&
  //        Settings1.bStereoUpmix            == Settings2.bStereoUpmix             &&
  //        Settings1.iQualityLevel           == Settings2.iQualityLevel;
}

// reserved for future implementation
//string CADSPHelpers::GetAddonHomePath()
//{
//
//}
//
//string CADSPHelpers::GetAddonSettingsFile()
//{
//
//}
