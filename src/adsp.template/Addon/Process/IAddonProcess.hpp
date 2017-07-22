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
  }

  const int ConnectionID;

// Fixed public methods
  virtual AUDIODSP_ADDON_ERROR Create() = 0;
  virtual AUDIODSP_ADDON_ERROR Destroy() = 0;

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
        {// disconnect this model
          m_Model->DisconnectDispatcher(m_Controller);
          m_Controller->DisconnectDispatcher(m_Model);
          m_Model = nullptr;
        }

        m_Model = Object;
      break;

      case MVCObject::VIEW_OBJECT:
      {// register view only once
        bool found = false;
        for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
        {
          if (*iter == Object)
          {
            found = true;
            break;
          }
        }
        
        if (!found)
        {
          m_Views.push_back(Object);
        }
      }
      break;

      case MVCObject::CONTROLLER_OBJECT:
        if (m_Controller)
        {
          for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
          {// disconnect all views
            (*iter)->DisconnectDispatcher(m_Controller);
          }

          if (m_Model)
          {// disconnect model
            m_Model->DisconnectDispatcher(m_Controller);
          }
        }

        m_Controller = Object;
        for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
        {// connect all registered views
          (*iter)->ConnectDispatcher(m_Controller);
        }

        if (m_Model)
        {// connect registered model
          m_Model->ConnectDispatcher(m_Controller);
        }
      break;

      default:
        return false;
    }

    if (Object->Type != MVCObject::CONTROLLER_OBJECT && m_Controller)
    {
      Object->ConnectDispatcher(m_Controller);
      m_Controller->ConnectDispatcher(Object);
    }

    return true;
  }

  virtual bool DisconnectObject(MVCObject *Object)
  {
    if (Object->ConnectionID != this->ConnectionID)
    {
      return false;
    }

    switch (Object->Type)
    {
      case MVCObject::MODEL_OBJECT:
        if (m_Model == Object && m_Controller)
        {
          m_Controller->DisconnectDispatcher(m_Model);
          m_Model->DisconnectDispatcher(m_Controller);
        }
        m_Model = nullptr;
      break;

      case MVCObject::VIEW_OBJECT:
        for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
        {
          if (*iter == Object)
          {
            if (m_Controller)
            {
              m_Controller->DisconnectDispatcher(*iter);
              (*iter)->DisconnectDispatcher(m_Controller);
            }

            m_Views.erase(iter);
            break;
          }
        }
      break;

      case MVCObject::CONTROLLER_OBJECT:
        if (m_Controller == Object)
        {
          for (MVCObjectList_t::iterator iter = m_Views.begin(); iter != m_Views.end(); ++iter)
          {
            (*iter)->DisconnectDispatcher(m_Controller);
            m_Controller->DisconnectDispatcher(*iter);
          }

          if (m_Model)
          {
            m_Model->DisconnectDispatcher(m_Controller);
            m_Controller->DisconnectDispatcher(m_Model);
          }

          m_Controller = nullptr;
        }
      break;

      default:
        return false;
    }

    return true;
  }

  void DisconnectAll()
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

private:
  MVCObject *m_Controller;
  MVCObject *m_Model;
  MVCObjectList_t m_Views;
};
