#include "CompressorModeEnvironment.hpp"

#include "EnumStrIDs.hpp"
#include "CompressorMode/CompressorModeDialogSettings.hpp"

#include "EnumStrIDs.hpp"

#include "include/client.h"

using namespace ADDON;


const std::string CCompressorModeEnvironmentName::ProcessName = CDispatcherIDs::ToString(CDispatcherIDs::CompressorMode);

#define SET_MODEL_FLOAT_PARAM(ErrCnt, Model, ParameterClass, ParameterID, Data)                                             \
                                          if (Model.SetParameter(ParameterClass::ParameterID, &fVal, sizeof(fVal)) != 0) {  \
                                            ErrCnt++;                                                                       \
                                            KODI->Log(LOG_ERROR, "%s, %i, Failed set parameter %s in %s model", __FUNCTION__, __LINE__, ParameterClass::ToString(ParameterClass::ParameterID), Model.Name.c_str());}\


CCompressorModeEnvironment::CCompressorModeEnvironment() :
  IAddonProcess(CADSPModeIDs::PostProcessingModeCompressor),
  m_Thread(this, CDispatcherIDs::ToString(CDispatcherIDs::CompressorModeController))
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

  m_Thread.Create();
  while (!m_Thread.IsRunning());

  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CCompressorModeEnvironment::Destroy()
{
  m_Thread.StopThread(true);

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
  int errorCounter = 0;
  // Consquently the model will be initialized with default parameters
  float fVal = 0.0f;
  //SET_MODEL_FLOAT_PARAM(errorCounter, m_CompressorModeModel, CSocketCompressorModeIDs, UpdateMainCompressor, fVal);

  if (errorCounter > 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to set %i parameters in model %s! Please contact the add-on author with this log file.", __FUNCTION__, __LINE__, errorCounter, m_CompressorModeModel.Name.c_str());
  }

  return errorCounter;
}

void CCompressorModeEnvironment::Run()
{
  while (m_Thread.IsRunning())
  {
    m_CompressorModeController.ProcessMessages();
    m_CompressorModeModel.ProcessMessages();
    m_CompressorModeModel.SaveParameters();
    m_Thread.Sleep(10);
  }
}
