#include "CpyModesProcess.hpp"

#include "PostProcessGain/GainModeParameterIDs.hpp"
#include "PostProcessGain/GainModeDialogSettings.hpp"

#include "ADSPModeInfos.h"

#include "include/client.h"

using namespace ADDON;


const std::string CCpyModesProcessName::ProcessName = "CCpyModesProcess";

#define SET_MODEL_FLOAT_PARAM(ErrCnt, Model, ParameterClass, ParameterID, Data)                                             \
                                    if (Model.SetParameter(ParameterClass::ParameterID, &fVal, sizeof(fVal)) != 0) {  \
                                      ErrCnt++;                                                                       \
                                      KODI->Log(LOG_ERROR, "%s, %i, Failed set parameter %s in %s model", __FUNCTION__, __LINE__, ParameterClass::Strs[ParameterClass::ParameterID], Model.Name.c_str());}\


CCpyModesProcess::CCpyModesProcess()
{
}

CCpyModesProcess::~CCpyModesProcess()
{
}


AE_DSP_ERROR CCpyModesProcess::Create()
{   
  if (InitPostProcessGainModel() != 0)
  {
    return AE_DSP_ERROR_FAILED;
  }

  CGainModeDialogSettings dialogSettings;
  ADSP->AddMenuHook((&dialogSettings));

  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CCpyModesProcess::Destroy()
{
  m_GainModeModel.Destroy();

  return AE_DSP_ERROR_NO_ERROR;
}


bool CCpyModesProcess::ConnectDispatcher(CMessageDispatcher *Dispatcher)
{
  if(Dispatcher->DispatcherName == CADSPModeInfos::Strs[CADSPModeInfos::ADSP_MODE_ID_PORTPROCESS_GAIN])
  {
    return m_GainModeModel.ConnectDispatcher(Dispatcher);
  }

  return false;
}

bool CCpyModesProcess::DisconnectDispatcher(CMessageDispatcher *Dispatcher)
{
  if (Dispatcher->DispatcherName == CADSPModeInfos::Strs[CADSPModeInfos::ADSP_MODE_ID_PORTPROCESS_GAIN])
  {
    return m_GainModeModel.DisconnectDispatcher(Dispatcher);
  }

  return false;
}


int CCpyModesProcess::InitPostProcessGainModel()
{
  if (m_GainModeModel.Create() != 0)
  {
      KODI->Log(LOG_ERROR, "%s, %i, Failed to create %s model", __FUNCTION__, __LINE__, m_GainModeModel.Name.c_str());
      return AE_DSP_ERROR_FAILED;
  }

  int errorCounter = 0;
  // TODO load settings from XML file

  // no XML file present. Consquently the model will be initialized with default parameters
  float fVal = 0.0f;
  SET_MODEL_FLOAT_PARAM(errorCounter, m_GainModeModel, CGainModeParameterIDs, MainGain, fVal);

  if (errorCounter > 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to set %i parameters in model %s! Please contact the add-on author with this log file.", __FUNCTION__, __LINE__, errorCounter, m_GainModeModel.Name.c_str());
  }

  return errorCounter;
}
