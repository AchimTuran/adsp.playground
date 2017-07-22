#pragma once
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



#include <string>
// reserved for future implementation
//#include <exception>
#include <kodi/addon-instance/AudioDSP.h>
#include "ADSPHelpers.h"

typedef unsigned long AE_DSP_CHANNEL_FLAGS;

class CADSPHelpers
{
public:
  static std::string            Translate_ChID_TO_String(AUDIODSP_ADDON_CHANNEL ChID);
  static float                  Convert_dB_TO_Value(float dB);
  static float                  Convert_Value_TO_dB(float Scale);

  /** @name Compare AudioDSP stream settings
   */
  /*!
   * @brief You can use this function to compare two stream settings.
   * @param Settings1 First stream settings.
   * @param Settings2 Second stream settings.
   * @return True if they're equal otherwise false.
   */
  static bool                   CmpStreamSettings(const AUDIODSP_ADDON_AUDIO_FORMAT& Lhs, const AUDIODSP_ADDON_AUDIO_FORMAT& Rhs);

  // reserved for future implementation
  //static std::string            GetAddonHomePath();
  //static std::string            GetAddonSettingsFile();
};

// reserved for future implementation
//class CADSPException : public CAddonException
//{
//
//};
