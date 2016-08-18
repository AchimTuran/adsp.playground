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



#include "Addon/MessageSystem/Communication/Message.hpp"
#include "KodiThreads/threads/CriticalSection.h"


class ISocket
{
public:
  ISocket(std::string Name, int ID, size_t Size) :
    Name(Name),
    ID(ID),
    Size(Size)
  {
    CSingleLock lock(this->Lock);
    HasUpdated = false;
  }

  virtual ~ISocket()
  {
  }

  const std::string Name;
  const int ID;
  const size_t Size;

  CCriticalSection Lock;
  bool HasUpdated;

  virtual int Set(Message &Msg) = 0;
  virtual void* Get() = 0;
};


// typedef for socket registration
typedef std::vector<ISocket*> SocketVector_t;
