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



#include "CompressorMode/CompressorModeControllerMessages.hpp"
#include "CompressorMode/CompressorModeController.hpp"
#include "Addon/MessageSystem/Sockets/TSocketForward.hpp"
#include "Addon/MessageSystem/Sockets/TSocketRangeCheck.hpp"
#include "EnumStrIDs.hpp"
#include "asplib/Dynamics/asplib_DynamicsOptions.hpp"

#include "CompressorMode/CompressorModeDefaults.h"

using namespace asplib;


CCompressorModeControllerMessages::CCompressorModeControllerMessages()
{
}


CCompressorModeControllerMessages::~CCompressorModeControllerMessages()
{
}

bool CCompressorModeControllerMessages::Create(CCompressorModeController *Controller)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketRangeCheck(CCompressorModeController, float, Controller, COMPRESSOR_TAU_RELEASE_MIN,       COMPRESSOR_TAU_RELEASE_MAX,       CSocketCompressorModeIDs, UpdateTauRelease));
  sockets.push_back(CreateTSocketRangeCheck(CCompressorModeController, float, Controller, COMPRESSOR_TAU_ATTACK_MIN,        COMPRESSOR_TAU_ATTACK_MAX,        CSocketCompressorModeIDs, UpdateTauAttack));
  sockets.push_back(CreateTSocketRangeCheck(CCompressorModeController, float, Controller, COMPRESSOR_THRESHOLD_MIN,         COMPRESSOR_THRESHOLD_MAX,         CSocketCompressorModeIDs, UpdateThreshold));
  sockets.push_back(CreateTSocketRangeCheck(CCompressorModeController, float, Controller, COMPRESSOR_COMPRESSION_RATIO_MIN, COMPRESSOR_COMPRESSION_RATIO_MAX, CSocketCompressorModeIDs, UpdateCompressionRatio));
  sockets.push_back(CreateTSocketRangeCheck(CCompressorModeController, float, Controller, COMPRESSOR_KNEE_WIDTH_MIN,        COMPRESSOR_KNEE_WIDTH_MAX,        CSocketCompressorModeIDs, UpdateKneeWidth));
  sockets.push_back(CreateTSocketRangeCheck(CCompressorModeController, int,   Controller, CompressorOptions::COMPRESSION_HARD_CLIPPING, asplib::CompressorOptions::COMPRESSION_MAX, CSocketCompressorModeIDs, UpdateGainCurve));
  
  sockets.push_back(CreateTSocketForward(CCompressorModeController, Controller, CSocketCompressorModeIDs, RequestModelState));

  return Controller->SetSockets(sockets);
}
