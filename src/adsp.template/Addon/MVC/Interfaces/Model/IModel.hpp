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

#include "include/client.h"

#include <vector>
#include <string>

using namespace ADDON;

// TODO: shift method implementations into an cpp file
class IModel : public MVCObject
{
private: // private declarations
  typedef std::vector<int> ParameterIDVector_t;

  class CParameterSort
  {
  public:
    bool operator() (IParameter *Lhs, IParameter *Rhs)
    {
      return (Lhs->ID < Rhs->ID);
    }
  };
public:
  typedef std::vector<IParameter*> ParameterVector_t;

  IModel(std::string Name, int ID, int ConnectionID) :
    MVCObject(MVCObject::MODEL_OBJECT, Name, ID, ConnectionID)
  {
    m_ParameterIDMapping  = nullptr;
    m_MaxParameters       = 0;
    m_ParameterArray      = nullptr;
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

    int id = GetParameterID(ID);
    if (id < 0)
    {
      return -1;
    }

    return m_ParameterArray[id]->Set(ID, Data, Size);
  }

  virtual int GetParameter(int ID, void *Data, size_t Size)
  {
    CSingleLock lock(m_ParameterLock);
    if (m_MaxParameters <= 0)
    {
      return -1;
    }

    int id = GetParameterID(ID);
    if (id < 0)
    {
      return -1;
    }

    return m_ParameterArray[id]->Get(ID, Data, Size);
  }


  virtual int SetParameterVector(ParameterVector_t &ParameterVector)
  {
    if (ParameterVector.size() <= 0)
    {
      KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to assign an empty parameter vector to model %s!", __FUNCTION__, __LINE__, Name.c_str());
      return false;
    }

    CSingleLock lock(m_ParameterLock);
    if (m_ParameterVector.size() > 0)
    {
      for (int ii = 0; ii < m_ParameterVector.size(); ii++)
      {
        ParameterVector.push_back(m_ParameterVector[ii]);
      }

      m_ParameterIDs.clear();
      m_ParameterVector.clear();
    }

    // sort the Parameter IDs in a ascending consecutive order
    sort(ParameterVector.begin(), ParameterVector.end(), m_Sort);

    int IDDistance = 1;
    for (unsigned int ii = 1; ii < ParameterVector.size() && IDDistance == 1; ii++)
    {
      int diff = ParameterVector[ii]->ID - ParameterVector[ii - 1]->ID;

      if (diff <= 0)
      {// Invalid input! Two equal Parameter IDs, which is not supported!
        KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to assign equal parameters to model %s!", __FUNCTION__, __LINE__, Name.c_str());

        for (size_t ii = 0; ii < ParameterVector.size(); ii++)
        {
          if (ParameterVector[ii])
          {
            delete ParameterVector[ii];
            ParameterVector[ii] = NULL;
          }
        }

        return false;
      }

      if (diff > IDDistance)
      {
        IDDistance = diff;
      }
    }

    // if there is only one Parameter or the first one has an ID > 0
    // always adjust IDDistance
    if ((ParameterVector.size() == 1 && ParameterVector[0]->ID > 0) || ParameterVector[0]->ID)
    {
      IDDistance = ParameterVector[0]->ID;
    }

    // if IDDistance is greater 1, this object will need a LUT (Look Up Table) for its Parameter IDs
    if (IDDistance != 1)
    {
      KODI->Log(LOG_DEBUG, "%s, %i, The model %s will use a LUT for Parameter assignment", __FUNCTION__, __LINE__, Name.c_str());
      m_ParameterIDs.reserve(ParameterVector.size());
      for (unsigned int ii = 0; ii < ParameterVector.size(); ii++)
      {
        m_ParameterIDs.push_back(ParameterVector[ii]->ID);
      }
    }

    KODI->Log(LOG_DEBUG, "%s, %i, The model %s has the following parameters: ", __FUNCTION__, __LINE__, Name.c_str());
    m_ParameterVector.reserve(ParameterVector.size());
    SocketVector_t sockets;
    for (unsigned int ii = 0; ii < ParameterVector.size(); ii++)
    {
      KODI->Log(LOG_DEBUG, "%s, %i,  Parameter: %s ID: %i", __FUNCTION__, __LINE__, ParameterVector[ii]->Name.c_str(), ParameterVector[ii]->ID);
      sockets.push_back(dynamic_cast<ISocket*>(new TSocketClassMethodCallback<IModel>(this, &IModel::UpdateModel, ParameterVector[ii]->Name, ParameterVector[ii]->ID)));
      m_ParameterVector.push_back(ParameterVector[ii]);
    }

    if (!this->SetSockets(sockets))
    {
      // TODO log error message
      return -1;
    }

    if (m_ParameterIDs.size() > 0)
    {
      m_ParameterIDMapping = m_ParameterIDs.data();
    }
    else
    {
      m_ParameterIDMapping = nullptr;
    }

    m_MaxParameters  = m_ParameterVector.size();
    m_ParameterArray = m_ParameterVector.data();

    return m_MaxParameters;
  }

