/*
 *      Copyright (C) 2005-2014 Team XBMC
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <vector>
#include <string>

// include kodi platform header files
#include <kodi/kodi_adsp_dll.h>
#include <p8-platform/util/util.h>

// include template configuration header files
#include "configuration/templateConfiguration.h"
#include "adsp.template/include/MACROHelper.h"
#include "include/checkTemplateConfig.h"

// include adsp template specific header files
#include "include/client.h"

#include "AudioDSP/Streams/ADSPStreamManager.hpp"
#include "Addon/Process/AddonProcessManager.hpp"

#include "KodiThreads/system.h"

using namespace std;
using namespace ADDON;

#ifdef TARGET_WINDOWS
#define snprintf _snprintf
#endif

int            m_iStreamsPresent  = 0;
bool           m_bCreated         = false;
ADDON_STATUS   m_CurStatus        = ADDON_STATUS_UNKNOWN;

/* User adjustable settings are saved here.
 * Default values are defined inside client.h
 * and exported to the other source files.
 */
std::string   g_strUserPath       = "";
std::string   g_strAddonPath      = "";


CHelper_libXBMC_addon       *KODI         = NULL;
CHelper_libKODI_adsp        *ADSP         = NULL;
CHelper_libKODI_guilib      *GUI          = NULL;
CHelper_libKODI_audioengine *AUDIOENGINE  = NULL;

static void loc_KodiLoggerCallback(int LogLevel, const char *format, ...)
{
}

