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



#include "CompressorModeEnvironment.hpp"

#include "EnumStrIDs.hpp"
#include "CompressorMode/CompressorModeDialogSettings.hpp"

#include "adsp.template/include/client.h"

using namespace ADDON;


const std::string CCompressorModeEnvironmentName::ProcessName = CDispatcherIDs::ToString(CDispatcherIDs::CompressorMode);

#define SET_MODEL_FLOAT_PARAM(ErrCnt, Model, ParameterClass, ParameterID, Data)                                             \
                                          if (Model.SetParameter(ParameterClass::ParameterID, &fVal, sizeof(fVal)) != 0) {  \
                                            ErrCnt++;                                                                       \
                                            KODI->Log(LOG_ERROR, "%s, %i, Failed set parameter %s in %s model", __FUNCTION__, __LINE__, ParameterClass::ToString(ParameterClass::ParameterID), Model.Name.c_str());}\


CCompressorModeEnvironment::CCompressorModeEnvironment() :
  IAddonProcess(CADSPModeIDs::PostProcessingModeCompressor),
  CThread(CDispatcherIDs::ToString(CDispatcherIDs::CompressorModeController))
{
}

CCompressorModeEnvironment::~CCompressorModeEnvironment()
{
}


AE_DSP_ERROR CCompressorModeEnvironment::Create()
{   
  if (m_CompressorModeController.Create() != 0)
  {
    // TODO: log error
    return AE_DSP_ERROR_FAILED;
  }

  if (InitCompressorModel() != 0)
  {
    // TODO: log error
    return AE_DSP_ERROR_FAILED;
  }

  if (!this->ConnectObject(&m_CompressorModeController) || !this->ConnectObject(&m_CompressorModeModel))
  {
    // TODO: log error
    return AE_DSP_ERROR_FAILED;
  }

  CCompressorModeDialogSettings dialogSettings;
  ADSP->AddMenuHook((&dialogSettings));

  CThread::Create();

  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CCompressorModeEnvironment::Destroy()
{
  CThread::StopThread(true);

  m_CompressorModeController.Destroy();
  m_CompressorModeModel.Destroy();

  return AE_DSP_ERROR_NO_ERROR;
}


int CCompressorModeEnvironment::InitCompressorModel()
{
  if (m_CompressorModeModel.Create() != 0)
  {
      KODI->Log(LOG_ERROR, "%s, %i, Failed to create %s model", __FUNCTION__, __LINE__, m_CompressorModeModel.Name.c_str());
      return AE_DSP_ERROR_FAILED;
  }

  return AE_DSP_ERROR_NO_ERROR;;
}

void CCompressorModeEnvironment::Process()
{
  while (!CThread::m_bStop)
  {
    m_CompressorModeController.ProcessMessages();
    m_CompressorModeController.ProcessConnectedMessages();
    m_CompressorModeModel.SaveParameters();
    CThread::Sleep(100);
  }
}
