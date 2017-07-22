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
#include <kodi/addon-instance/AudioDSP.h>
#include <kodi/Filesystem.h>
#include <p8-platform/util/util.h>

// include template configuration header files
#include "configuration/templateConfiguration.h"
#include "adsp.template/include/MACROHelper.h"
#include "include/checkTemplateConfig.h"

// include adsp template specific header files
#include "include/client.h"

#include "AudioDSP/Streams/ADSPStreamManager.hpp"
#include "Addon/Process/AddonProcessManager.hpp"
#include "EnumStrIDs.hpp"
#include "GainMode/GainModeDialog.hpp"
#include "CompressorMode/CompressorModeDialog.hpp"

#include "KodiThreads/system.h"

using namespace std;
using namespace ADDON;

#ifdef TARGET_WINDOWS
#define snprintf _snprintf
#undef CreateDirectory
#endif

int            m_iStreamsPresent  = 0;
bool           m_bCreated         = false;
ADDON_STATUS   m_CurStatus        = ADDON_STATUS_UNKNOWN;

CHelper_libXBMC_addon       *KODI         = NULL;
CHelper_libKODI_guilib      *GUI          = NULL;

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

  void* adspprops = (void*)props;

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

  KODI->Log(LOG_DEBUG, "%s, %i, - Creating Audio DSP template", __FUNCTION__, __LINE__);
  SetKodiThreadsLogger(loc_KodiLoggerCallback);

  m_CurStatus     = ADDON_STATUS_UNKNOWN;
  g_strUserPath   = "";//adspprops->strUserPath;
  g_strAddonPath  = "";//adspprops->strAddonPath;

  ADDON_ReadSettings();

  // create addon user path
  if (!kodi::vfs::DirectoryExists(kodi::GetBaseUserPath()))
  {
    kodi::vfs::CreateDirectory(kodi::GetBaseUserPath());
  }

  m_CurStatus = ADDON_STATUS_OK;
  if (CADSPStreamManager::Create() != AUDIODSP_ADDON_ERROR_NO_ERROR)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
  }

  if (CAddonProcessManager::CreateProcesses() != AUDIODSP_ADDON_ERROR_NO_ERROR)
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

  SAFE_DELETE(GUI);
  SAFE_DELETE(KODI);

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
const char* GetDSPName(void)
{
  return ADSP_ADDON_NAME;
}

const char* GetDSPVersion(void)
{
  return ADSP_ADDON_VERSION;
}

AUDIODSP_ADDON_ERROR CallMenuHook(const AE_DSP_MENUHOOK& Menuhook, const AE_DSP_MENUHOOK_DATA &Item)
{
  // TODO implement an generic interface to open any kind of dialogs
  if (Menuhook.iHookId == CADSPModeIDs::PostProcessingModeGain/* && Item.category == AE_DSP_MENUHOOK_POST_PROCESS*/)
  {
    CGainModeDialog dialog;
    IKodiGUIView *view = dynamic_cast<IKodiGUIView*>(&dialog);
    view->DoModal();
    //view->Destroy();
    return AUDIODSP_ADDON_ERROR_NO_ERROR;
  }

  if (Menuhook.iHookId == CADSPModeIDs::PostProcessingModeCompressor/* && Item.category == AE_DSP_MENUHOOK_POST_PROCESS*/)
  {
    CCompressorModeDialog dialog;
    IKodiGUIView *view = dynamic_cast<IKodiGUIView*>(&dialog);
    view->DoModal();
    //view->Destroy();
    return AUDIODSP_ADDON_ERROR_NO_ERROR;
  }

  KODI->Log(LOG_ERROR, "called unknown menu hook!");
  return AUDIODSP_ADDON_ERROR_FAILED;
}


/*!
 * Control function for start and stop of dsp processing.
 */
AUDIODSP_ADDON_ERROR StreamCreate(const AE_DSP_SETTINGS *AddonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties, ADDON_HANDLE Handle)
{
  return CADSPStreamManager::StreamCreate(AddonSettings, pProperties, Handle);
}

AUDIODSP_ADDON_ERROR StreamDestroy(const ADDON_HANDLE Handle)
{
  return CADSPStreamManager::StreamDestroy(Handle->dataIdentifier);
}

AUDIODSP_ADDON_ERROR StreamInitialize(const ADDON_HANDLE Handle, const AE_DSP_SETTINGS *AddonSettings)
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
}
