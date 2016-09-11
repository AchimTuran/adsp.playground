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



#include "CompressorMode/CompressorModeDialogSettings.hpp"
#include "CompressorMode/CompressorModeDialog.hpp"
#include "CompressorModeEnvironment.hpp"
#include "CompressorMode/CompressorModeDefaults.h"
#include "EnumStrIDs.hpp"

#include "adsp.template/Addon/Process/AddonProcessManager.hpp"
#include "adsp.template/utils/stdStringUtils.h"

#include "asplib/Dynamics/asplib_DynamicsOptions.hpp"

#include <math.h>


using namespace asplib;
using namespace ADDON;


#define BUTTON_OK                 10050
#define BUTTON_CANCEL             10051
#define BUTTON_DEFAULT            10052

#define SPIN_GainCurve                  8001
#define SLIDER_TauRelease               8002
#define SLIDER_TauAttack                8003
#define SLIDER_Threshold                8004
#define SLIDER_CompressionRatio         8005
#define SLIDER_KneeWidth                8006

#define MAIN_Compressor_PAGE_STEP       1.0f


CCompressorModeDialog::CCompressorModeDialog() :
  IKodiGUIView("DialogCompressorMode.xml", false, true, 
               CDispatcherIDs::ToString(CDispatcherIDs::CompressorModeDialog), 
               CDispatcherIDs::CompressorModeDialog,
               CADSPModeIDs::PostProcessingModeCompressor)
{
  m_PageActionValue  = 0.0f;
  m_GainCurve        = 0;
  m_TauAttack        = 0.0f;
  m_Threshold        = 0.0f;
  m_KneeWidth        = 0.0f;
  m_TauRelease       = 0.0f;
  m_CompressionRatio = 0.0f;
}


bool CCompressorModeDialog::OnInit()
{
  m_SpinGainCurve          = GUI->Control_getSpin(this->m_window, SPIN_GainCurve);
  m_SliderTauAttack        = GUI->Control_getSettingsSlider(this->m_window, SLIDER_TauAttack);
  m_SliderThreshold        = GUI->Control_getSettingsSlider(this->m_window, SLIDER_Threshold);
  m_SliderKneeWidth        = GUI->Control_getSettingsSlider(this->m_window, SLIDER_KneeWidth);
  m_SliderTauRelease       = GUI->Control_getSettingsSlider(this->m_window, SLIDER_TauRelease);
  m_SliderCompressionRatio = GUI->Control_getSettingsSlider(this->m_window, SLIDER_CompressionRatio);


  if (!m_SpinGainCurve || !m_SliderTauAttack || !m_SliderThreshold || !m_SliderKneeWidth || !m_SliderTauRelease || !m_SliderCompressionRatio)
  {
    KODI->Log(LOG_ERROR, "%s, %i, One ore more controls not found!", __FUNCTION__, __LINE__);
    return false;
  }
  m_SpinGainCurve->SetValue(m_GainCurve);
  m_SpinGainCurve->SetVisible(true);
  m_SpinGainCurve->AddLabel(KODI->GetLocalizedString(32007), CompressorOptions::COMPRESSION_HARD_CLIPPING);

  m_SliderTauAttack->SetFloatRange(COMPRESSOR_TAU_ATTACK_MIN, COMPRESSOR_TAU_ATTACK_MAX);
  m_SliderTauAttack->SetFloatValue(m_TauAttack);
  m_SliderTauAttack->SetVisible(true);
  m_SliderTauRelease->SetFloatRange(COMPRESSOR_TAU_RELEASE_MIN, COMPRESSOR_TAU_RELEASE_MAX);
  m_SliderTauRelease->SetFloatValue(m_CompressionRatio);
  m_SliderTauRelease->SetVisible(true);
  m_SliderThreshold->SetFloatRange(COMPRESSOR_THRESHOLD_MIN, COMPRESSOR_THRESHOLD_MAX);
  m_SliderThreshold->SetFloatValue(m_Threshold);
  m_SliderThreshold->SetVisible(true);
  m_SliderKneeWidth->SetFloatRange(COMPRESSOR_KNEE_WIDTH_MIN, COMPRESSOR_KNEE_WIDTH_MAX);
  m_SliderKneeWidth->SetFloatValue(m_KneeWidth);
  m_SliderKneeWidth->SetVisible(true);
  m_SliderCompressionRatio->SetFloatRange(COMPRESSOR_COMPRESSION_RATIO_MIN, COMPRESSOR_COMPRESSION_RATIO_MAX);
  m_SliderCompressionRatio->SetFloatValue(m_CompressionRatio);
  m_SliderCompressionRatio->SetVisible(true);

  if (!CCompressorModeDialogMessages::Create(this))
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to connect sockets from %s", __FUNCTION__, __LINE__, this->Name.c_str());
    return false;
  }

  if(CAddonProcessManager::ConnectObject(this) != 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed to connect message dispachter %s", __FUNCTION__, __LINE__, this->Name.c_str());
    return false;
  }

  if (!this->SendMsg(nullptr, 0, CSocketCompressorModeIDs::RequestModelState))
  {
    KODI->Log(LOG_ERROR, "%s, %i, Failed send \"%s\" from dispatcher %s to compressor controller", __FUNCTION__, __LINE__, CSocketCompressorModeIDs::ToString(CSocketCompressorModeIDs::RequestModelState), this->Name.c_str());
  }

  return true;
}

