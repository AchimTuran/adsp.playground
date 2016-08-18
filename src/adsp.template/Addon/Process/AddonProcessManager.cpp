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


#include "AddonProcessManager.hpp"

#include "include/client.h"

#include <string>
#include <algorithm>

using namespace std;
using namespace ADDON;


// static member variables
bool CAddonProcessManager::m_IsCreated = false;


AE_DSP_ERROR CAddonProcessManager::CreateProcesses()
{
  if (GetProcessMap().size() <= 0)
  {
    KODI->Log(LOG_NOTICE, "%s, %i, Tried to create CAddonProcessManager without any registered AddonProcesses. Consequently CAddonProcessManager will be disabled.", __FUNCTION__, __LINE__);

    return AE_DSP_ERROR_IGNORE_ME;
  }

  AddonProcessVector_t &addonProcesses = GetAddonProcessVector();
  AddonProcessMap_t &processMap = GetProcessMap();
  for (AddonProcessMap_t::iterator iter = processMap.begin(); iter != processMap.end(); ++iter)
  {
    IAddonProcess* process  = iter->second.CreateProcess();
    string processStr       = TranslateProcessID_TO_Str(iter->first.ProcessID);

    if (processStr == "")
    {
      KODI->Log(LOG_NOTICE, "%s, %i, Could not retrieve AddonProcess name! Please contact the author of this add-on with this log file.", __FUNCTION__, __LINE__);
    }

    AE_DSP_ERROR err = process->Create();
    if (err != AE_DSP_ERROR_NO_ERROR)
    {
      delete process;
      process = NULL;
      KODI->Log(LOG_ERROR, "%s, %i, Failed to create AddonProcess %s with ID: %i!", __FUNCTION__, __LINE__, processStr.c_str(), iter->first.ProcessID);
    }

    if (process)
    {
      KODI->Log(LOG_DEBUG, "%s, %i, Successful created AddonProcess %s with ID: %i", __FUNCTION__, __LINE__, processStr.c_str(), iter->first.ProcessID);
      addonProcesses.push_back(process);
    }
  }

  KODI->Log(LOG_DEBUG, "%s, %i, Successful created %i from %i AddonProcesses, %i failed", __FUNCTION__, __LINE__, addonProcesses.size(), processMap.size(), processMap.size() - addonProcesses.size());

  m_IsCreated = true;

  return AE_DSP_ERROR_NO_ERROR;
}


void CAddonProcessManager::DestroyProcesses()
{
  AddonProcessVector_t &addonProcesses = GetAddonProcessVector();
  for (unsigned int ii = 0; ii < addonProcesses.size(); ii++)
  {
    if (addonProcesses[ii])
    {
      AE_DSP_ERROR err = addonProcesses[ii]->Destroy();
      if (err != AE_DSP_ERROR_NO_ERROR)
      {
        KODI->Log(LOG_ERROR, "%s, %i, An error occured during AddonProcess destruction! Please contact the author of this add-on with this log file.", __FUNCTION__, __LINE__);
      }

      delete addonProcesses[ii];
    }

    addonProcesses[ii] = NULL;
  }

  addonProcesses.clear();
  m_IsCreated = false;
}


int CAddonProcessManager::RegisterAddonProcess(const std::string ProcessName, AddonProcessCallbacks_t Callbacks)
{
  if (m_IsCreated)
  {
    KODI->Log(LOG_ERROR, "%s, %i, It is not possible to add new AddonProcesses, when the CAddonProcessManager is already created!", __FUNCTION__, __LINE__);
    
    return -1;
  }

  static int processID=0;
  AddonProcessKey_t processKey(processID);
  AddonProcessMap_t::iterator iter = GetProcessMap().find(processKey);
  if (iter == GetProcessMap().end())
  {// only register not yet known products
    GetProcessMap()[processKey] = Callbacks;
  }

  GetAddonProcessNameMap()[ProcessName] = processKey;

  return processID++;
}


int CAddonProcessManager::GetActiveProcesses(std::string &ProcessName)
{
  AddonProcessNameMap_t &addonProcessNameMappingTable = GetAddonProcessNameMap();
  AddonProcessNameMap_t::iterator iterModeKey = addonProcessNameMappingTable.find(ProcessName);
  if (iterModeKey == addonProcessNameMappingTable.end())
  {
    return -1;
  }

  AddonProcessMap_t::iterator iterCB = GetProcessMap().find((*iterModeKey).second);
  if (iterCB == GetProcessMap().end())
  {
    return -1;
  }

  return (*iterCB).second.GetActiveProcesses();
}


int CAddonProcessManager::GetCreatedProcesses(std::string &ProcessName)
{
  AddonProcessNameMap_t &addonProcessNameMappingTable = GetAddonProcessNameMap();
  AddonProcessNameMap_t::iterator iterModeKey = addonProcessNameMappingTable.find(ProcessName);
  if (iterModeKey == addonProcessNameMappingTable.end())
  {
    return -1;
  }

  AddonProcessMap_t::iterator iterCB = GetProcessMap().find((*iterModeKey).second);
  if (iterCB == GetProcessMap().end())
  {
    return -1;
  }

  return (*iterCB).second.GetCreatedProcesses();
}

int CAddonProcessManager::GetDestroyedProcesses(std::string &ProcessName)
{
  AddonProcessNameMap_t &addonProcessNameMappingTable = GetAddonProcessNameMap();
  AddonProcessNameMap_t::iterator iterModeKey = addonProcessNameMappingTable.find(ProcessName);
  if (iterModeKey == addonProcessNameMappingTable.end())
  {
    return -1;
  }

  AddonProcessMap_t::iterator iterCB = GetProcessMap().find((*iterModeKey).second);
  if (iterCB == GetProcessMap().end())
  {
    return -1;
  }

  return (*iterCB).second.GetDestroyedProcesses();
}

int CAddonProcessManager::ConnectDispatcher(CMessageDispatcher *Dispatcher)
{
  AddonProcessVector_t &addonProcesses = GetAddonProcessVector();
  for (AddonProcessVector_t::iterator iter = addonProcesses.begin(); iter != addonProcesses.end(); ++iter)
  {
    (*iter)->ConnectDispatcher(Dispatcher);
  }

  return 0;
}

int CAddonProcessManager::DisconnectDispatcher(CMessageDispatcher *Dispatcher)
{
  AddonProcessVector_t &addonProcesses = GetAddonProcessVector();
  for (AddonProcessVector_t::iterator iter = addonProcesses.begin(); iter != addonProcesses.end(); ++iter)
  {
    (*iter)->DisconnectDispatcher(Dispatcher);
  }

  return 0;
}
