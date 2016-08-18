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
#include <queue>
#include "memory.h"

#include "Addon/MessageSystem/Interfaces/IProtocol.hpp"


class CActorProtocol : public IProtocol
{
public:
  CActorProtocol(std::string Name, CEvent* InEvent = NULL, CEvent *OutEvent = NULL) :
    IProtocol(Name, InEvent, OutEvent)
  {}

  virtual ~CActorProtocol();

  // get a message from freeMessageQueue or create a new one
  virtual Message *GetMessage();
  // return a message and store it in freeMessageQueue
  virtual void ReturnMessage(Message *msg);
  // send an out message and store a pointer in outMessages
  virtual bool SendOutMessage(int signal, void *data = NULL, int size = 0, Message *outMsg = NULL);
  // send an in message and store a pointer in inMessages
  virtual bool SendInMessage(int signal, void *data = NULL, int size = 0, Message *outMsg = NULL);
  // send an synchronuous out message with a time out value
  virtual bool SendOutMessageSync(int signal, Message **retMsg, int timeout, void *data = NULL, int size = 0);
  virtual bool ReceiveOutMessage(Message **msg);
  virtual bool ReceiveInMessage(Message **msg);

  // empty in- and out-message queues
  virtual void Purge();
  // empty in-message queue except the message with the ID stored in signal
  virtual void PurgeIn(int Signal);
  // empty out-message queue except the message with the ID stored in signal
  virtual void PurgeOut(int Signal);

protected:
  std::queue<Message*> outMessages;             // thread safe out message queue
  std::queue<Message*> inMessages;              // thread safe in message queue
  std::queue<Message*> freeMessageQueue;        // thread safe free message queue that are used for new messages
};
