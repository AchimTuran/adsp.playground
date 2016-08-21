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


#include "kodi/kodi_adsp_types.h"

#include "Addon/MVC/Interfaces/MVCObject.hpp"

#include <list>

class CAddonProcessManager;


class IAddonProcess
{
  friend class CAddonProcessManager;
  typedef std::list<MVCObject*> MVCObjectList_t;
public:
  IAddonProcess(int ConnectionID) :
    ConnectionID(ConnectionID)
  {
    m_Controller  = nullptr;
    m_Model       = nullptr;
    m_Views.clear();
  }

  virtual ~IAddonProcess()
  {
    if (m_Controller)
    {
      for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
      {
        (*iter)->DisconnectDispatcher(m_Controller);
      }

      if (m_Model)
      {
        m_Model->DisconnectDispatcher(m_Controller);
      }
    }
    m_Views.clear();

  }

  const int ConnectionID;

// Fixed public methods
  virtual AE_DSP_ERROR Create() = 0;
  virtual AE_DSP_ERROR Destroy() = 0;

  virtual bool ConnectObject(MVCObject *Object)
  {
    if (Object->ConnectionID != this->ConnectionID)
    {
      return false;
    }

    switch (Object->Type)
    {
      case MVCObject::MODEL_OBJECT:
        if (m_Model && m_Controller)
        {
          m_Model->DisconnectDispatcher(m_Controller);
        }
        m_Model = Object;
      break;

      case MVCObject::VIEW_OBJECT:
        m_Views.push_back(Object);
      break;

      case MVCObject::CONTROLLER_OBJECT:
        if (m_Controller)
        {
          for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
          {
            (*iter)->DisconnectDispatcher(m_Controller);
          }
        }
        m_Controller = Object;
        for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
        {
          (*iter)->ConnectDispatcher(m_Controller);
        }

        if (m_Model)
        {
          m_Model->ConnectDispatcher(m_Controller);
        }
      break;

      default:
        return false;
    }

    if (Object->Type != MVCObject::CONTROLLER_OBJECT && m_Controller)
    {
      Object->ConnectDispatcher(m_Controller);
    }

    return true;
  }

  virtual bool DisconnectObject(MVCObject *Object)
  {
    if (Object->ConnectionID != this->ConnectionID)
    {
      return false;
    }

    return true;
  }

private:
  MVCObject *m_Controller;
  MVCObject *m_Model;
  std::list<MVCObject*> m_Views;
};
