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

#include <memory.h>


#define CreateTParameter(Type, StringClass, ID) dynamic_cast<IParameter*>(new TParameter<float>(StringClass::ToString(StringClass::ID), StringClass::ID))


template<class T>
class TParameter : public IParameter
{
public:
  TParameter(std::string Name, int ID) :
    IParameter(Name, ID, sizeof(T))
  {
    memset(&m_Data, 0, this->Size);
  }

  virtual void* GetDataPtr()
  {
    return (void*)&m_Data;
  }


protected: // protected member variables
  virtual int Set(void *Data)
  {
    memcpy(&this->m_Data, Data, this->Size);
    
    return 0;
  }
  
  virtual int Get(void *Data)
  {
    memcpy(Data, &this->m_Data, this->Size);

    return 0;
  }

  T m_Data;
};
