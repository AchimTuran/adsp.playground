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


#define CreateTSocketForward(ObjType, Obj, StringClass, ID) dynamic_cast<ISocket*>(new TSocketForward<ObjType>(Obj, StringClass::ToString(StringClass::ID), StringClass::ID))


template<class T>
class TSocketForward : public ISocket
{
public:
  TSocketForward(T *Dispatcher, std::string Name, int Signal) : ISocket(Name, Signal, sizeof(TSocketForward<T>))
  {
    m_Dispatcher = Dispatcher;
  }

  virtual int Set(Message &Msg)
  {
    if (Msg.signal != this->ID)
    {
      // TODO: error code
      return -1;
    }

    CSingleLock lock(this->Lock);
    this->HasUpdated = true;
    
    return m_Dispatcher->SendMsg(nullptr, 0, ISocket::ID);
  }

  virtual void* Get()
  {
    return nullptr;
  }

private:
  T *m_Dispatcher;
};
