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



#include "GainMode/GainModeModelMessages.hpp"
#include "GainMode/GainModeModel.hpp"
#include "EnumStrIDs.hpp"

#include "adsp.template/Addon/MessageSystem/Sockets/TSocketMemcpy.hpp"
#include "adsp.template/Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"


CGainModeModelMessages::CGainModeModelMessages()
{
}


CGainModeModelMessages::~CGainModeModelMessages()
{
}


bool CGainModeModelMessages::Create(CGainModeModel *Model)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_FL));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_FR));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_FC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_LFE));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_BL));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_BR));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_FLOC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_FROC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_BC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_SL));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_SR));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TFL));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TFR));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TFC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TBL));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TBR));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_TBC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_BLOC));
  sockets.push_back(CreateTSocketClassMethodCallback(CGainModeModel, Model, &CGainModeModel::RequestGain, CSocketGainModeIDs, RequestGain_BROC));

  return Model->SetSockets(sockets);
}
