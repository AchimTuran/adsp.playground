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



#include <kodi/kodi_adsp_types.h>

#include "ADSPStream.hpp"
#include "AudioDSP/FactoryADSPModes/IADSPMode.hpp"


class IADSPStreamBuilder
{
public:
  virtual AE_DSP_ERROR ConstructStream(CADSPStream &ADSPStream, const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties) = 0;
  virtual AE_DSP_ERROR InitializeStream(CADSPStream &ADSPStream, const AE_DSP_SETTINGS *Settings) = 0;

  virtual AE_DSP_ERROR AddMode(CADSPStream &ADSPStream, IADSPMode *ADSPMode) = 0;
  virtual AE_DSP_ERROR AddMode(CADSPStream &ADSPStream, unsigned int ModeID) = 0;

  virtual AE_DSP_ERROR RemoveMode(CADSPStream &ADSPStream, IADSPMode *ADSPMode) = 0;
  virtual AE_DSP_ERROR RemoveMode(CADSPStream &ADSPStream, unsigned int ModeID) = 0;

protected:
  class CADSPStreamAccessor
  {
  public:
    static inline CADSPStream::ADSPModeVector_t& m_ADSPModeVector(CADSPStream &ADSPStream)
    {
      return ADSPStream.m_ADSPModeVector;
    }

    static inline unsigned int& m_MaxADSPModes(CADSPStream &ADSPStream)
    {
      return ADSPStream.m_MaxADSPModes;
    }

    static inline IADSPMode**& m_ADSPModes(CADSPStream &ADSPStream)
    {
      return ADSPStream.m_ADSPModes;
    }
  };
};