bool CCompressorModeDialog::OnClick(int controlId)
{
  switch (controlId)
  {
    case BUTTON_OK:
    {
      this->Close();
    }
    break;

    case BUTTON_CANCEL:
    {
      this->Close();
    }
    break;

    case BUTTON_DEFAULT:          ProcessDefaultButton();           break;
    case SPIN_GainCurve:          ProcessSpinGainCurve();           break;
    case SLIDER_TauAttack:        ProcessSliderTauAttack();         break;
    case SLIDER_KneeWidth:        ProcessSliderKneeWidth();         break;
    case SLIDER_Threshold:        ProcessSliderThreshold();         break;
    case SLIDER_TauRelease:       ProcessSliderTauRelease();        break;
    case SLIDER_CompressionRatio: ProcessSliderCompressionRatio();  break;
  }

  if(m_PageActionValue != 0.0f)
  {
    m_PageActionValue = 0.0f;
  }

  return true;
}

bool CCompressorModeDialog::OnFocus(int controlId)
{
  return false;
}

bool CCompressorModeDialog::OnAction(int actionId)
{
  if (actionId == ADDON_ACTION_CLOSE_DIALOG ||
      actionId == ADDON_ACTION_PREVIOUS_MENU ||
      actionId == ADDON_ACTION_NAV_BACK)
  {
    return OnClick(BUTTON_CANCEL);
  }

  if (actionId == ADDON_ACTION_PAGE_DOWN)
  {
    m_PageActionValue = -MAIN_Compressor_PAGE_STEP;
    return OnClick(m_window->GetFocusId());
  }

  if (actionId == ADDON_ACTION_PAGE_UP)
  {
    m_PageActionValue = MAIN_Compressor_PAGE_STEP;
    return OnClick(m_window->GetFocusId());
  }

  return false;
}

void CCompressorModeDialog::OnClose()
{
  CAddonProcessManager::DisconnectObject(this);
  GUI->Control_releaseSpin(m_SpinGainCurve);
  GUI->Control_releaseSettingsSlider(m_SliderTauRelease);
  GUI->Control_releaseSettingsSlider(m_SliderTauAttack);
  GUI->Control_releaseSettingsSlider(m_SliderThreshold);
  GUI->Control_releaseSettingsSlider(m_SliderCompressionRatio);
  GUI->Control_releaseSettingsSlider(m_SliderKneeWidth);
}

void CCompressorModeDialog::ProcessDefaultButton()
{
  float fVal = COMPRESSOR_TAU_ATTACK_DEFAULT;
  this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateTauAttack);
  fVal = COMPRESSOR_TAU_RELEASE_DEFAULT;
  this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateTauRelease);
  fVal = COMPRESSOR_KNEE_WIDTH_DEFAULT;
  this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateKneeWidth);
  fVal = COMPRESSOR_THRESHOLD_DEFAULT;
  this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateThreshold);
  fVal = COMPRESSOR_COMPRESSION_RATIO_DEFAULT;
  this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateCompressionRatio);
}

void CCompressorModeDialog::ProcessSpinGainCurve()
{
  // TODO!
  //if (m_PageActionValue != 0.0f)
  //{
  //  m_ += m_PageActionValue;
  //  m_MainCompressorSlider->SetFloatValue(m_MainCompressor);
  //  m_MainCompressor = m_MainCompressorSlider->GetFloatValue();
  //}
  //else
  //{
  //  m_MainCompressor = m_MainCompressorSlider->GetFloatValue();
  //}

  ////this->SendMsg(static_cast<void*>(&m_MainCompressor), sizeof(float), CSocketCompressorModeIDs::UpdateMainCompressor);
}

