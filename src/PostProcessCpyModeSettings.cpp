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



#include "PostProcessCpyModeSettings.hpp"

#include "EnumStrIDs.hpp"

#include <string.h>


CPostProcessCpyModeSettings::CPostProcessCpyModeSettings()
{
  strcpy(this->strModeName, CADSPModeIDs::ToString(CADSPModeIDs::PostProcessingModeCopy));

  //this->iModeSupportTypeFlags = AE_DSP_PRSNT_ASTREAM_BASIC | AE_DSP_PRSNT_ASTREAM_MUSIC | AE_DSP_PRSNT_ASTREAM_MOVIE | AE_DSP_PRSNT_ASTREAM_GAME | AE_DSP_PRSNT_ASTREAM_APP | AE_DSP_PRSNT_ASTREAM_MESSAGE | AE_DSP_PRSNT_ASTREAM_PHONE;
  this->bHasSettingsDialog    = false;
  this->bIsDisabled           = true;

  this->iModeName         = 30012;
  this->iModeSetupName    = 30013;
  this->iModeDescription  = 30014;
  this->iModeHelp         = 30015;

  strcpy(this->strOwnModeImage,       "img1.jpg");
  strcpy(this->strOverrideModeImage,  "img1.jpg");
}
