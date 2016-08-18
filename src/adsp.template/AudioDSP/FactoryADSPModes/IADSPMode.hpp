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


#include "kodi/kodi_adsp_types.h"

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
  AE_DSP_ERROR Create(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties)
  {
    memcpy((void*)&m_StreamSettings, Settings, sizeof(AE_DSP_SETTINGS));
    memcpy((void*)&m_StreamProperties, pProperties, sizeof(AE_DSP_STREAM_PROPERTIES));

    return ModeCreate(m_StreamSettings, m_StreamProperties);
  }

  AE_DSP_ERROR Initialize(const AE_DSP_SETTINGS *Settings)
  {
    if (!CADSPHelpers::CmpStreamSettings(*Settings, m_StreamSettings))
    {
      return AE_DSP_ERROR_NO_ERROR;
    }

    // If stream settings have changed then copy them and recreate AudioDSP processing mode
    memcpy((void*)&m_StreamSettings, Settings, sizeof(AE_DSP_SETTINGS));

    return ModeCreate(m_StreamSettings, m_StreamProperties);
  }

  //!  This gets the current stream settings and properties. 
  /*!
  * Get stream settings and properties. For details see  and AE_DSP_STREAM_PROPERTIES structures.
  * If the add-on operate with buffered arrays and the output size can be higher as
  * the input it becomes asked about needed size before any PostProcess call.
  * @param pSettings Stream settings for details see AE_DSP_SETTINGS.
  * @param pProperties Stream properties for details see AE_DSP_STREAM_PROPERTIES.
  * @return AE_DSP_ERROR_INVALID_PARAMETERS: if your input parameters were invalid.
  * AE_DSP_ERROR_NO_ERROR: if all was ok.
  */
  AE_DSP_ERROR GetStreamInfos(const AE_DSP_SETTINGS& Settings, const AE_DSP_STREAM_PROPERTIES& Properties, void *CustomStreamInfos = NULL)
  {
    memcpy((void*)&Settings, &m_StreamSettings, sizeof(AE_DSP_SETTINGS));
    memcpy((void*)&Properties, &m_StreamProperties, sizeof(AE_DSP_STREAM_PROPERTIES));

    //if (CustomStreamInfos)
    //{
    //  return GetCustomStreamInfos(CustomStreamInfos);
    //}

    return AE_DSP_ERROR_NO_ERROR;
  }


// Requiered Create/Destroy Methods
  virtual AE_DSP_ERROR ModeCreate(const AE_DSP_SETTINGS &Settings, const AE_DSP_STREAM_PROPERTIES &Properties) = 0;
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
    return (float)m_StreamSettings.iInFrames / (float)m_StreamSettings.iInSamplerate;
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
  * @return AE_DSP_ERROR_NO_ERROR if the properties were fetched successfully.
  * If the stream is not supported the ADSP addon must return AE_DSP_ERROR_IGNORE_ME.
  * @remarks By default this method accept all processing types.
  * If you wanna filter processing mode types then you have to overload this method in
  * your processing class.
  */
  virtual AE_DSP_ERROR StreamIsModeSupported(AE_DSP_MODE_TYPE Type, unsigned int ModeID, int UniqueDBModeID)
  {
    UniqueDBModeID; // prevent compiler warnings, because adsp.template doesn't use this information

    if (Type != m_ModeType || ModeID != m_ModeID)
    {
      return AE_DSP_ERROR_IGNORE_ME;
    }

    return AE_DSP_ERROR_NO_ERROR;
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
    return m_StreamSettings.iProcessSamplerate;
  }

protected:
  unsigned int      m_ModeID;
  AE_DSP_MODE_TYPE  m_ModeType;
  ////! ToDo: description.
  ///*!
  //* Returns ToDo!
  //* @return ToDo!
  //* @remarks ToDo!
  //*/
  //virtual AE_DSP_ERROR GetCustomStreamInfos(void *CustomStreamSettings)
  //{
  //  return AE_DSP_ERROR_NO_ERROR;
  //}

private:
  void ResetStreamSettings()
  {
    memset(&m_StreamSettings, 0, sizeof(AE_DSP_SETTINGS));
    memset(&m_StreamProperties, 0, sizeof(AE_DSP_STREAM_PROPERTIES));
  }

  //! Used stream settings for details see AE_DSP_SETTINGS.
  AE_DSP_SETTINGS m_StreamSettings;
  //! Used stream properties for details see AE_DSP_STREAM_PROPERTIES.
  AE_DSP_STREAM_PROPERTIES m_StreamProperties;
};
