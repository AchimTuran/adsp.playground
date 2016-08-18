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



#include <string>
#include <queue>

#include "KodiThreads/threads/Thread.h"

class Message;


class IProtocol
{
public:
  IProtocol(std::string &Name, CEvent* InEvent, CEvent *OutEvent) :
    portName(Name)
  {
    inDefered   = false;
    outDefered  = false;

    containerInEvent  = InEvent;
    containerOutEvent = OutEvent;
  };

  virtual ~IProtocol() {}

  // ???
  void DeferIn(bool Value) { inDefered = Value; };
  
  // ???
  void DeferOut(bool Value) { outDefered = Value; };
  
  // set thread safe lock
  void Lock() { criticalSection.lock(); };
  
  // unset thread safe lock
  void Unlock() { criticalSection.unlock(); };

  

  // protocol port name
  const std::string portName;


// abstract methods  
  // return a message and store it in freeMessageQueue
  virtual void ReturnMessage(Message *Msg) = 0;
  // send an out message and store a pointer in outMessages
  virtual bool SendOutMessage(int Signal, void *Data = NULL, int Size = 0, Message *OutMsg = NULL) = 0;
  // send an in message and store a pointer in inMessages
  virtual bool SendInMessage(int Signal, void *Data = NULL, int Size = 0, Message *InMsg = NULL) = 0;
  // send an synchronuous out message with a time out value
  virtual bool SendOutMessageSync(int Signal, Message **RetMsg, int Timeout, void *Data = NULL, int Size = 0) = 0;
  virtual bool ReceiveOutMessage(Message **Msg) = 0;
  virtual bool ReceiveInMessage(Message **Msg) = 0;

  // empty in- and out-message queues
  virtual void Purge() = 0;
  // empty in-message queue except the message with the ID stored in signal
  virtual void PurgeIn(int Signal) = 0;
  // empty out-message queue except the message with the ID stored in signal
  virtual void PurgeOut(int Signal) = 0;
  // get a message from freeMessageQueue or create a new one
  virtual Message *GetMessage() = 0;

protected:
  CEvent *containerInEvent;
  CEvent *containerOutEvent;                    // ???
  CCriticalSection criticalSection;             // lock object for accessing message queues
  
  bool inDefered;                               // Defer messages for in or out
  bool outDefered;                              // Defer messages for in or out
};
