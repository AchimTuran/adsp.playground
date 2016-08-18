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



#include <memory.h>

#include "Message.hpp"
#include "Addon/MessageSystem/Interfaces/IProtocol.hpp"


void Message::Release()
{
  bool skip;
  origin->Lock();
  skip = isSync ? !isSyncFini : false;
  isSyncFini = true;
  origin->Unlock();

  if (skip)
    return;

  // free data buffer
  if (data != buffer)
    delete[] data;

  // delete event in case of sync message
  if (event)
    delete event;

  origin->ReturnMessage(this);
}

bool Message::Reply(int sig, void *data /* = NULL*/, int size /* = 0 */)
{
  if (!isSync)
  {
    if (isOut)
      return origin->SendInMessage(sig, data, size);
    else
      return origin->SendOutMessage(sig, data, size);
  }

  origin->Lock();

  if (!isSyncTimeout)
  {
    Message *msg = origin->GetMessage();
    msg->signal = sig;
    msg->isOut = !isOut;
    replyMessage = msg;
    if (data)
    {
      if (size > MSG_INTERNAL_BUFFER_SIZE)
        msg->data = new uint8_t[size];
      else
        msg->data = msg->buffer;
      memcpy(msg->data, data, size);
    }
  }

  origin->Unlock();

  if (event)
    event->Set();

  return true;
}
