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

#include "utils/stdStringUtils.h"

#include <math.h>


#define BUTTON_OK                 10050
#define BUTTON_CANCEL             10051
#define BUTTON_DEFAULT            10052

#define SLIDER_MAIN_GAIN          8000
#define LABEL_MAIN_GAIN_DB_LEVEL  8200

#define MAIN_GAIN_MAX_DB          90.0f
#define MAIN_GAIN_MIN_DB          -90.0f
#define MAIN_GAIN_PAGE_STEP       1.0f


std::string float_dB_toString(float dB);


CGainModeDialog::CGainModeDialog() :
  IKodiGUIView("DialogGainMode.xml", false, true, 
               CDispatcherIDs::ToString(CDispatcherIDs::GainModeDialog), 
               CDispatcherIDs::GainModeDialog,
               CADSPModeIDs::PostProcessingModeGain)
{
  m_MainGain = 0.0f;
  m_PageActionValue = 0.0f;
}


bool CGainModeDialog::OnInit()
{
  m_MainGainSlider = GUI->Control_getSettingsSlider(this->m_window, SLIDER_MAIN_GAIN);
  if (!m_MainGainSlider)
  {
    KODI->Log(ADDON::LOG_ERROR, "%s, %i, Slider with ID: %i (%s) not found!", __FUNCTION__, __LINE__, SLIDER_MAIN_GAIN, "Main Gain");
    return false;
  }
  m_MainGainSlider->SetFloatRange(MAIN_GAIN_MIN_DB, MAIN_GAIN_MAX_DB);
  m_MainGainSlider->SetFloatValue(m_MainGain);
  m_MainGainSlider->SetVisible(true);
  m_window->SetControlLabel(LABEL_MAIN_GAIN_DB_LEVEL, float_dB_toString(m_MainGain).c_str());

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


    case SLIDER_MAIN_GAIN:
      ProcessMainGainSlider();
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
  GUI->Control_releaseSettingsSlider(m_MainGainSlider);
}

void CGainModeDialog::ProcessMainGainSlider()
{
  if (m_PageActionValue != 0.0f)
  {
    m_MainGain += m_PageActionValue;
    m_MainGainSlider->SetFloatValue(m_MainGain);
    m_MainGain = m_MainGainSlider->GetFloatValue(); 
  }
  else
  {
    m_MainGain = m_MainGainSlider->GetFloatValue();
  }

  m_window->SetControlLabel(LABEL_MAIN_GAIN_DB_LEVEL, float_dB_toString(m_MainGain).c_str());
  this->SendMsg(static_cast<void*>(&m_MainGain), sizeof(float), CSocketGainModeIDs::UpdateMainGain);
}

// private MC callback methods
int CGainModeDialog::UpdateMainGain(Message &Msg)
{
  m_MainGain = *(float*)(Msg.data);
  m_MainGainSlider->SetFloatValue(m_MainGain);
  m_window->SetControlLabel(LABEL_MAIN_GAIN_DB_LEVEL, float_dB_toString(m_MainGain).c_str());

  return 0;
}


// helper functions
std::string float_dB_toString(float dB)
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
