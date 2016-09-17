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



#include "GainMode/GainModeDialogSettings.hpp"
#include "GainMode/GainModeDialog.hpp"
#include "GainModeEnvironment.hpp"
#include "adsp.template/Addon/Process/AddonProcessManager.hpp"

#include "EnumStrIDs.hpp"

#include "adsp.template/utils/stdStringUtils.h"

#include <math.h>


#define BUTTON_OK                 10050
#define BUTTON_CANCEL             10051
#define BUTTON_DEFAULT            10052

#define SLIDER_FL                 8000
#define SLIDER_FR                 8001
#define SLIDER_FC                 8002
#define SLIDER_LFE                8003
#define SLIDER_BL                 8004
#define SLIDER_BR                 8005
#define SLIDER_FLOC               8006
#define SLIDER_FROC               8007
#define SLIDER_BC                 8008
#define SLIDER_SL                 8009
#define SLIDER_SR                 8010
#define SLIDER_TFL                8011
#define SLIDER_TFR                8012
#define SLIDER_TFC                8013
#define SLIDER_TC                 8014
#define SLIDER_TBL                8015
#define SLIDER_TBR                8016
#define SLIDER_TBC                8017
#define SLIDER_BLOC               8018
#define SLIDER_BROC               8019
#define LABEL_MAIN_GAIN_DB_LEVEL  8200

#define MAIN_GAIN_MAX_DB          90.0f
#define MAIN_GAIN_MIN_DB          -90.0f
#define MAIN_GAIN_PAGE_STEP       1.0f


static std::string float_dB_toString(float dB);


CGainModeDialog::CGainModeDialog() :
  IKodiGUIView("DialogGainMode.xml", false, true, 
               CDispatcherIDs::ToString(CDispatcherIDs::GainModeDialog), 
               CDispatcherIDs::GainModeDialog,
               CADSPModeIDs::PostProcessingModeGain)
{
  memset(m_GainSliders, NULL, sizeof(m_GainSliders));
  m_PageActionValue = 0.0f;
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_Gain[ii] = 0.0f;
  }
}


bool CGainModeDialog::OnInit()
{
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    m_GainSliders[ii] = GUI->Control_getSettingsSlider(this->m_window, SLIDER_FL + ii);
    if (!m_GainSliders[ii])
    {
      KODI->Log(ADDON::LOG_ERROR, "%s, %i, Slider with ID: %i (%s) not found!", __FUNCTION__, __LINE__, SLIDER_FL + ii, "Main Gain");
      return false;
    }
    m_GainSliders[ii]->SetFloatRange(MAIN_GAIN_MIN_DB, MAIN_GAIN_MAX_DB);
    m_GainSliders[ii]->SetFloatValue(m_Gain[ii]);
    m_GainSliders[ii]->SetVisible(true);
    //m_window->SetControlLabel(LABEL_MAIN_GAIN_DB_LEVEL, float_dB_toString(m_MainGain).c_str());
  }
  
  

  // TODO: why does GetCurrentSinkFormat fail?
  //AudioEngineFormat sinkFmt;
  //if (!AUDIOENGINE->GetCurrentSinkFormat(sinkFmt))
  //{
  //  KODI->Log(ADDON::LOG_ERROR, "%s, %i, Failed to get sink data format!", __FUNCTION__, __LINE__);
  //  return false;
  //}

  if (!CGainModeDialogMessages::Create(this))
  {
    KODI->Log(ADDON::LOG_ERROR, "%s, %i, Failed to connect sockets from %s", __FUNCTION__, __LINE__, this->Name.c_str());
    return false;
  }

  if(CAddonProcessManager::ConnectObject(this) != 0)
  {
    KODI->Log(ADDON::LOG_ERROR, "%s, %i, Failed to connect message dispachter %s", __FUNCTION__, __LINE__, this->Name.c_str());
    return false;
  }

  return true;
}

bool CGainModeDialog::OnClick(int controlId)
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

    case BUTTON_DEFAULT:
    {
    }
    break;

    case SLIDER_FL:
    case SLIDER_FR:
    case SLIDER_FC:
    case SLIDER_LFE:
    case SLIDER_BL:
    case SLIDER_BR:
    case SLIDER_FLOC:
    case SLIDER_FROC:
    case SLIDER_BC:
    case SLIDER_SL:
    case SLIDER_SR:
    case SLIDER_TFL:
    case SLIDER_TFR:
    case SLIDER_TFC:
    case SLIDER_TC:
    case SLIDER_TBL:
    case SLIDER_TBR:
    case SLIDER_TBC:
    case SLIDER_BLOC:
    case SLIDER_BROC:
      ProcessGainSlider(controlId - SLIDER_FL);
    break;
  }

  if(m_PageActionValue != 0.0f)
  {
    m_PageActionValue = 0.0f;
  }

  return true;
}

bool CGainModeDialog::OnFocus(int controlId)
{
  return false;
}

bool CGainModeDialog::OnAction(int actionId)
{
  if (actionId == ADDON_ACTION_CLOSE_DIALOG ||
      actionId == ADDON_ACTION_PREVIOUS_MENU ||
      actionId == ADDON_ACTION_NAV_BACK)
  {
    return OnClick(BUTTON_CANCEL);
  }

  if (actionId == ADDON_ACTION_PAGE_DOWN)
  {
    m_PageActionValue = -MAIN_GAIN_PAGE_STEP;
    return OnClick(m_window->GetFocusId());
  }

  if (actionId == ADDON_ACTION_PAGE_UP)
  {
    m_PageActionValue = MAIN_GAIN_PAGE_STEP;
    return OnClick(m_window->GetFocusId());
  }

  return false;
}

void CGainModeDialog::OnClose()
{
  CAddonProcessManager::DisconnectObject(this);
  for (int ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    if (m_GainSliders[ii])
    {
      GUI->Control_releaseSettingsSlider(m_GainSliders[ii]);
    }
  }
}

void CGainModeDialog::ProcessGainSlider(int Index)
{
  if (m_PageActionValue != 0.0f)
  {
    m_Gain[Index] += m_PageActionValue;
    m_GainSliders[Index]->SetFloatValue(m_Gain[Index]);
    m_Gain[Index] = m_GainSliders[Index]->GetFloatValue();
  }
  else
  {
    m_Gain[Index] = m_GainSliders[Index]->GetFloatValue();
  }

  this->SendMsg(static_cast<void*>(&m_Gain[Index]), sizeof(float), CSocketGainModeIDs::UpdateGain_FL + Index);
}

// private MC callback methods
int CGainModeDialog::UpdateGain(Message &Msg)
{
  if (Msg.signal < CSocketGainModeIDs::UpdateGain_FL || Msg.signal > CSocketGainModeIDs::UpdateGain_BROC)
  {
    return -1;
  }

  int index = Msg.signal - CSocketGainModeIDs::UpdateGain_FL;

  m_Gain[index] = *(float*)(Msg.data);
  m_GainSliders[index]->SetFloatValue(m_Gain[index]);

  return 0;
}


// helper functions
static std::string float_dB_toString(float dB)
{
  std::string str = toString(roundf(dB*10.0f)/10.0f);
  float val10 = (float)((int)fabsf(roundf(dB*10.0f)));
  int fraction = (int)(val10 - ((int)(val10/10.0f)*10.0f));

  if (fraction == 0 || dB == 0.0f)
  {
    str += ".0";
  }

  str += "dB";

  return str;
}