void CCompressorModeDialog::ProcessSliderTauAttack()
{
  if (m_PageActionValue != 0.0f)
  {
    m_TauAttack += m_PageActionValue;
    m_SliderTauAttack->SetFloatValue(m_TauAttack);
    m_TauAttack = m_SliderTauAttack->GetFloatValue();
  }
  else
  {
    m_TauAttack = m_SliderTauAttack->GetFloatValue();
  }

  this->SendMsg(static_cast<void*>(&m_TauAttack), sizeof(float), CSocketCompressorModeIDs::UpdateTauAttack);
}

void CCompressorModeDialog::ProcessSliderKneeWidth()
{
  if (m_PageActionValue != 0.0f)
  {
    m_KneeWidth += m_PageActionValue;
    m_SliderKneeWidth->SetFloatValue(m_KneeWidth);
    m_KneeWidth = m_SliderKneeWidth->GetFloatValue();
  }
  else
  {
    m_KneeWidth = m_SliderKneeWidth->GetFloatValue();
  }

  this->SendMsg(static_cast<void*>(&m_KneeWidth), sizeof(float), CSocketCompressorModeIDs::UpdateKneeWidth);
}

void CCompressorModeDialog::ProcessSliderThreshold()
{
  if (m_PageActionValue != 0.0f)
  {
    m_Threshold += m_PageActionValue;
    m_SliderThreshold->SetFloatValue(m_Threshold);
    m_Threshold = m_SliderThreshold->GetFloatValue();
  }
  else
  {
    m_Threshold = m_SliderThreshold->GetFloatValue();
  }

  this->SendMsg(static_cast<void*>(&m_Threshold), sizeof(float), CSocketCompressorModeIDs::UpdateThreshold);
}

void CCompressorModeDialog::ProcessSliderTauRelease()
{
  if (m_PageActionValue != 0.0f)
  {
    m_TauRelease += m_PageActionValue;
    m_SliderTauRelease->SetFloatValue(m_TauRelease);
    m_TauRelease = m_SliderTauRelease->GetFloatValue();
  }
  else
  {
    m_TauRelease = m_SliderTauRelease->GetFloatValue();
  }

  this->SendMsg(static_cast<void*>(&m_TauRelease), sizeof(float), CSocketCompressorModeIDs::UpdateTauRelease);
}

void CCompressorModeDialog::ProcessSliderCompressionRatio()
{
  if (m_PageActionValue != 0.0f)
  {
    m_CompressionRatio += m_PageActionValue;
    m_SliderCompressionRatio->SetFloatValue(m_CompressionRatio);
    m_CompressionRatio = m_SliderCompressionRatio->GetFloatValue();
  }
  else
  {
    m_CompressionRatio = m_SliderCompressionRatio->GetFloatValue();
  }

  this->SendMsg(static_cast<void*>(&m_CompressionRatio), sizeof(float), CSocketCompressorModeIDs::UpdateCompressionRatio);
}

// private MC callback methods
int CCompressorModeDialog::UpdateTauRelease(Message &Msg)
{
  m_TauRelease = *(float*)(Msg.data);
  m_SliderTauRelease->SetFloatValue(m_TauRelease);

  return 0;
}

int CCompressorModeDialog::UpdateTauAttack(Message &Msg)
{
  m_TauAttack = *(float*)(Msg.data);
  m_SliderTauAttack->SetFloatValue(m_TauAttack);

  return 0;
}

int CCompressorModeDialog::UpdateThreshold(Message &Msg)
{
  m_Threshold = *(float*)(Msg.data);
  m_SliderThreshold->SetFloatValue(m_Threshold);

  return 0;
}

int CCompressorModeDialog::UpdateCompressionRation(Message &Msg)
{
  m_CompressionRatio = *(float*)(Msg.data);
  m_SliderCompressionRatio->SetFloatValue(m_CompressionRatio);

  return 0;
}

int CCompressorModeDialog::UpdateKneeWidth(Message &Msg)
{
  m_KneeWidth = *(float*)(Msg.data);
  m_SliderKneeWidth->SetFloatValue(m_KneeWidth);

  return 0;
}

int CCompressorModeDialog::UpdateGainCurve(Message &Msg)
{
  m_GainCurve = *(int*)(Msg.data);
  m_SpinGainCurve->SetValue(m_GainCurve);

  return 0;
}
