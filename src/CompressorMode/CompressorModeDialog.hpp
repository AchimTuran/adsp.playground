#pragma once
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



#include "Addon/MVC/Interfaces/View/IKodiGUIView.hpp"
#include "CompressorMode/CompressorModeDialogMessages.hpp"


class CCompressorModeDialog : public IKodiGUIView,
                              public CCompressorModeDialogMessages
{
  friend class CCompressorModeDialogMessages;

public:
  CCompressorModeDialog();

private:
  // Derived from IView
  virtual bool OnInit();
  virtual bool OnClick(int controlId);
  virtual bool OnFocus(int controlId);
  virtual bool OnAction(int actionId);
  virtual void OnClose();

  void ProcessSpinGainCurve();
  void ProcessSliderTauAttack();
  void ProcessSliderKneeWidth();
  void ProcessSliderThreshold();
  void ProcessSliderTauRelease();
  void ProcessSliderCompressionRatio();

  CAddonGUISpinControl           *m_SpinGainCurve;
  CAddonGUISettingsSliderControl *m_SliderTauRelease;
  CAddonGUISettingsSliderControl *m_SliderTauAttack;
  CAddonGUISettingsSliderControl *m_SliderThreshold;
  CAddonGUISettingsSliderControl *m_SliderCompressionRatio;
  CAddonGUISettingsSliderControl *m_SliderKneeWidth;
  
private: // MC methods
  int UpdateTauRelease(Message &Msg);
  int UpdateTauAttack(Message &Msg);
  int UpdateThreshold(Message &Msg);
  int UpdateCompressionRation(Message &Msg);
  int UpdateKneeWidth(Message &Msg);
  int UpdateGainCurve(Message &Msg);

private: // private member variables
  int   m_GainCurve;
  float m_TauRelease;
  float m_TauAttack;
  float m_Threshold;
  float m_CompressionRatio;
  float m_KneeWidth;

  float m_PageActionValue;
};