extern "C" {

void ADDON_ReadSettings(void)
{
}

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!hdl || !props)
  {
    return ADDON_STATUS_UNKNOWN;
  }

  AE_DSP_PROPERTIES* adspprops = (AE_DSP_PROPERTIES*)props;

  KODI = new CHelper_libXBMC_addon;
  if (!KODI->RegisterMe(hdl))
  {
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  GUI = new CHelper_libKODI_guilib;
  if (!GUI->RegisterMe(hdl))
  {
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  ADSP = new CHelper_libKODI_adsp;
  if (!ADSP->RegisterMe(hdl))
  {
    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  
  AUDIOENGINE = new CHelper_libKODI_audioengine;
  if (!AUDIOENGINE->RegisterMe(hdl))
  {
    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    SAFE_DELETE(AUDIOENGINE);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  KODI->Log(LOG_DEBUG, "%s, %i, - Creating Audio DSP template", __FUNCTION__, __LINE__);
  SetKodiThreadsLogger(loc_KodiLoggerCallback);

  m_CurStatus     = ADDON_STATUS_UNKNOWN;
  g_strUserPath   = adspprops->strUserPath;
  g_strAddonPath  = adspprops->strAddonPath;

  ADDON_ReadSettings();

  m_CurStatus = ADDON_STATUS_OK;
  if (CADSPStreamManager::Create() != AE_DSP_ERROR_NO_ERROR)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
  }

  if (CAddonProcessManager::CreateProcesses() != AE_DSP_ERROR_NO_ERROR)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to create CAddonProcessManager!", __FUNCTION__, __LINE__);
  }

  m_bCreated = true;
  m_iStreamsPresent = 0;
  return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  m_bCreated = false;
  m_iStreamsPresent = 0;

  CAddonProcessManager::DestroyProcesses();
  CADSPStreamManager::Destroy();

  ReleaseKodiThreadsLogger();

  SAFE_DELETE(ADSP);
  SAFE_DELETE(GUI);
  SAFE_DELETE(KODI);
  SAFE_DELETE(AUDIOENGINE);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

bool ADDON_HasSettings()
{
#ifdef ADSP_ADDON_USE_SETTINGS
  return true;
#else
  return false;
#endif
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  (void) sSet; // Remove compiler warning
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *SettingName, const void *SettingValue)
{
  string settingName(SettingName);
  if(!SettingValue)
  {
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  return ADDON_STATUS_OK;
}

void ADDON_Stop()
{
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char *Flag, const char *Sender, const char *Message, const void *Data)
{
  // Remove compiler warnings
  (void) Flag;
  (void) Sender; 
  (void) Message; 
  (void) Data;
}


/***********************************************************
 * Audio DSP Client AddOn specific public library functions
 ***********************************************************/
const char* GetAudioDSPAPIVersion(void)
{
  return KODI_AE_DSP_API_VERSION;
}

const char* GetMinimumAudioDSPAPIVersion(void)
{
  return KODI_AE_DSP_MIN_API_VERSION;
}

const char* GetGUIAPIVersion(void)
{
  return KODI_GUILIB_API_VERSION;
}

const char* GetMinimumGUIAPIVersion(void)
{
  return KODI_GUILIB_MIN_API_VERSION;
}

AE_DSP_ERROR GetAddonCapabilities(AE_DSP_ADDON_CAPABILITIES* pCapabilities)
{
  if(pCapabilities == NULL)
  {
    return AE_DSP_ERROR_FAILED;
  }

  pCapabilities->bSupportsInputProcess    = CADSPStreamManager::SupportsInputProcess();
  pCapabilities->bSupportsPreProcess      = CADSPStreamManager::SupportsPreProcess();
  pCapabilities->bSupportsMasterProcess   = CADSPStreamManager::SupportsMasterProcess();
  pCapabilities->bSupportsPostProcess     = CADSPStreamManager::SupportsPostProcess();
  pCapabilities->bSupportsInputResample   = CADSPStreamManager::SupportsInputResample();
  pCapabilities->bSupportsOutputResample  = CADSPStreamManager::SupportsOutputResample();

  return AE_DSP_ERROR_NO_ERROR;
}

const char* GetDSPName(void)
{
  return ADSP_ADDON_NAME;
}

const char* GetDSPVersion(void)
{
  return ADSP_ADDON_VERSION;
}

// TODO move to top
#include "EnumStrIDs.hpp"
#include "GainMode/GainModeDialog.hpp"

AE_DSP_ERROR CallMenuHook(const AE_DSP_MENUHOOK& Menuhook, const AE_DSP_MENUHOOK_DATA &Item)
{
  if (Menuhook.iHookId == CADSPModeIDs::PostProcessingModeGain/* && Item.category == AE_DSP_MENUHOOK_POST_PROCESS*/)
  {
    CGainModeDialog dialog;
    IKodiGUIView *view = dynamic_cast<IKodiGUIView*>(&dialog);
    view->DoModal();
    //view->Destroy();
    return AE_DSP_ERROR_NO_ERROR;
  }

  KODI->Log(LOG_ERROR, "called unknown menu hook!");
  return AE_DSP_ERROR_FAILED;
}


/*!
 * Control function for start and stop of dsp processing.
 */
AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *AddonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties, ADDON_HANDLE Handle)
{
  return CADSPStreamManager::StreamCreate(AddonSettings, pProperties, Handle);
}

AE_DSP_ERROR StreamDestroy(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::StreamDestroy(Handle->dataIdentifier);
}

AE_DSP_ERROR StreamInitialize(const ADDON_HANDLE Handle, const AE_DSP_SETTINGS *AddonSettings)
{
  return CADSPStreamManager::StreamInitialize(Handle, AddonSettings);
}


/*!
 * Pre processing related functions
 */
unsigned int PreProcessNeededSamplesize(const ADDON_HANDLE Handle, unsigned int Mode_id)
{
  return CADSPStreamManager::NeededSamplesize(Handle, Mode_id);
}

float PreProcessGetDelay(const ADDON_HANDLE Handle, unsigned int Mode_id)
{
  return CADSPStreamManager::GetDelay(Handle, Mode_id);
}

/*!
 * Resampling related functions before master processing.
 * only one dsp addon is allowed to do this
 */
unsigned int InputResampleProcessNeededSamplesize(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::NeededSamplesize(Handle, AE_DSP_MODE_TYPE_INPUT_RESAMPLE);
}

int InputResampleSampleRate(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::ResamplingRate(Handle, AE_DSP_MODE_TYPE_INPUT_RESAMPLE);
}
  
float InputResampleGetDelay(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::GetDelay(Handle, AE_DSP_MODE_TYPE_INPUT_RESAMPLE);
}

/*!
 * Master processing functions
 * only one during playback selectable dsp addon is allowed to do this
 */
AE_DSP_ERROR MasterProcessSetMode(const ADDON_HANDLE Handle, AE_DSP_STREAMTYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
  return CADSPStreamManager::MasterProcessSetMode(Handle, Type, Mode_id, Unique_db_mode_id);
}

unsigned int MasterProcessNeededSamplesize(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::NeededSamplesize(Handle, AE_DSP_MODE_TYPE_MASTER_PROCESS);
}

float MasterProcessGetDelay(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::GetDelay(Handle, AE_DSP_MODE_TYPE_MASTER_PROCESS);
}

int MasterProcessGetOutChannels(const ADDON_HANDLE Handle, unsigned long &Out_channel_present_flags)
{
  return CADSPStreamManager::MasterProcessGetOutChannels(Handle, Out_channel_present_flags);
}

const char *MasterProcessGetStreamInfoString(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::MasterProcessGetStreamInfoString(Handle);
}


/*!
 * Post processing related functions
 * all enabled addons allowed todo this
 */
unsigned int PostProcessNeededSamplesize(const ADDON_HANDLE Handle, unsigned int Mode_id)
{
  return CADSPStreamManager::NeededSamplesize(Handle, Mode_id);
}

float PostProcessGetDelay(const ADDON_HANDLE Handle, unsigned int Mode_id)
{
  return CADSPStreamManager::GetDelay(Handle, Mode_id);
}

/*!
 * Resampling related functions after final processing.
 * only one dsp addon is allowed to do this
 */
unsigned int OutputResampleProcessNeededSamplesize(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::NeededSamplesize(Handle, AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE);
}

int OutputResampleSampleRate(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::ResamplingRate(Handle, AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE);
}

float OutputResampleGetDelay(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::GetDelay(Handle, AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE);
}

/*!
 *  Addon Processing functions
 */
bool InputProcess(const ADDON_HANDLE Handle, const float **Array_in, unsigned int Samples)
{
  return CADSPStreamManager::ProcessMode(Handle, AE_DSP_MODE_TYPE_MAX, Array_in, Samples) > 0;
}

unsigned int InputResampleProcess(const ADDON_HANDLE Handle, float **Array_in, float **Array_out, unsigned int Samples)
{
  return CADSPStreamManager::ProcessMode(Handle, AE_DSP_MODE_TYPE_INPUT_RESAMPLE, Array_in, Array_out, Samples);
}

unsigned int PreProcess(const ADDON_HANDLE Handle, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
  return CADSPStreamManager::ProcessMode(Handle, Mode_id, Array_in, Array_out, Samples);
}

unsigned int MasterProcess(const ADDON_HANDLE Handle, float **Array_in, float **Array_out, unsigned int Samples)
{
  return CADSPStreamManager::ProcessMode(Handle, AE_DSP_MODE_TYPE_MASTER_PROCESS, Array_in, Array_out, Samples);
}

unsigned int PostProcess(const ADDON_HANDLE Handle, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
  return CADSPStreamManager::ProcessMode(Handle, Mode_id, Array_in, Array_out, Samples);
}

unsigned int OutputResampleProcess(const ADDON_HANDLE Handle, float **Array_in, float **Array_out, unsigned int Samples)
{
  return CADSPStreamManager::ProcessMode(Handle, AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE, Array_in, Array_out, Samples);
}

AE_DSP_ERROR StreamIsModeSupported(const ADDON_HANDLE Handle, AE_DSP_MODE_TYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
  return CADSPStreamManager::StreamIsModeSupported(Handle, Type, Mode_id, Unique_db_mode_id);
}
}
