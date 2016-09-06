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
 
 

#include "Addon/MVC/Interfaces/Model/IParameter.hpp"
#include "Addon/MVC/Interfaces/MVCObject.hpp"
#include "Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"

#include <vector>
#include <string>


class IModel : public MVCObject
{
public:
  typedef std::vector<IParameter*> ParameterVector_t;

  IModel(std::string Name, int ID, int ConnectionID) :
    MVCObject(MVCObject::MODEL_OBJECT, Name, ID, ConnectionID)
  {
    m_ParameterIDMapping  = NULL;
    m_MaxParameters       = 0;
    m_ParameterArray      = NULL;
  }

  virtual ~IModel()
  {
    DestroyParameters();
  }


  virtual int SetParameter(int ID, void *Data, size_t Size)
  {
    CSingleLock lock(m_ParameterLock);
    if (m_MaxParameters <= 0)
    {
      return -1;
    }

    if (m_ParameterIDMapping)
    {
      for (int ii = 0; ii < m_MaxParameters; ii++)
      {
        if (ID == m_ParameterIDMapping[ii])
        {
          ID = m_ParameterIDMapping[ii];
          break;
        }
      }
    }

    return m_ParameterArray[ID]->Set(ID, Data, Size);
  }

  virtual int GetParameter(int ID, void *Data, size_t Size)
  {
    CSingleLock lock(m_ParameterLock);
    if (m_MaxParameters <= 0)
    {
      return -1;
    }

    if (m_ParameterIDMapping)
    {
      for (int ii = 0; ii < m_MaxParameters; ii++)
      {
        if (ID == m_ParameterIDMapping[ii])
        {
          ID = m_ParameterIDMapping[ii];
          break;
        }
      }
    }

    return m_ParameterArray[ID]->Get(ID, Data, Size);
  }


  virtual int SetParameterVector(ParameterVector_t &ParameterVector)
  {
    if (ParameterVector.size() <= 0)
    {
      return -1;
    }

    // TODO: sort parameter IDs
    SocketVector_t sockets;
    CSingleLock lock(m_ParameterLock);
    for (size_t ii = 0; ii < ParameterVector.size(); ii++)
    {
      // TODO: how to get signal name string?
      sockets.push_back(dynamic_cast<ISocket*>(new TSocketClassMethodCallback<IModel>(this, &IModel::UpdateModel, "", ParameterVector[ii]->ID)));
      m_ParameterVector.push_back(ParameterVector[ii]);
    }

    if (!this->SetSockets(sockets))
    {
      // TODO log error message
      return -1;
    }

    m_ParameterArray = m_ParameterVector.data();
    m_MaxParameters = m_ParameterVector.size();

    return m_ParameterVector.size();
  }

  virtual int AddParameter(IParameter *Parameter)
  {
    if (!Parameter)
    {
      return -1;
    }

    // TODO: how to get signal name string?
    if (!this->AddSocket(dynamic_cast<ISocket*>(new TSocketClassMethodCallback<IModel>(this, &IModel::UpdateModel, "", Parameter->ID))))
    {
      return -1;
    }

    CSingleLock lock(m_ParameterLock);
    // TODO: sort parameter IDs
    m_ParameterVector.push_back(Parameter);
    m_ParameterArray = m_ParameterVector.data();
    m_MaxParameters = m_ParameterVector.size();

    return m_MaxParameters;
  }

  virtual int RemoveParameter(int ID)
  {
    if (m_ParameterIDMapping)
    {
    }

    for (ParameterVector_t::iterator iter = m_ParameterVector.begin(); iter != m_ParameterVector.end(); ++iter)
    {
      if (ID == (*iter)->ID)
      {
        this->RemoveSocket((*iter)->ID);
        delete *iter;
        *iter = NULL;
      }
    }

    return 0;
  }

  virtual int SaveParameters()
  {
    // todo implement XML sink
    return 0;
  }
    
protected: // protected member methods
  virtual void DestroyParameters()
  {
    CSingleLock lock(m_ParameterLock);
    m_ParameterArray = NULL;

    for (size_t ii = 0; ii < m_ParameterVector.size(); ii++)
    {
      if (m_ParameterVector[ii])
      {
        delete m_ParameterVector[ii];
        m_ParameterVector[ii] = NULL;
      }
    }

    m_ParameterVector.clear();
  }

protected: // prototected member methods
  virtual int UpdateModel(Message &Msg)
  {
    return this->SetParameter(Msg.signal, Msg.data, Msg.size);
  }

protected: // protected member variables
  int         *m_ParameterIDMapping; // TODO implement parameter ID LUT functionality
  int         m_MaxParameters;
  IParameter  **m_ParameterArray;

  ParameterVector_t m_ParameterVector;

private:
  CCriticalSection  m_ParameterLock;
};
