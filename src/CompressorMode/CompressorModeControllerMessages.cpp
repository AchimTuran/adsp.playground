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
#include "Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"
#include "EnumStrIDs.hpp"


CCompressorModeControllerMessages::CCompressorModeControllerMessages()
{
}


CCompressorModeControllerMessages::~CCompressorModeControllerMessages()
{
}

bool CCompressorModeControllerMessages::Create(CCompressorModeController *Controller)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeController, Controller, &CCompressorModeController::SetTauRelease,        CSocketCompressorModeIDs, UpdateTauRelease));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeController, Controller, &CCompressorModeController::SetTauAttack,         CSocketCompressorModeIDs, UpdateTauAttack));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeController, Controller, &CCompressorModeController::SetThreshold,         CSocketCompressorModeIDs, UpdateThreshold));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeController, Controller, &CCompressorModeController::SetCompressionRation, CSocketCompressorModeIDs, UpdateCompressionRatio));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeController, Controller, &CCompressorModeController::SetKneeWidth,         CSocketCompressorModeIDs, UpdateKneeWidth));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeController, Controller, &CCompressorModeController::SetGainCurve,         CSocketCompressorModeIDs, UpdateGainCurve));

  return Controller->SetSockets(sockets);
}
