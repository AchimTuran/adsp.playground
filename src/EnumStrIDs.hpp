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

#include "adsp.template/Addon/Utils/EnumIDsToString.hpp"

// Gain Mode Socket IDs
BEGIN_ENUM_ID_CLASS(SocketGainModeIDs)
  ADD_ENUM_ID(UpdateGain_FL)
  ADD_ENUM_ID(UpdateGain_FR)
  ADD_ENUM_ID(UpdateGain_FC)
  ADD_ENUM_ID(UpdateGain_LFE)
  ADD_ENUM_ID(UpdateGain_BL)
  ADD_ENUM_ID(UpdateGain_BR)
  ADD_ENUM_ID(UpdateGain_FLOC)
  ADD_ENUM_ID(UpdateGain_FROC)
  ADD_ENUM_ID(UpdateGain_BC)
  ADD_ENUM_ID(UpdateGain_SL)
  ADD_ENUM_ID(UpdateGain_SR)
  ADD_ENUM_ID(UpdateGain_TFL)
  ADD_ENUM_ID(UpdateGain_TFR)
  ADD_ENUM_ID(UpdateGain_TFC)
  ADD_ENUM_ID(UpdateGain_TC)
  ADD_ENUM_ID(UpdateGain_TBL)
  ADD_ENUM_ID(UpdateGain_TBR)
  ADD_ENUM_ID(UpdateGain_TBC)
  ADD_ENUM_ID(UpdateGain_BLOC)
  ADD_ENUM_ID(UpdateGain_BROC)
  ADD_ENUM_ID(RequestGain_FL)
  ADD_ENUM_ID(RequestGain_FR)
  ADD_ENUM_ID(RequestGain_FC)
  ADD_ENUM_ID(RequestGain_LFE)
  ADD_ENUM_ID(RequestGain_BL)
  ADD_ENUM_ID(RequestGain_BR)
  ADD_ENUM_ID(RequestGain_FLOC)
  ADD_ENUM_ID(RequestGain_FROC)
  ADD_ENUM_ID(RequestGain_BC)
  ADD_ENUM_ID(RequestGain_SL)
  ADD_ENUM_ID(RequestGain_SR)
  ADD_ENUM_ID(RequestGain_TFL)
  ADD_ENUM_ID(RequestGain_TFR)
  ADD_ENUM_ID(RequestGain_TFC)
  ADD_ENUM_ID(RequestGain_TC)
  ADD_ENUM_ID(RequestGain_TBL)
  ADD_ENUM_ID(RequestGain_TBR)
  ADD_ENUM_ID(RequestGain_TBC)
  ADD_ENUM_ID(RequestGain_BLOC)
  ADD_ENUM_ID(RequestGain_BROC)
  ADD_ENUM_ID(RequestModelState)
END_ENUM_ID_CLASS(SocketGainModeIDs)

// Compressor Mode Socket IDs
BEGIN_ENUM_ID_CLASS(SocketCompressorModeIDs)
  ADD_ENUM_ID(UpdateTauRelease)
  ADD_ENUM_ID(RequestTauRelease)
  ADD_ENUM_ID(UpdateTauAttack)
  ADD_ENUM_ID(RequestTauAttack)
  ADD_ENUM_ID(UpdateThreshold)
  ADD_ENUM_ID(RequestThreshold)
  ADD_ENUM_ID(UpdateCompressionRatio)
  ADD_ENUM_ID(RequestCompressionRatio)
  ADD_ENUM_ID(UpdateKneeWidth)
  ADD_ENUM_ID(RequestKneeWidth)
  ADD_ENUM_ID(UpdateGainCurve)
  ADD_ENUM_ID(RequestGainCurve)
  ADD_ENUM_ID(RequestModelState)
END_ENUM_ID_CLASS(SocketCompressorModeIDs)

// Add-On AudioDSP Mode IDs
BEGIN_ENUM_ID_CLASS(ADSPModeIDs)
  ADD_ENUM_ID(PreProcessingModeCopy)
  ADD_ENUM_ID(PostProcessingModeCopy)
  ADD_ENUM_ID(InputResamplingModeCopy)
  ADD_ENUM_ID(PostProcessingModeGain)
  ADD_ENUM_ID(PostProcessingModeCompressor)
END_ENUM_ID_CLASS(ADSPModeIDs)


// Add-On AudioDSP Mode IDs
BEGIN_ENUM_ID_CLASS(DispatcherIDs)
  ADD_ENUM_ID(GainModeController)
  ADD_ENUM_ID(GainModeModel)
  ADD_ENUM_ID(GainModeDialog)
  ADD_ENUM_ID(GainMode)
  ADD_ENUM_ID(CompressorMode)
  ADD_ENUM_ID(CompressorModeController)
  ADD_ENUM_ID(CompressorModeModel)
  ADD_ENUM_ID(CompressorModeDialog)
END_ENUM_ID_CLASS(DispatcherIDs)

#endif //defined(CREATE_ENUM_ID_STRINGS) || !defined(__ADDON_ENUM_IDS__)
