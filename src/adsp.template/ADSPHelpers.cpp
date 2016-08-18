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

AE_DSP_MENUHOOK_CAT CADSPHelpers::Translate_ADSPModeType_TO_MenuHook(AE_DSP_MODE_TYPE ModeType)
{
  AE_DSP_MENUHOOK_CAT menuHookCat = AE_DSP_MENUHOOK_UNKNOWN;
  switch (ModeType)
  {
    case AE_DSP_MODE_TYPE_INPUT_RESAMPLE:
      menuHookCat = AE_DSP_MENUHOOK_RESAMPLE;
    break;

    case AE_DSP_MODE_TYPE_PRE_PROCESS:
      menuHookCat = AE_DSP_MENUHOOK_PRE_PROCESS;
    break;

    case AE_DSP_MODE_TYPE_MASTER_PROCESS:
      menuHookCat = AE_DSP_MENUHOOK_MASTER_PROCESS;
    break;

    case AE_DSP_MODE_TYPE_POST_PROCESS:
      menuHookCat = AE_DSP_MENUHOOK_POST_PROCESS;
    break;

    case AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE:
      menuHookCat = AE_DSP_MENUHOOK_RESAMPLE;
    break;
  }

  return menuHookCat;
}

// Channel Layouts and channel names are from: https://trac.ffmpeg.org/wiki/AudioChannelManipulation
AE_DSP_CHANNEL CADSPHelpers::Translate_ChFlag_TO_ChID(AE_DSP_CHANNEL_PRESENT ChFlag)
{
  switch(ChFlag)
  {
    case AE_DSP_PRSNT_CH_FL:
      return AE_DSP_CH_FL;
    break;

    case AE_DSP_PRSNT_CH_FR:
      return AE_DSP_CH_FR;
    break;

    case AE_DSP_PRSNT_CH_FC:
      return AE_DSP_CH_FC;
    break;

    case AE_DSP_PRSNT_CH_LFE:
      return AE_DSP_CH_LFE;
    break;

    case AE_DSP_PRSNT_CH_BL:
      return AE_DSP_CH_BL;
    break;

    case AE_DSP_PRSNT_CH_BR:
      return AE_DSP_CH_BR;
    break;

    case AE_DSP_PRSNT_CH_FLOC:
      return AE_DSP_CH_FLOC;
    break;

    case AE_DSP_PRSNT_CH_FROC:
      return AE_DSP_CH_FROC;
    break;

    case AE_DSP_PRSNT_CH_BC:
      return AE_DSP_CH_BC;
    break;

    case AE_DSP_PRSNT_CH_SL:
      return AE_DSP_CH_SL;
    break;

    case AE_DSP_PRSNT_CH_SR:
      return AE_DSP_CH_SR;
    break;

    case AE_DSP_PRSNT_CH_TFL:
      return AE_DSP_CH_TFL;
    break;

    case AE_DSP_PRSNT_CH_TFR:
      return AE_DSP_CH_TFR;
    break;

    case AE_DSP_PRSNT_CH_TFC:
      return AE_DSP_CH_TFC;
    break;

    case AE_DSP_PRSNT_CH_TC:
      return AE_DSP_CH_TC;
    break;

    case AE_DSP_PRSNT_CH_TBL:
      return AE_DSP_CH_TBL;
    break;

    case AE_DSP_PRSNT_CH_TBR:
      return AE_DSP_CH_TBR;
    break;

    case AE_DSP_PRSNT_CH_TBC:
      return AE_DSP_CH_TBC;
    break;

    case AE_DSP_PRSNT_CH_BLOC:
      return AE_DSP_CH_BLOC;
    break;

    case AE_DSP_PRSNT_CH_BROC:
      return AE_DSP_CH_BROC;
    break;

    default:
      return AE_DSP_CH_INVALID;
    break;
  }
}

