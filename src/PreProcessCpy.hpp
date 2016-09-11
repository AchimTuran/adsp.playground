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



#include "adsp.template/AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp"
#include "PreProcessCpyModeSettings.hpp"

#include "EnumStrIDs.hpp"

class CPreProcessCpyModeName
{
public:
  static const std::string ModeName;
};

const std::string CPreProcessCpyModeName::ModeName = CADSPModeIDs::ToString(CADSPModeIDs::PreProcessingModeCopy);


class CPreProcessCpy : REGISTER_ADSP_MODE_CLASS(CPreProcessCpy, CPreProcessCpyModeName, CPreProcessCpyModeSettings, AE_DSP_MODE_TYPE_PRE_PROCESS)
{
public:
  CPreProcessCpy();
  ~CPreProcessCpy();

  virtual AE_DSP_ERROR ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties);
  virtual void ModeDestroy();

  // Requiered Processing Methods
  virtual unsigned int ModeProcess(float **ArrayIn, float **ArrayOut, unsigned int Samples);

private:
  int           m_InChannels;               /*!< @brief the amount of input channels */
  unsigned long m_InChannelPresentFlags;    /*!< @brief the exact channel mapping flags of input */
  
  int           m_ChannelMappingIdx[AE_DSP_CH_MAX];
  unsigned long m_ChannelMappingFlags[AE_DSP_CH_MAX];
};
