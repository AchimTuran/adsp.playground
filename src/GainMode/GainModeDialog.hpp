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



#include "adsp.template/Addon/MVC/Interfaces/View/IKodiGUIView.hpp"
#include "GainMode/GainModeDialogMessages.hpp"


class CGainModeDialog : public IKodiGUIView,
                        public CGainModeDialogMessages
{
  friend class CGainModeDialogMessages;

public:
  CGainModeDialog();

private:
  // Derived from IView
  virtual bool OnInit();
  virtual bool OnClick(int controlId);
  virtual bool OnFocus(int controlId);
  virtual bool OnAction(int actionId);
  virtual void OnClose();

  void ProcessMainGainSlider();

  CAddonGUISettingsSliderControl *m_MainGainSlider;
  
private: // MC methods
  int UpdateMainGain(Message &Msg);

private: // private member variables
  float m_MainGain;
  float m_PageActionValue;
};
