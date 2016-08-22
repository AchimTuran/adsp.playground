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



#include "KodiThreads/threads/Thread.h"
#include "Addon/MessageSystem/Interfaces/IProtocol.hpp"


#define MSG_INTERNAL_BUFFER_SIZE 32

class Message
{
  friend class IProtocol;

public:
  Message() 
  {
    signal = -1;
    isSync = false;
    isSyncFini = false;
    isOut = false;
    isSyncTimeout = false;
    payloadSize = MSG_INTERNAL_BUFFER_SIZE;
    size = MSG_INTERNAL_BUFFER_SIZE;
    memset(buffer, 0, sizeof(uint8_t)*MSG_INTERNAL_BUFFER_SIZE);
    data = buffer;
    replyMessage = nullptr;

    origin = nullptr;
    destiny = nullptr;

    event = nullptr;
  };

  int       signal;
  bool      isSync;
  bool      isSyncFini;
  bool      isOut;
  bool      isSyncTimeout;
  int       payloadSize;
  int       size;
  uint8_t   buffer[MSG_INTERNAL_BUFFER_SIZE];
  uint8_t   *data;
  Message   *replyMessage;
  
  IProtocol *origin;    // address to sender Protocol instance
  IProtocol *destiny;   // address to receiver Protocol instance
  
  CEvent    *event;

  void Release();
  bool Reply(int sig, void *data = NULL, int size = 0);
};
