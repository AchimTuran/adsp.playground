#include "GainModeEnvironment.hpp"

#include "EnumStrIDs.hpp"
#include "GainMode/GainModeDialogSettings.hpp"

#include "EnumStrIDs.hpp"

#include "adsp.template/include/client.h"

using namespace ADDON;


const std::string CGainModeEnvironmentName::ProcessName = CDispatcherIDs::ToString(CDispatcherIDs::GainMode);

#define SET_MODEL_FLOAT_PARAM(ErrCnt, Model, ParameterClass, ParameterID, Data)                                             \
                                          if (Model.SetParameter(ParameterClass::ParameterID, &fVal, sizeof(fVal)) != 0) {  \
                                            ErrCnt++;                                                                       \
                                            KODI->Log(LOG_ERROR, "%s, %i, Failed set parameter %s in %s model", __FUNCTION__, __LINE__, ParameterClass::ToString(ParameterClass::ParameterID), Model.Name.c_str());}\


CGainModeEnvironment::CGainModeEnvironment() :
  IAddonProcess(CADSPModeIDs::PostProcessingModeGain),
  CThread(CDispatcherIDs::ToString(CDispatcherIDs::GainModeController))
{
}

CGainModeEnvironment::~CGainModeEnvironment()
{
}


AE_DSP_ERROR CGainModeEnvironment::Create()
{   
  if (m_GainModeController.Create() != 0)
  {
    // TODO: log error
    return AE_DSP_ERROR_FAILED;
  }

  if (InitGainModel() != 0)
  {
    // TODO: log error
    return AE_DSP_ERROR_FAILED;
  }

  if (!this->ConnectObject(&m_GainModeController) || !this->ConnectObject(&m_GainModeModel))
  {
    // TODO: log error
    return AE_DSP_ERROR_FAILED;
  }

  CGainModeDialogSettings dialogSettings;
  ADSP->AddMenuHook((&dialogSettings));

  CThread::Create();

  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CGainModeEnvironment::Destroy()
{
  CThread::StopThread(true);
    
  m_GainModeController.Destroy();
  m_GainModeModel.Destroy();

  return AE_DSP_ERROR_NO_ERROR;
}


int CGainModeEnvironment::InitGainModel()
{
  if (m_GainModeModel.Create() != 0)
  {
      KODI->Log(LOG_ERROR, "%s, %i, Failed to create %s model", __FUNCTION__, __LINE__, m_GainModeModel.Name.c_str());
      return AE_DSP_ERROR_FAILED;
  }
  int errorCounter = 0;
  // Consquently the model will be initialized with default parameters
  float fVal = 0.0f;
  SET_MODEL_FLOAT_PARAM(errorCounter, m_GainModeModel, CSocketGainModeIDs, UpdateMainGain, fVal);

  if (errorCounter > 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to set %i parameters in model %s! Please contact the add-on author with this log file.", __FUNCTION__, __LINE__, errorCounter, m_GainModeModel.Name.c_str());
  }

  return errorCounter;
}

void CGainModeEnvironment::Process()
{
  while (!CThread::m_bStop)
  {
    m_GainModeController.ProcessMessages();
    m_GainModeController.ProcessConnectedMessages();
    CThread::Sleep(100);
  }
}
