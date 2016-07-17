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



#include "GainMode/GainModeMessages.hpp"
#include "GainMode/GainMode.hpp"
#include "Addon/MessageSystem/Communication/ActorProtocol.h"
#include "Addon/MessageSystem/Sockets/TSocketMemcpy.hpp"
#include "EnumStrIDs.hpp"


CGainModeMessages::CGainModeMessages() :
  CMessageDispatcher(new CActorProtocol(CADSPModeIDs::ToString(CADSPModeIDs::PostProcessingModeGain)),
                     CADSPModeIDs::ToString(CADSPModeIDs::PostProcessingModeGain))
{
}


CGainModeMessages::~CGainModeMessages()
{
}


bool CGainModeMessages::Create(CGainMode *Mode)
{
  SocketVector_t sockets;
  
  sockets.push_back(CreateTSocketMemcpy(float, &(Mode->m_MainGain), CSocketGainModeIDs, MainGain));

  return this->SetSockets(sockets);
}
