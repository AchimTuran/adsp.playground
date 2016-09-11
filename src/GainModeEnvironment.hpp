#pragma once
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



#include "adsp.template/Addon/Process/AddonProcessManager.hpp"

#include "GainMode/GainModeModel.hpp"
#include "GainMode/GainModeController.hpp"

#include <KodiThreads/threads/Thread.h>


class CGainModeEnvironmentName
{
public:
  static const std::string ProcessName;
};


class CGainModeEnvironment : 
  private CThread,
  REGISTER_ADDON_PROCESS_CLASS(CGainModeEnvironment, CGainModeEnvironmentName)
{
public:
  CGainModeEnvironment();
  ~CGainModeEnvironment();

  virtual AE_DSP_ERROR Create();
  virtual AE_DSP_ERROR Destroy();
  
  //virtual bool ConnectDispatcher(MVCObject *Object);
  //virtual bool DisconnectDispatcher(MVCObject *Object);

private: // private member methods
  int InitGainModel();
  virtual void Process();
  
private: // private member variables
  CGainModeModel      m_GainModeModel;
  CGainModeController m_GainModeController;
};
