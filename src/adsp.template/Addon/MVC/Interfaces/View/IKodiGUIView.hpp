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



#include "Addon/MVC/Interfaces/View/IView.hpp"

#include <kodi/libKODI_guilib.h>
#include "ActionIDs.hpp"

#include "include/client.h"


class IKodiGUIView : public IView
{
public:
  IKodiGUIView(std::string XMLFileName, bool ForceFallback, bool AsDialog, std::string Name, int ID, int ConnectionID, std::string DefaultSkin = "Confluence") :
    IView(Name, ID, ConnectionID)
  {
    m_XMLFileName = XMLFileName;
    m_DefaultSkin = DefaultSkin;
    m_ForceFallback = ForceFallback;
    m_AsDialog = AsDialog;
  }

  virtual ~IKodiGUIView()
  {
    Destroy();
  }

  void Destroy()
  {
    if (m_window)
    {
      GUI->Window_destroy(m_window);
    }

    m_window = NULL;
  }

  int Create()
  {
    m_window = GUI->Window_create(m_XMLFileName.c_str(),
                                  m_DefaultSkin.c_str(),
                                  m_ForceFallback,
                                  m_AsDialog);
    if (!m_window)
    {
      KODI->Log(ADDON::LOG_ERROR, "%s, %i, Couldn't create IView!", __FUNCTION__, __LINE__);
      return false;
    }

    m_window->m_cbhdl     = this;
    m_window->CBOnInit    = OnInitCB;
    m_window->CBOnFocus   = OnFocusCB;
    m_window->CBOnClick   = OnClickCB;
    m_window->CBOnAction  = OnActionCB;

    return true;
  }

  bool Show()
  {
    if (!Create() || !m_window)
    {
      return false;
    }

    return m_window->Show();
  }

  void Close()
  {
    if (m_window)
    {
      m_window->Close();
    }

    OnClose();

    Destroy();
  }

  void DoModal()
  {
    if (!Create() || !m_window)
    {
      return;
    }

    
    m_window->DoModal();
  }

private:
  static bool OnClickCB(GUIHANDLE cbhdl, int controlId)
  {
    IKodiGUIView *dialog = static_cast<IKodiGUIView*>(cbhdl);
    return dialog->OnClick(controlId);
  }

  static bool OnFocusCB(GUIHANDLE cbhdl, int controlId)
  {
    IKodiGUIView *dialog = static_cast<IKodiGUIView*>(cbhdl);
    return dialog->OnFocus(controlId);
  }

  static bool OnInitCB(GUIHANDLE cbhdl)
  {
    IKodiGUIView *dialog = static_cast<IKodiGUIView*>(cbhdl);
    return dialog->OnInit();
  }

  static bool OnActionCB(GUIHANDLE cbhdl, int actionId)
  {
    IKodiGUIView *dialog = static_cast<IKodiGUIView*>(cbhdl);
    return dialog->OnAction(actionId);
  }

protected:
  CAddonGUIWindow          *m_window;  // window handle

private:  // private virtual methods
  virtual bool OnInit() = 0;
  virtual bool OnClick(int controlId) = 0;
  virtual bool OnFocus(int controlId) = 0;
  virtual bool OnAction(int actionId) = 0;
  virtual void OnClose() = 0;

  std::string m_XMLFileName;
  bool m_ForceFallback;
  bool m_AsDialog;
  std::string m_DefaultSkin;
};
