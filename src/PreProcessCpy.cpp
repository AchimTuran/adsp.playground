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



#include "PreProcessCpy.hpp"

#include "adsp.template/ADSPHelpers.h"
#include "adsp.template/include/client.h"

using namespace ADDON;


CPreProcessCpy::CPreProcessCpy()
{
}


CPreProcessCpy::~CPreProcessCpy()
{
}


AE_DSP_ERROR CPreProcessCpy::ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties)
{
  m_InChannels            = Settings.iInChannels;
  m_InChannelPresentFlags = Settings.lInChannelPresentFlags;

  // reset used channel mapping array
  for (int ch = 0; ch < AE_DSP_CH_MAX; ch++)
  {
    m_ChannelMappingIdx[ch] = -1;
    m_ChannelMappingFlags[ch] = 0;
  }

  // add used channels to channel mapping array
  int lastAudioChannel = 0;
  for (int ch = 0; ch < m_InChannels; ch++)
  {
    unsigned long tempChannelFlag = 1 << lastAudioChannel;
    m_ChannelMappingIdx[ch] = CADSPHelpers::GetNextChID(m_InChannelPresentFlags,
                                                        CADSPHelpers::Translate_ChFlag_TO_ChID((AE_DSP_CHANNEL_PRESENT)tempChannelFlag));
    m_ChannelMappingFlags[ch] = CADSPHelpers::Translate_ChID_TO_ChFlag((AE_DSP_CHANNEL)m_ChannelMappingIdx[ch]);
    if (m_ChannelMappingIdx[ch] == AE_DSP_CH_INVALID)
    {
      KODI->Log(LOG_ERROR, "%s line %i: Tried to create invalid channel mappings!", __FUNCTION__, __LINE__);
      return AE_DSP_ERROR_FAILED;
    }
    lastAudioChannel = m_ChannelMappingIdx[ch] + 1;
  }
  

  return AE_DSP_ERROR_NO_ERROR;
}


void CPreProcessCpy::ModeDestroy()
{
}


// Requiered Processing Methods
unsigned int CPreProcessCpy::ModeProcess(float **ArrayIn, float **ArrayOut, unsigned int Samples)
{
  for (int ch = 0; ch < m_InChannels; ch++)
  { 
    memcpy(ArrayOut[m_ChannelMappingIdx[ch]], ArrayIn[m_ChannelMappingIdx[ch]], sizeof(float)*Samples);
  }

  return Samples;
}