AE_DSP_CHANNEL_PRESENT CADSPHelpers::Translate_ChID_TO_ChFlag(AE_DSP_CHANNEL ChID)
{
  switch(ChID)
  {
    case AE_DSP_CH_FL:
      return AE_DSP_PRSNT_CH_FL;
    break;

    case AE_DSP_CH_FR:
      return AE_DSP_PRSNT_CH_FR;
    break;

    case AE_DSP_CH_FC:
      return AE_DSP_PRSNT_CH_FC;
    break;

    case AE_DSP_CH_LFE:
      return AE_DSP_PRSNT_CH_LFE;
    break;

    case AE_DSP_CH_BL:
      return AE_DSP_PRSNT_CH_BL;
    break;

    case AE_DSP_CH_BR:
      return AE_DSP_PRSNT_CH_BR;
    break;

    case AE_DSP_CH_FLOC:
      return AE_DSP_PRSNT_CH_FLOC;
    break;

    case AE_DSP_CH_FROC:
      return AE_DSP_PRSNT_CH_FROC;
    break;

    case AE_DSP_CH_BC:
      return AE_DSP_PRSNT_CH_BC;
    break;

    case AE_DSP_CH_SL:
      return AE_DSP_PRSNT_CH_SL;
    break;

    case AE_DSP_CH_SR:
      return AE_DSP_PRSNT_CH_SR;
    break;

    case AE_DSP_CH_TFL:
      return AE_DSP_PRSNT_CH_TFL;
    break;

    case AE_DSP_CH_TFR:
      return AE_DSP_PRSNT_CH_TFR;
    break;

    case AE_DSP_CH_TFC:
      return AE_DSP_PRSNT_CH_TFC;
    break;

    case AE_DSP_CH_TC:
      return AE_DSP_PRSNT_CH_TC;
    break;

    case AE_DSP_CH_TBL:
      return AE_DSP_PRSNT_CH_TBL;
    break;

    case AE_DSP_CH_TBR:
      return AE_DSP_PRSNT_CH_TBR;
    break;

    case AE_DSP_CH_TBC:
      return AE_DSP_PRSNT_CH_TBC;
    break;

    case AE_DSP_CH_BLOC:
      return AE_DSP_PRSNT_CH_BLOC;
    break;

    case AE_DSP_CH_BROC:
      return AE_DSP_PRSNT_CH_BROC;
    break;

    default:
      return AE_DSP_PRSNT_CH_UNDEFINED;
    break;
  }
}

string CADSPHelpers::Translate_ChFlag_TO_String(AE_DSP_CHANNEL_PRESENT ChFlag)
{
  string chStr = "";
  switch(ChFlag)
  {
    case AE_DSP_PRSNT_CH_FL:
      chStr = "front left";
    break;

    case AE_DSP_PRSNT_CH_FR:
      chStr = "front right";
    break;

    case AE_DSP_PRSNT_CH_FC:
      chStr = "front center";
    break;

    case AE_DSP_PRSNT_CH_LFE:
      chStr = "low frequency";
    break;

    case AE_DSP_PRSNT_CH_BL:
      chStr = "back left";
    break;

    case AE_DSP_PRSNT_CH_BR:
      chStr = "back right";
    break;

    case AE_DSP_PRSNT_CH_FLOC:
      chStr = "front left-of-center";
    break;

    case AE_DSP_PRSNT_CH_FROC:
      chStr = "front right-of-center";
    break;

    case AE_DSP_PRSNT_CH_BC:
      chStr = "back center";
    break;

    case AE_DSP_PRSNT_CH_SL:
      chStr = "side left";
    break;

    case AE_DSP_PRSNT_CH_SR:
      chStr = "side right";
    break;

    case AE_DSP_PRSNT_CH_TFL:
      chStr = "top front left";
    break;

    case AE_DSP_PRSNT_CH_TFR:
      chStr = "top front right";
    break;

    case AE_DSP_PRSNT_CH_TFC:
      chStr = "top front center";
    break;

    case AE_DSP_PRSNT_CH_TC:
      chStr = "top center";
    break;

    case AE_DSP_PRSNT_CH_TBL:
      chStr = "top back left";
    break;

    case AE_DSP_PRSNT_CH_TBR:
      chStr = "top back right";
    break;

    case AE_DSP_PRSNT_CH_TBC:
      chStr = "top back center";
    break;

    case AE_DSP_PRSNT_CH_BLOC:
      chStr = "back left of center";
    break;

    case AE_DSP_PRSNT_CH_BROC:
      chStr = "back right of center";
    break;

    default:
      chStr = "undefined";
    break;
  }

  return chStr;
}

