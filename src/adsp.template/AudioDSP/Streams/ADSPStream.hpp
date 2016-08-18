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



#include <vector>

#include <kodi/kodi_adsp_types.h>

#include "AudioDSP/FactoryADSPModes/IADSPMode.hpp"

class IADSPStreamBuilder;


class CADSPStream
{
  friend class IADSPStreamBuilder;

public:
  typedef std::vector<IADSPMode*> ADSPModeVector_t;

  CADSPStream();
  ~CADSPStream();

  AE_DSP_ERROR Create(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties);
  AE_DSP_ERROR Destroy();

  AE_DSP_ERROR StreamIsModeSupported(AE_DSP_MODE_TYPE ModeType, unsigned int ModeID, int UniqueDBModeID);

  unsigned int ProcessMode(unsigned int ModeID, float **ArrayIn, float **ArrayOut, unsigned int Samples);
  unsigned int ProcessMode(AE_DSP_MODE_TYPE ModeType, float **ArrayIn, float **ArrayOut, unsigned int Samples);
  unsigned int ProcessMode(AE_DSP_MODE_TYPE ModeType, const float **Array_in, unsigned int Samples);

  unsigned int NeededSamplesize(unsigned int ModeID);
  unsigned int NeededSamplesize(AE_DSP_MODE_TYPE ModeType);
  float GetDelay(unsigned int ModeID);
  float GetDelay(AE_DSP_MODE_TYPE ModeType);

  // Optional Methods for Master Modes
  int MasterProcessGetOutChannels(unsigned long &OutChannelFlags);
  const char* MasterProcessGetStreamInfoString();
  AE_DSP_ERROR MasterProcessSetMode(AE_DSP_STREAMTYPE StreamType, unsigned int ModeID, int UniqueDBModeID);

  // Optional Resampling Methods
  int ResamplingRate(AE_DSP_MODE_TYPE ModeType);

private:
  // Gets current active master, input- or output-resampling process by mode type
  inline IADSPMode* GetSingleMode(AE_DSP_MODE_TYPE ModeType)
  {
    IADSPMode *mode = NULL;
    switch (ModeType)
    {
      case AE_DSP_MODE_TYPE_INPUT_RESAMPLE:
        mode = m_CurrentInputResampleMode;
      break;

      case AE_DSP_MODE_TYPE_MASTER_PROCESS:
        mode = m_CurrentMasterMode;
      break;

      case AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE:
        mode = m_CurrentOutputResampleMode;
      break;
    }

    return mode;
  }

  ADSPModeVector_t  m_ADSPModeVector;
  unsigned int      m_MaxADSPModes;
  IADSPMode         **m_ADSPModes;
  IADSPMode         *m_CurrentMasterMode;
  IADSPMode         *m_CurrentInputResampleMode;
  IADSPMode         *m_CurrentOutputResampleMode;
};
