#include "GainModeEnvironment.hpp"

#include "EnumStrIDs.hpp"
#include "GainMode/GainModeDialogSettings.hpp"

#include "EnumStrIDs.hpp"

#include "adsp.template/include/client.h"

using namespace ADDON;


const std::string CGainModeEnvironmentName::ProcessName = CDispatcherIDs::ToString(CDispatcherIDs::GainMode);

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

  if (m_GainModeModel.Create() != 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to create %s model", __FUNCTION__, __LINE__, m_GainModeModel.Name.c_str());
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

void CGainModeEnvironment::Process()
{
  while (!CThread::m_bStop)
  {
    m_GainModeController.ProcessMessages();
    m_GainModeController.ProcessConnectedMessages();
    CThread::Sleep(100);
  }
}
