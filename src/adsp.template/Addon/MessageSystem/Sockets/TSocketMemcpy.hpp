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



#include "Addon/MessageSystem/Interfaces/ISocket.hpp"
#include "Addon/MessageSystem/Communication/Message.hpp"


#define CreateTSocketMemcpy(Type, ControlledMember, StringClass, ID) dynamic_cast<ISocket*>(new TSocketMemcpy<float>(ControlledMember, StringClass::ToString(StringClass::ID), StringClass::ID))


template<class T>
class TSocketMemcpy : public ISocket
{
public:
  TSocketMemcpy(T *SocketPtr, std::string Name, int Signal) : ISocket(Name, Signal, sizeof(T))
  {
    if (!SocketPtr)
    {
      // TODO throw exception
    }

    m_SocketPtr = SocketPtr;
    memcpy(&m_CurrenVal, SocketPtr, this->Size);
  }


  ~TSocketMemcpy()
  {
  }

  virtual int Set(Message &Msg)
  {
    if (!Msg.data || Msg.signal != this->ID || Msg.size != this->Size)
    {
      // TODO: error code
      return -1;
    }

    CSingleLock lock(this->Lock);
    this->HasUpdated = true;
    memcpy(&m_CurrenVal, Msg.data, this->Size);
    memcpy(this->m_SocketPtr, &m_CurrenVal, this->Size);

    return 0;
  }
  
  virtual void* Get()
  {
    return &m_CurrenVal;
  }


protected:
  T *m_SocketPtr;
  T m_CurrenVal;  
};
