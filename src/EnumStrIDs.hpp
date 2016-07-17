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


#if defined(CREATE_ENUM_ID_STRINGS) || !defined(__ADDON_ENUM_IDS__)

#ifndef CREATE_CLASS_ID_STRINGS
#define __ADDON_ENUM_IDS__
#endif

#include "template/Addon/Utils/EnumIDsToString.hpp"

// Gain Mode Socket IDs
BEGIN_ENUM_ID_CLASS(SocketGainModeIDs)
  ADD_ENUM_ID(UpdateMainGain)
  ADD_ENUM_ID(RequestMainGain)
END_ENUM_ID_CLASS(SocketGainModeIDs)


// Add-On AudioDSP Mode IDs
BEGIN_ENUM_ID_CLASS(ADSPModeIDs)
  ADD_ENUM_ID(PreProcessingModeCopy)
  ADD_ENUM_ID(PostProcessingModeCopy)
  ADD_ENUM_ID(InputResamplingModeCopy)
  ADD_ENUM_ID(PostProcessingModeGain)
END_ENUM_ID_CLASS(ADSPModeIDs)


// Add-On Mode Environment IDs
BEGIN_ENUM_ID_CLASS(ModeEnvironmentIDs)
  ADD_ENUM_ID(GainMode)
END_ENUM_ID_CLASS(ModeEnvironmentIDs)

#endif //defined(CREATE_ENUM_ID_STRINGS) || !defined(__ADDON_ENUM_IDS__)
