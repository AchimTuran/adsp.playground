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



#include "IADSPMode.hpp"
#include "Addon/Utils/TCreationStatistics.hpp"
#include <kodi/kodi_adsp_types.h>

#include <map>
#include <vector>

// Type:
// AE_DSP_MODE_TYPE_INPUT_RESAMPLE  = 0,        /*!< @brief for input re sample */
// AE_DSP_MODE_TYPE_PRE_PROCESS     = 1,        /*!< @brief for preprocessing */
// AE_DSP_MODE_TYPE_MASTER_PROCESS  = 2,        /*!< @brief for master processing */
// AE_DSP_MODE_TYPE_POST_PROCESS    = 3,        /*!< @brief for post processing */
// AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE = 4,        /*!< @brief for output re sample */
#define REGISTER_ADSP_MODE_CLASS(ADSPMode, ModeName, ModeSettings, Type)  public IADSPMode,                                                                   \
                                                                          public CFactoryADSPModes::TRegisterADSPMode<ADSPMode, ModeName, ModeSettings, Type>


class CFactoryADSPModes
{
public:
  typedef struct ADSPModeKey_t
  {
    int               ModeID;
    AE_DSP_MODE_TYPE  ModeType;

    ADSPModeKey_t()
    {
      ModeID = -1;
      ModeType = AE_DSP_MODE_TYPE_UNDEFINED;
    }

    ADSPModeKey_t(int ID, AE_DSP_MODE_TYPE Type)
    {
      ModeID = ID;
      ModeType = Type;
    }
  }ADSPModeKey_t;

  typedef struct
  {
    std::string   ModeName;
    ADSPModeKey_t ModeInfo;
  }ADSPModeInfo_t;

  typedef std::vector<ADSPModeInfo_t> ADSPModeInfoVector_t;

private:
  typedef IADSPMode* (*ADSPModeCreateCallback)();
  typedef const AE_DSP_MODES::AE_DSP_MODE& (*ADSPModeSettingsCallback)();
  typedef int(*ADSPModeStatisticCallback)();
  typedef struct ADSPModeCallbacks_t
  {
    ADSPModeCreateCallback     CreateADSPMode;
    ADSPModeStatisticCallback  GetActiveADSPMode;
    ADSPModeStatisticCallback  GetCreatedADSPMode;
    ADSPModeStatisticCallback  GetDestroyedADSPMode;
    ADSPModeSettingsCallback   GetADSPModeSettings;
  }ADSPModeCallbacks_t;

  class CADSPModeKeyCmp
  {
  public:
    bool operator()(const ADSPModeKey_t &Obj1, const ADSPModeKey_t &Obj2) const
    {
      return  Obj1.ModeID < Obj2.ModeID || (Obj1.ModeID == Obj2.ModeID  && Obj1.ModeType < Obj2.ModeType);
    }
  };

  typedef std::map<ADSPModeKey_t, ADSPModeCallbacks_t, CADSPModeKeyCmp> ADSPModeMap_t;
  typedef std::map<std::string, ADSPModeKey_t> ADSPModeNameMap_t;

public:
  template<class TADSPMode, class TModeName, class TModeSettings, AE_DSP_MODE_TYPE TType>
  class TRegisterADSPMode : public TCreationStatistics<TADSPMode>
  {
    friend class CFactoryADSPModes;

  public:
    TRegisterADSPMode() : m_HiddenID(ModeID) {} // force registration by assinging ModeID to m_HiddenID

    static const int  ModeID;
    static const AE_DSP_MODE_TYPE ModeType;
    static const char* ModeName;

    static IADSPMode* Create() { return dynamic_cast<IADSPMode*>(new TADSPMode); }
    static const AE_DSP_MODES::AE_DSP_MODE& GetADSPModeSettings() { return m_ModeSettings; }

  private:
    const  int                 m_HiddenID; // force initialization for m_HiddenID
    static const TModeSettings m_ModeSettings;
    static ADSPModeCallbacks_t m_Callbacks;
  };

  static AE_DSP_ERROR Create(int ModeID, AE_DSP_MODE_TYPE ModeType, IADSPMode *&InterfacePtr);
  static void Destroy(IADSPMode *&ADSPMode);
  static int RegisterADSPMode(const std::string ModeName, AE_DSP_MODE_TYPE ModeType, ADSPModeCallbacks_t Callbacks);
  static AE_DSP_ERROR GetAvailableModes(ADSPModeInfoVector_t &ModeInfos);
  static AE_DSP_ERROR GetADSPModeSettings(int ModeID, AE_DSP_MODE_TYPE ModeType, AE_DSP_MODES::AE_DSP_MODE &adspModeSettings);

  static int GetActiveADSPMode(std::string &ModeName);
  static int GetCreatedADSPMode(std::string &ModeName);
  static int GetDestroyedADSPMode(std::string &ModeName);

private:
  // Force initialization and creation of product map by using a static method and a std::map
  inline static ADSPModeMap_t& GetADSPModeMap()
  {
    static ADSPModeMap_t s_ADSPModes;

    return s_ADSPModes;
  }

  static bool ModeIDSort(ADSPModeInfo_t &Mode1, ADSPModeInfo_t &Mode2)
  {
    return (Mode1.ModeInfo.ModeID < Mode2.ModeInfo.ModeID);
  }

  inline static ADSPModeNameMap_t& GetADSPModeNameMap()
  {
    static ADSPModeNameMap_t  s_ModeNameMappingTable;

    return s_ModeNameMappingTable;
  }
};


template<class TADSPMode, class TModeName, class TModeSettings, AE_DSP_MODE_TYPE TType>
const int CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::ModeID =
CFactoryADSPModes::RegisterADSPMode(TModeName::ModeName, TType, CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::m_Callbacks);

template<class TADSPMode, class TModeName, class TModeSettings, AE_DSP_MODE_TYPE TType>
const AE_DSP_MODE_TYPE CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::ModeType = TType;

template<class TADSPMode, class TModeName, class TModeSettings, AE_DSP_MODE_TYPE TType>
const char* CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::ModeName = TModeName::ModeName;

template<class TADSPMode, class TModeName, class TModeSettings, AE_DSP_MODE_TYPE TType>
const TModeSettings CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::m_ModeSettings;

template<class TADSPMode, class TModeName, class TModeSettings, AE_DSP_MODE_TYPE TType>
CFactoryADSPModes::ADSPModeCallbacks_t
CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::m_Callbacks =
{
  CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::Create,
  CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::GetStatisticsActive,
  CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::GetStatisticsCreated,
  CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::GetStatisticsDestroyed,
  CFactoryADSPModes::TRegisterADSPMode<TADSPMode, TModeName, TModeSettings, TType>::GetADSPModeSettings
};
