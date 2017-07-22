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


#include "kodi/addon-instance/AudioDSP.h"

#include "adsp.template/ADSPHelpers.h"

#include <memory.h>

class CFactoryADSPModes;


class IADSPMode
{
  friend class CFactoryADSPModes;

public:
  IADSPMode()
  {
    ResetStreamSettings();
  }

  virtual ~IADSPMode()
  {
    ResetStreamSettings();
  }


// Fixed public methods
  AUDIODSP_ADDON_ERROR Create(AUDIODSP_ADDON_AUDIO_FORMAT m_inputFormat, AUDIODSP_ADDON_AUDIO_FORMAT m_outputFormat, const AUDIODSP_ADDON_STREAM_PROPERTIES *pProperties)
  {
    return ModeCreate(m_inputFormat, m_outputFormat, m_streamProperties);
  }

// Requiered Create/Destroy Methods
  virtual AUDIODSP_ADDON_ERROR ModeCreate(AUDIODSP_ADDON_AUDIO_FORMAT m_inputFormat, AUDIODSP_ADDON_AUDIO_FORMAT m_outputFormat, const AUDIODSP_ADDON_STREAM_PROPERTIES &pProperties) = 0;
  virtual void ModeDestroy() = 0;

// Requiered Processing Methods
  virtual unsigned int ModeProcess(float **Array_in, float **Array_out, unsigned int Samples) = 0;
  

// Optional common Methods
  virtual unsigned int NeededSamplesize()
  {
    return 0;
  }

  virtual float GetDelay()
  {
    return (float)m_inputFormat.frameSize / (float)m_inputFormat.frameSize;
  }

  //! This method checks the input stream and can filter processing mode types.
  /*!
  * Ask the add-on about a requested processing mode that it is supported on the current
  * stream. Is called about every add-on mode after successed StreamCreate.
  * @param Type The processing mode type, see AE_DSP_MODE_TYPE for definitions
  * @param ModeID The mode inside add-on which must be performed on call.
  * Id is set from add-on and can be modified templateConfiguration.cpp by
  * editing adspPreModeNum array. Best practise is to use a global enum to
  * have only a mode id once. For example see adspProcessingModeIDs in
  * templateConfiguration.h
  * @param UniqueDBModeID The Mode unique id generated from dsp database.
  * @return AUDIODSP_ADDON_ERROR_NO_ERROR if the properties were fetched successfully.
  * If the stream is not supported the ADSP addon must return AUDIODSP_ADDON_ERROR_IGNORE_ME.
  * @remarks By default this method accept all processing types.
  * If you wanna filter processing mode types then you have to overload this method in
  * your processing class.
  */
  virtual AUDIODSP_ADDON_ERROR StreamIsModeSupported(unsigned int ModeID, int UniqueDBModeID)
  {
    UniqueDBModeID; // prevent compiler warnings, because adsp.template doesn't use this information

    if (ModeID != m_ModeID)
    {
      return AUDIODSP_ADDON_ERROR_IGNORE_ME;
    }

    return AUDIODSP_ADDON_ERROR_NO_ERROR;
  }


// Optional Methods for Master Modes
  virtual int MasterProcessGetOutChannels(unsigned long &OutChannelFlags)
  {
    return -1;
  }

  //!  Returns an information string, which will be shown in the UI.
  /*!
   * @return A string to show. This method always returns a empty string.
   * @remarks If you wanna return a custom string, then you must overload this method
   * in your processing class.
   */
  virtual const char *MasterProcessGetStreamInfoString()
  {
    return "";
  }


// Optional Resampling Methods
  virtual int ResamplingRate()
  {
    return m_inputFormat.sampleRate;
  }

protected:
  unsigned int      m_ModeID;
  ////! ToDo: description.
  ///*!
  //* Returns ToDo!
  //* @return ToDo!
  //* @remarks ToDo!
  //*/
  //virtual AUDIODSP_ADDON_ERROR GetCustomStreamInfos(void *CustomStreamSettings)
  //{
  //  return AUDIODSP_ADDON_ERROR_NO_ERROR;
  //}

private:
  void ResetStreamSettings()
  {
    memset(&m_inputFormat, 0, sizeof(AUDIODSP_ADDON_AUDIO_FORMAT));
    memset(&m_outputFormat, 0, sizeof(AUDIODSP_ADDON_AUDIO_FORMAT));
    memset(&m_streamProperties, 0, sizeof(AUDIODSP_ADDON_STREAM_PROPERTIES));
  }

  //! Used stream settings for details see AE_DSP_SETTINGS.
  AUDIODSP_ADDON_AUDIO_FORMAT m_inputFormat;
  AUDIODSP_ADDON_AUDIO_FORMAT m_outputFormat;
  //! Used stream properties for details see AE_DSP_STREAM_PROPERTIES.
  AUDIODSP_ADDON_STREAM_PROPERTIES m_streamProperties;
};
