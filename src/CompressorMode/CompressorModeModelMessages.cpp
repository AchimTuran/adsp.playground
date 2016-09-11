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
#include "EnumStrIDs.hpp"

#include "adsp.template/Addon/MessageSystem/Sockets/TSocketMemcpy.hpp"
#include "adsp.template/Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"


CCompressorModeModelMessages::CCompressorModeModelMessages()
{
}


CCompressorModeModelMessages::~CCompressorModeModelMessages()
{
}


bool CCompressorModeModelMessages::Create(CCompressorModeModel *Model)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestModelState,       CSocketCompressorModeIDs, RequestModelState));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestCompressionRatio, CSocketCompressorModeIDs, RequestCompressionRatio));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestKneeWidth,        CSocketCompressorModeIDs, RequestKneeWidth));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestGainCurve,        CSocketCompressorModeIDs, RequestGainCurve));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestTauAttack,        CSocketCompressorModeIDs, RequestTauAttack));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestTauRelease,       CSocketCompressorModeIDs, RequestTauRelease));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeModel, Model, &CCompressorModeModel::RequestThreshold,        CSocketCompressorModeIDs, RequestThreshold));

  return Model->SetSockets(sockets);
}
