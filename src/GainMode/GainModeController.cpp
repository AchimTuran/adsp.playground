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



#include "Addon/MVC/Model/TParameter.hpp"
#include "GainMode/GainModeController.hpp"
#include "EnumStrIDs.hpp"

#define MAX_GAIN 24.0f

CGainModeController::CGainModeController() :
  IController("GainModeController", 0) // TODO: create ID list
{
}


CGainModeController::~CGainModeController()
{
}


int CGainModeController::Create()
{
  if (!CGainModeControllerMessages::Create(this))
  {
    // TODO error message
    return 0;
  }

  return 0;
}

void CGainModeController::Destroy()
{
}

int CGainModeController::SetMainGain(Message &Msg)
{
  float gain = *((float*)(Msg.data));

  if (gain >= MAX_GAIN || gain <= -MAX_GAIN)
  {
    // TODO: error code
    return -1;
  }

  m_MainGain = gain;
  if (!this->SendMsg(&m_MainGain, sizeof(float), CSocketGainModeIDs::MainGain))
  {
    // TODO: error code
    return -1;
  }

  return 0;
}

