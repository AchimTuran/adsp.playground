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
#include "adsp.template/Addon/MessageSystem/Sockets/TSocketRangeCheck.hpp"
#include "GainModeDefaults.h"


CGainModeControllerMessages::CGainModeControllerMessages()
{
}


CGainModeControllerMessages::~CGainModeControllerMessages()
{
}

bool CGainModeControllerMessages::Create(CGainModeController *Controller)
{
  SocketVector_t sockets;

  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_FL));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_FR));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_FC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_LFE));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_BL));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_BR));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_FLOC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_FROC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_BC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_SL));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_SR));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TFL));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TFR));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TFC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TBL));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TBR));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_TBC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_BLOC));
  sockets.push_back(CreateTSocketRangeCheck(CGainModeController, float, Controller, GAIN_MIN, GAIN_MAX, CSocketGainModeIDs, UpdateGain_BROC));

  return Controller->SetSockets(sockets);
}