  virtual int AddParameter(IParameter *Parameter)
  {
    if (!Parameter)
    {
      return -1;
    }

    int id = GetParameterID(Parameter->ID);
    if (id != -1)
    {
      KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to add parameter %s with ID %i, which is already registered in model %s", __FUNCTION__, __LINE__, Parameter->Name.c_str(), Parameter->ID, Name.c_str());
      delete Parameter;
      return false;
    }

    if (!this->AddSocket(dynamic_cast<ISocket*>(new TSocketClassMethodCallback<IModel>(this, &IModel::UpdateModel, Parameter->Name, Parameter->ID))))
    {
      return -1;
    }

    CSingleLock lock(m_ParameterLock);
    m_ParameterVector.push_back(Parameter);

    // sort the Socket IDs in a ascending consecutive order
    sort(m_ParameterVector.begin(), m_ParameterVector.end(), m_Sort);

    // todo check order!
    m_ParameterIDs.clear();
    m_ParameterIDs.reserve(m_ParameterVector.size());
    for (unsigned int ii = 0; ii < m_ParameterVector.size(); ii++)
    {
      m_ParameterIDs[ii] = m_ParameterVector[ii]->ID;
    }

    KODI->Log(LOG_DEBUG, "%s, %i, Added parameter %s with ID %i to model %s", __FUNCTION__, __LINE__, Parameter->Name.c_str(), Parameter->ID, Name.c_str());

    m_ParameterIDMapping = m_ParameterIDs.data();
    m_MaxParameters = m_ParameterVector.size();
    m_ParameterArray = m_ParameterVector.data();

    return m_MaxParameters;
  }

  virtual int RemoveParameter(int ID)
  {
    int id = GetParameterID(ID);
    if (id < 0)
    {
      KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to remove a negative ParameterID from model %s!", __FUNCTION__, __LINE__, Name.c_str());
      return false;
    }

    CSingleLock lock(m_ParameterLock);

    ParameterVector_t::iterator iter = m_ParameterVector.begin() + id;
    delete *iter;
    *iter = NULL;

    this->RemoveSocket(m_ParameterVector[id]->ID);
    m_ParameterVector.erase(m_ParameterVector.begin() + id);
    m_ParameterIDs.erase(m_ParameterIDs.begin() + id);

    if (m_ParameterIDs.size() <= 0 || m_ParameterVector.size() <= 0)
    {

      m_ParameterIDMapping  = nullptr;
      m_MaxParameters       = 0;
      m_ParameterArray      = nullptr;
    }
    else
    {
      m_ParameterIDMapping = m_ParameterIDs.data();
      m_MaxParameters = m_ParameterVector.size();
      m_ParameterArray = m_ParameterVector.data();
    }

    KODI->Log(LOG_DEBUG, "%s, %i, Removed Parameter %s with ID %i", __FUNCTION__, __LINE__, m_ParameterArray[id]->Name.c_str(), m_ParameterArray[id]->ID);

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
  ParameterIDVector_t m_ParameterIDs;

private:
  inline int GetParameterID(int ParameterID)
  {
    if (ParameterID < 0)
    {
      return -1;
    }

    CSingleLock lock(m_ParameterLock);
    if (m_MaxParameters == 0)
    {
      return -1;
    }

    int id = ParameterID;
    if (m_ParameterIDMapping)
    {
      bool ParameterIDFound = false;
      for (int ii = 0; ii < m_MaxParameters; ii++)
      {
        if (m_ParameterIDMapping[ii] == ParameterID)
        {
          id = ii;
          ParameterIDFound = true;

          break;
        }
      }

      if (!ParameterIDFound)
      {
        return -1;
      }
    }

    return id;
  }

  CCriticalSection  m_ParameterLock;
  CParameterSort    m_Sort;
};
