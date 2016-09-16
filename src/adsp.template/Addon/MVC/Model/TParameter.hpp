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

// TODO add more macros
#define CreateFloatParameter(StringClass, ID)       dynamic_cast<IParameter*>(new TParameter<float, TYPE_float>(StringClass::ToString(StringClass::ID), StringClass::ID))
#define CreateDoubleParameter(StringClass, ID)      dynamic_cast<IParameter*>(new TParameter<double, TYPE_double>(StringClass::ToString(StringClass::ID), StringClass::ID))
#define CreateIntParameter(StringClass, ID)         dynamic_cast<IParameter*>(new TParameter<int, TYPE_int>(StringClass::ToString(StringClass::ID), StringClass::ID))
#define CreateUnsignedIntParameter(StringClass, ID) dynamic_cast<IParameter*>(new TParameter<unsigned int, TYPE_unsigned_int>(StringClass::ToString(StringClass::ID), StringClass::ID))


template<class T, TypeIDs_t TID>
class TParameter :  public IParameter,
                    public TTypeIDs<T, TID>
{
public:
  TParameter(std::string Name, int ID) :
    IParameter(Name, ID, sizeof(T), TTypeIDs<T, TID>::TypeID, TTypeIDs<T, TID>::TypeIDStr)
  {
    int id = TTypeIDs<T, TID>::TypeID;
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
