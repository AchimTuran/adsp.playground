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



#include "CompressorMode/CompressorModeModelMessages.hpp"
#include "CompressorMode/CompressorModeModel.hpp"
#include "Addon/MessageSystem/Sockets/TSocketMemcpy.hpp"
#include "Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"
#include "EnumStrIDs.hpp"
#include "EnumStrIDs.hpp"


CCompressorModeModelMessages::CCompressorModeModelMessages()
{
}


CCompressorModeModelMessages::~CCompressorModeModelMessages()
{
}


bool CCompressorModeModelMessages::Create(CCompressorModeModel *Model)
{
  SocketVector_t sockets;
  
  sockets.push_back(CreateTSocketMemcpy(float, &(Model->m_TauRelease),       CSocketCompressorModeIDs, UpdateTauRelease));
  sockets.push_back(CreateTSocketMemcpy(float, &(Model->m_TauAttack),        CSocketCompressorModeIDs, UpdateTauAttack));
  sockets.push_back(CreateTSocketMemcpy(float, &(Model->m_Threshold),        CSocketCompressorModeIDs, UpdateThreshold));
  sockets.push_back(CreateTSocketMemcpy(float, &(Model->m_CompressionRatio), CSocketCompressorModeIDs, UpdateCompressionRatio));
  sockets.push_back(CreateTSocketMemcpy(float, &(Model->m_KneeWidth),        CSocketCompressorModeIDs, UpdateKneeWidth));
  sockets.push_back(CreateTSocketMemcpy(int,   &(Model->m_GainCurve),        CSocketCompressorModeIDs, UpdateGainCurve));

  return Model->SetSockets(sockets);
}
