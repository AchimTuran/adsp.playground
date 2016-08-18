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

#include <vector>
#include <string>


class IModel
{
public:
  typedef std::vector<IParameter*> ParameterVector_t;

  IModel(std::string Name, int ID) :
    Name(Name),
    ID(ID)
  {
    m_ParameterIDMapping  = NULL;
    m_MaxParameters       = 0;
    m_ParameterArray      = NULL;
  }

  virtual ~IModel()
  {
    DestroyParameters();
  }

  
  const std::string Name;
  const int ID;


  virtual int SetParameter(int ID, void* Data, size_t Size)
  {
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

  virtual int GetParameter(int ID, void* Data, size_t Size)
  {
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
    for (size_t ii = 0; ii < ParameterVector.size(); ii++)
    {
      m_ParameterVector.push_back(ParameterVector[ii]);
    }
    InitializeParameters();

    return m_ParameterVector.size();
  }


  virtual int AddParameter(IParameter *Parameter)
  {
    if (!Parameter)
    {
      return -1;
    }

    // TODO: sort parameter IDs
    m_ParameterVector.push_back(Parameter);
    InitializeParameters();

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
        delete *iter;
        *iter = NULL;
      }
    }

    return 0;
  }

   
  virtual int Create() = 0;
  virtual void Destroy() = 0;

  
protected: // protected member methods
  virtual void DestroyParameters()
  {
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

  void InitializeParameters()
  {
    m_ParameterArray  = m_ParameterVector.data();
    m_MaxParameters   = m_ParameterVector.size();
  }


protected: // protected member variables
  int         *m_ParameterIDMapping; // TODO implement parameter ID LUT functionality
  int         m_MaxParameters;
  IParameter  **m_ParameterArray;

  ParameterVector_t m_ParameterVector;
};
