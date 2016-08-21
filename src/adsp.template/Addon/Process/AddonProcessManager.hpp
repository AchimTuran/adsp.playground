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



#include "IAddonProcess.hpp"
#include "Addon/Utils/TCreationStatistics.hpp"
#include <kodi/kodi_adsp_types.h>
#include "Addon/MVC/Interfaces/MVCObject.hpp"

#include <map>
#include <vector>

// Type:
// AE_DSP_MODE_TYPE_INPUT_RESAMPLE  = 0,        /*!< @brief for input re sample */
// AE_DSP_MODE_TYPE_PRE_PROCESS     = 1,        /*!< @brief for preprocessing */
// AE_DSP_MODE_TYPE_MASTER_PROCESS  = 2,        /*!< @brief for master processing */
// AE_DSP_MODE_TYPE_POST_PROCESS    = 3,        /*!< @brief for post processing */
// AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE = 4,        /*!< @brief for output re sample */
#define REGISTER_ADDON_PROCESS_CLASS(AddonProcess, ProcessName)   public IAddonProcess,                                                                 \
                                                                  public CAddonProcessManager::TRegisterAddonProcess<AddonProcess, ProcessName>


class CAddonProcessManager
{
public:
  typedef struct AddonProcessKey_t
  {
    int               ProcessID;

    AddonProcessKey_t()
    {
      ProcessID = -1;
    }

    AddonProcessKey_t(int ID)
    {
      ProcessID = ID;
    }
  }AddonProcessKey_t;

  typedef struct
  {
    std::string       ProcessName;
    AddonProcessKey_t ProcessInfo;
  }AddonProcessInfo_t;

  typedef std::vector<AddonProcessInfo_t> AddonProcessInfoVector_t;

private:
  typedef std::vector<IAddonProcess*> AddonProcessVector_t;
  typedef IAddonProcess* (*AddonProcessCreateCallback)();
  typedef const AE_DSP_MODES::AE_DSP_MODE& (*ADSPModeSettingsCallback)();
  typedef int(*ADSPModeStatisticCallback)();
  typedef struct AddonProcessCallbacks_t
  {
    AddonProcessCreateCallback CreateProcess;
    ADSPModeStatisticCallback  GetActiveProcesses;
    ADSPModeStatisticCallback  GetCreatedProcesses;
    ADSPModeStatisticCallback  GetDestroyedProcesses;
  }AddonProcessCallbacks_t;

  class CPAddonProcessKeyCmp
  {
  public:
    bool operator()(const AddonProcessKey_t &Obj1, const AddonProcessKey_t &Obj2) const
    {
      return  Obj1.ProcessID < Obj2.ProcessID;
    }
  };

  typedef std::map<AddonProcessKey_t, AddonProcessCallbacks_t, CPAddonProcessKeyCmp> AddonProcessMap_t;
  typedef std::map<std::string, AddonProcessKey_t> AddonProcessNameMap_t;

public:
  template<class TAddonProcess, class TProcessName>
  class TRegisterAddonProcess : public TCreationStatistics<TAddonProcess>
  {
    friend class CAddonProcessManager;

  public:
    TRegisterAddonProcess() : m_HiddenID(ProcessID) {} // force registration by assinging ProcessID to m_HiddenID

    static const int  ProcessID;
    static const char* ProcessName;

    static IAddonProcess* Create() { return dynamic_cast<IAddonProcess*>(new TAddonProcess); }
    static const AE_DSP_MODES::AE_DSP_MODE& GetADSPModeSettings() { return m_ModeSettings; }

  private:
    const  int                     m_HiddenID; // force initialization for m_HiddenID
    static AddonProcessCallbacks_t m_Callbacks;
    static const AE_DSP_MODES::AE_DSP_MODE m_ModeSettings;
  };

  static AE_DSP_ERROR CreateProcesses();
  static void DestroyProcesses();

  static int RegisterAddonProcess(const std::string ProcessName, AddonProcessCallbacks_t Callbacks);

  static int GetActiveProcesses(std::string &ProcessName);
  static int GetCreatedProcesses(std::string &ProcessName);
  static int GetDestroyedProcesses(std::string &ProcessName);

  static int ConnectObject(MVCObject *Object);
  static int DisconnectObject(MVCObject *Object);

private:
  // Force initialization and creation of product map by using a static method and a std::map
  inline static AddonProcessMap_t& GetProcessMap()
  {
    static AddonProcessMap_t s_AddonProcesses;

    return s_AddonProcesses;
  }

  static bool ProcessIDSort(AddonProcessInfo_t &Process1, AddonProcessInfo_t &Process2)
  {
    return (Process1.ProcessInfo.ProcessID < Process2.ProcessInfo.ProcessID);
  }

  static std::string TranslateProcessID_TO_Str(int ProcessID)
  {
    std::string processStr = "";

    for (AddonProcessNameMap_t::iterator iter = GetAddonProcessNameMap().begin(); iter != GetAddonProcessNameMap().end(); ++iter)
    {
      if (ProcessID == iter->second.ProcessID)
      {
        processStr = iter->first;
        break;
      }
    }

    return processStr;
  }

  inline static AddonProcessNameMap_t& GetAddonProcessNameMap()
  {
    static AddonProcessNameMap_t  s_ProcessNameMappingTable;

    return s_ProcessNameMappingTable;
  }
  
  inline static AddonProcessVector_t& GetAddonProcessVector()
  {
    static AddonProcessVector_t s_AddonProcesses;

    return s_AddonProcesses;
  }

  static bool m_IsCreated;
};


template<class TAddonProcess, class TProcessName>
const int CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::ProcessID =
CAddonProcessManager::RegisterAddonProcess(TProcessName::ProcessName, CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::m_Callbacks);

template<class TAddonProcess, class TProcessName>
const char* CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::ProcessName = TProcessName::ProcessName;

template<class TAddonProcess, class TProcessName>
const AE_DSP_MODES::AE_DSP_MODE CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::m_ModeSettings;

template<class TAddonProcess, class TProcessName>
CAddonProcessManager::AddonProcessCallbacks_t
CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::m_Callbacks =
{
  CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::Create,
  CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::GetStatisticsActive,
  CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::GetStatisticsCreated,
  CAddonProcessManager::TRegisterAddonProcess<TAddonProcess, TProcessName>::GetStatisticsDestroyed
};
