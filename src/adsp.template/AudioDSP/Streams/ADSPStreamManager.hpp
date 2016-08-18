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
#include "KodiThreads/threads/SingleLock.h"

#include "AudioDSP/Streams/ADSPStream.hpp"
#include "AudioDSP/Streams/IADSPStreamBuilder.hpp"
#include "AudioDSP/Streams/ADSPStreamBuilderAll.hpp"

class CADSPStreamManager
{
public:
  static AE_DSP_ERROR Create();
  static void Destroy();

// stream methods
  static AE_DSP_ERROR StreamIsModeSupported(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType, unsigned int ModeID, int UniqueDBModeID);

  static unsigned int ProcessMode(const ADDON_HANDLE Handle, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples);
  static unsigned int ProcessMode(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType, float **Array_in, float **Array_out, unsigned int Samples);
  static unsigned int ProcessMode(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType, const float **Array_in, unsigned int Samples);

  static unsigned int NeededSamplesize(const ADDON_HANDLE Handle, unsigned int ModeID);
  static unsigned int NeededSamplesize(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType);
  static float GetDelay(const ADDON_HANDLE Handle, unsigned int ModeID);
  static float GetDelay(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType);

// Optional Methods for Master Modes
  static int MasterProcessGetOutChannels(const ADDON_HANDLE Handle, unsigned long &OutChannelFlags);
  static const char* MasterProcessGetStreamInfoString(const ADDON_HANDLE Handle);
  static AE_DSP_ERROR MasterProcessSetMode(const ADDON_HANDLE Handle, AE_DSP_STREAMTYPE StreamType, unsigned int ModeID, int UniqueDBModeID);

// Optional Resampling Methods
  static int ResamplingRate(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE ModeType);


  /*!
  * Control function for start and stop of dsp processing.
  */
  static AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties, const ADDON_HANDLE handle);
  static AE_DSP_ERROR StreamInitialize(const ADDON_HANDLE Handle, const AE_DSP_SETTINGS *AddonSettings);
  static AE_DSP_ERROR StreamDestroy(unsigned int Id);

  ///*!
  //* initialize or destroy methods for the AddonHandler
  //*/
  //void Destroy();
  //bool Init();

  /*!
  * Supported processing methods
  */
  static bool SupportsInputProcess();
  static bool SupportsPreProcess();
  static bool SupportsMasterProcess();
  static bool SupportsPostProcess();
  static bool SupportsInputResample();
  static bool SupportsOutputResample();

  ///*!
  //* Get Stream
  //*/
  //CADSPProcessorHandle *GetStream(AE_DSP_STREAM_ID Id);

  //!  This gets the current stream Id settings and properties. 
  /*!
  * Get stream settings and properties. For details see  and AE_DSP_STREAM_PROPERTIES structures.
  * If the add-on operate with buffered arrays and the output size can be higher as
  * the input it becomes asked about needed size before any PostProcess call.
  * @param Id The requested stream id. Never use a Id equal or greater than AE_DSP_STREAM_MAX_STREAMS.
  * @param pSettings Stream settings for details see AE_DSP_SETTINGS.
  * @param pProperties Stream properties for details see AE_DSP_STREAM_PROPERTIES.
  * @return AE_DSP_ERROR_INVALID_PARAMETERS: if your input parameters were invalid.
  * AE_DSP_ERROR_REJECTED: when the requested Stream Id is not active.
  * AE_DSP_ERROR_NO_ERROR: if all was ok.
  */
  //AE_DSP_ERROR GetStreamInfos(AE_DSP_STREAM_ID Id, const AE_DSP_SETTINGS *pSettings, const AE_DSP_STREAM_PROPERTIES* pProperties, void *CustomStreamInfos = NULL);

  //AE_DSP_ERROR SendMessageToStream(/*CADSPModeMessage &Message*/);


private:
  static int m_HasProcesses;

  /*!
  * Pointer array for active dsp processing classes, for this reason the
  * stream id is never more as AE_DSP_STREAM_MAX_STREAMS and can be used as pointer to this array.
  */
  static CADSPStream             *m_ADSPStreams[AE_DSP_STREAM_MAX_STREAMS];
  static CCriticalSection        m_Lock; // Lock for access to m_ADSPStreams
  static IADSPStreamBuilder      *m_ADSPStreamBuilder;
  static CADSPStreamBuilderAll   m_ADSPStreamBuilderAll;
};
