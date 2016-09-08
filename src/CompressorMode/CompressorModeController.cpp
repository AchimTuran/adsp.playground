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
#include "CompressorMode/CompressorModeController.hpp"
#include "EnumStrIDs.hpp"

#define MAX_Compressor 90.0f

CCompressorModeController::CCompressorModeController() :
  IController(CDispatcherIDs::ToString(CDispatcherIDs::CompressorModeController),
              CDispatcherIDs::CompressorModeController,
              CADSPModeIDs::PostProcessingModeCompressor)
{
}


CCompressorModeController::~CCompressorModeController()
{
}


int CCompressorModeController::Create()
{
  if (!CCompressorModeControllerMessages::Create(this))
  {
    // TODO error message
    return -1;
  }

  return 0;
}

void CCompressorModeController::Destroy()
{
}