string CADSPHelpers::Translate_ChID_TO_String(AE_DSP_CHANNEL ChID)
{
  string chStr = "";
  switch(ChID)
  {
    case AE_DSP_CH_FL:
      chStr = "front left";
    break;

    case AE_DSP_CH_FR:
      chStr = "front right";
    break;

    case AE_DSP_CH_FC:
      chStr = "front center";
    break;

    case AE_DSP_CH_LFE:
      chStr = "low frequency";
    break;

    case AE_DSP_CH_BL:
      chStr = "back left";
    break;

    case AE_DSP_CH_BR:
      chStr = "back right";
    break;

    case AE_DSP_CH_FLOC:
      chStr = "front left-of-center";
    break;

    case AE_DSP_CH_FROC:
      chStr = "front right-of-center";
    break;

    case AE_DSP_CH_BC:
      chStr = "back center";
    break;

    case AE_DSP_CH_SL:
      chStr = "side left";
    break;

    case AE_DSP_CH_SR:
      chStr = "side right";
    break;

    case AE_DSP_CH_TFL:
      chStr = "top front left";
    break;

    case AE_DSP_CH_TFR:
      chStr = "top front right";
    break;

    case AE_DSP_CH_TFC:
      chStr = "top front center";
    break;

    case AE_DSP_CH_TC:
      chStr = "top center";
    break;

    case AE_DSP_CH_TBL:
      chStr = "top back left";
    break;

    case AE_DSP_CH_TBR:
      chStr = "top back right";
    break;

    case AE_DSP_CH_TBC:
      chStr = "top back center";
    break;

    case AE_DSP_CH_BLOC:
      chStr = "back left of center";
    break;

    case AE_DSP_CH_BROC:
      chStr = "back right of center";
    break;

    default:
      chStr = "undefined";
    break;
  }

  return chStr;
}

bool CADSPHelpers::IsChannelFlag_Present(AE_DSP_CHANNEL_FLAGS AvailableFlags, AE_DSP_CHANNEL_PRESENT ChFlag)
{
  return (bool)(AvailableFlags & (ChFlag));
}

bool CADSPHelpers::IsChannelID_Present(AE_DSP_CHANNEL_FLAGS AvailableFlags, AE_DSP_CHANNEL ChID)
{
  return (bool)(AvailableFlags & (1<<ChID));
}

AE_DSP_CHANNEL CADSPHelpers::GetNextChID(AE_DSP_CHANNEL_FLAGS AvailableFlags, AE_DSP_CHANNEL ChID)
{
  int retCh = AE_DSP_CH_INVALID;
  for(int ii = ChID; ii < AE_DSP_CH_MAX; ii++)
  {
    if(AvailableFlags & 1<<ii)
    {
      retCh = ii;
      break;
    }
  }

  return (AE_DSP_CHANNEL)retCh;
}

AE_DSP_CHANNEL_PRESENT CADSPHelpers::GetNextChFlag(AE_DSP_CHANNEL_FLAGS AvailableFlags, AE_DSP_CHANNEL_PRESENT ChFlag)
{
  return Translate_ChID_TO_ChFlag(GetNextChID(AvailableFlags, Translate_ChFlag_TO_ChID(ChFlag)));
}

float CADSPHelpers::Convert_dB_TO_Value(float dB)
{
  return powf(10.0f, dB/20.0f);
}

float CADSPHelpers::Convert_Value_TO_dB(float Scale)
{
  return 20.0f*log10f(abs(Scale));
}

bool CADSPHelpers::CmpStreamSettings(const AE_DSP_SETTINGS& Settings1, const AE_DSP_SETTINGS& Settings2)
{
  return  Settings1.iStreamID               == Settings2.iStreamID                &&
          Settings1.iStreamType             == Settings2.iStreamType              &&
          Settings1.iInChannels             == Settings2.iInChannels              &&
          Settings1.lInChannelPresentFlags  == Settings2.lInChannelPresentFlags   &&
          Settings1.iInFrames               == Settings2.iInFrames                &&
          Settings1.iInSamplerate           == Settings2.iInSamplerate            &&
          Settings1.iProcessFrames          == Settings2.iProcessFrames           &&
          Settings1.iProcessSamplerate      == Settings2.iProcessSamplerate       &&
          Settings1.iOutChannels            == Settings2.iOutChannels             &&
          Settings1.lOutChannelPresentFlags == Settings2.lOutChannelPresentFlags  &&
          Settings1.iOutFrames              == Settings2.iOutFrames               &&
          Settings1.iOutSamplerate          == Settings2.iOutSamplerate           &&
          Settings1.bInputResamplingActive  == Settings2.bInputResamplingActive   &&
          Settings1.bStereoUpmix            == Settings2.bStereoUpmix             &&
          Settings1.iQualityLevel           == Settings2.iQualityLevel;
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
