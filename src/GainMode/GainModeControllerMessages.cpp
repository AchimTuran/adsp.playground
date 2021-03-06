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



#include "GainMode/GainModeControllerMessages.hpp"
#include "GainMode/GainModeController.hpp"
#include "EnumStrIDs.hpp"
#include "adsp.template/Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"


CGainModeControllerMessages::CGainModeControllerMessages()
{
}


CGainModeControllerMessages::~CGainModeControllerMessages()
{
}

bool CGainModeControllerMessages::Create(CGainModeController *Controller)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeController, Controller, &CGainModeController::SetMainGain, CSocketGainModeIDs, UpdateMainGain));

  return Controller->SetSockets(sockets);
}
