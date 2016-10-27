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
#include "adsp.template/Addon/MVC/Interfaces/View/IView.hpp"
#include "CompressorModeSettings.hpp"
#include "CompressorModeMessages.hpp"

#include "asplib/Dynamics/Interfaces/asplib_IDynamics.hpp"
#include "asplib/Dynamics/Compressor/asplib_DynamicsCompressorOptions.hpp"

#include "EnumStrIDs.hpp"


class CCompressorModeName
{
public:
  static const std::string ModeName;
};


class CCompressorMode : REGISTER_ADSP_MODE_CLASS(CCompressorMode, CCompressorModeName, CCompressorModeSettings, AE_DSP_MODE_TYPE_POST_PROCESS),
                        public CCompressorModeMessages,
                        public IView
{
  friend class CCompressorModeMessages;

public:
  CCompressorMode();
  ~CCompressorMode();

  virtual AE_DSP_ERROR ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties);
  virtual AE_DSP_ERROR ModeInitialize(const AE_DSP_SETTINGS &Settings);
  virtual void ModeDestroy();

  // TODO remove
  virtual int Create() { return 0; }
  virtual void Destroy() {}

  // Requiered Processing Methods
  virtual unsigned int ModeProcess(float **ArrayIn, float **ArrayOut, unsigned int Samples);

private:
  int SetTauRelease(Message &Msg);
  int SetTauAttack(Message &Msg);
  int SetThreshold(Message &Msg);
  int SetCompressionRation(Message &Msg);
  int SetKneeWidth(Message &Msg);
  int SetGainCurve(Message &Msg);

private:
  int ConfigureCompressors();
  asplib::CompressorOptions m_CompressorOptions[AE_DSP_CH_MAX];

  int           m_InChannels;               /*!< @brief the amount of input channels */
  unsigned long m_InChannelPresentFlags;    /*!< @brief the exact channel mapping flags of input */
  
  int           m_ChannelMappingIdx[AE_DSP_CH_MAX];
  unsigned long m_ChannelMappingFlags[AE_DSP_CH_MAX];

  uint32_t m_SampleFrequency;
  uint32_t m_FrameSize;

  asplib::IDynamics *m_Compressors[AE_DSP_CH_MAX];
  // Process parameters
  float m_MainCompressor;
};
