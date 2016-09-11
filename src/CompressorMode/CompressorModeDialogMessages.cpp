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



#include "CompressorMode/CompressorModeDialogMessages.hpp"
#include "CompressorMode/CompressorModeDialog.hpp"
#include "EnumStrIDs.hpp"

#include "adsp.template/Addon/MessageSystem/Sockets/TSocketMemcpy.hpp"
#include "adsp.template/Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"


CCompressorModeDialogMessages::CCompressorModeDialogMessages()
{
}


CCompressorModeDialogMessages::~CCompressorModeDialogMessages()
{
}


bool CCompressorModeDialogMessages::Create(CCompressorModeDialog *Dialog)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeDialog, Dialog, &CCompressorModeDialog::UpdateTauRelease,        CSocketCompressorModeIDs, UpdateTauRelease));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeDialog, Dialog, &CCompressorModeDialog::UpdateTauAttack,         CSocketCompressorModeIDs, UpdateTauAttack));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeDialog, Dialog, &CCompressorModeDialog::UpdateThreshold,         CSocketCompressorModeIDs, UpdateThreshold));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeDialog, Dialog, &CCompressorModeDialog::UpdateCompressionRation, CSocketCompressorModeIDs, UpdateCompressionRatio));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeDialog, Dialog, &CCompressorModeDialog::UpdateKneeWidth,         CSocketCompressorModeIDs, UpdateKneeWidth));
  sockets.push_back(CreateTSocketClassMethodCallback(CCompressorModeDialog, Dialog, &CCompressorModeDialog::UpdateGainCurve,         CSocketCompressorModeIDs, UpdateGainCurve));

  return Dialog->SetSockets(sockets);
}
