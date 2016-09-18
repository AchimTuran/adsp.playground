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



#include <vector>

#include "KodiThreads/threads/CriticalSection.h"
#include "KodiThreads/threads/SingleLock.h"

#include "Addon/MessageSystem/Interfaces/IProtocol.hpp"
#include "Addon/MessageSystem/Interfaces/ISocket.hpp"


class CMessageDispatcher
{
  friend class CMessageDispatcher;

public:
  CMessageDispatcher(IProtocol *Protocol, std::string Name, int ID = -1, bool IsMaster = false);
  virtual ~CMessageDispatcher();

  bool SetSockets(SocketVector_t &SocketVector, bool ForceSockets=false);
  bool AddSocket(ISocket *Socket);
  bool RemoveSocket(int SocketID);

  bool ConnectDispatcher(CMessageDispatcher *Dispatcher);
  bool DisconnectDispatcher(CMessageDispatcher *Dispatcher);

  bool SendMsg(int SocketID, int DispatcherID = -1);
  bool SendMsg(void *Data, size_t Size, int SocketID, int DispatcherID = -1);
  bool ReceiveMsg(Message *Msg);

  void ProcessMessage();
  void ProcessMessages();
  void ProcessConnectedMessage();
  void ProcessConnectedMessages();

  const std::string Name;     // Custom name for this object
  const int         ID;       // assigned unique member ID for message assignment

private:
  void DestroySockets();

  inline int GetSocketID(int SocketID)
  {
    if (SocketID < 0)
    {
      return -1;
    }

    CSingleLock lock(m_SocketLock);
    if (m_MaxSockets == 0)
    {
      return -1;
    }

    int id = -1;
    if (m_SocketIDLUT)
    {
      bool SocketIDFound = false;
      for (int ii = 0; ii < m_MaxSockets; ii++)
      {
        if (m_SocketIDLUT[ii] == SocketID)
        {
          id = ii;
          SocketIDFound = true;

          break;
        }
      }

      if (!SocketIDFound)
      {
        return -1;
      }
    }
    else
    {
      if (SocketID < m_MaxSockets)
      {
        id = SocketID;
      }
    }

    return id;
  }

  inline int GetDispatcherIdx(int DispatcherID)
  {
    int idx = -1;

    {
      CSingleLock lock(m_ConnectionLock);
      bool DispatcherFound = false;

      if(m_DispatcherIDLUT)
      {
        for (int ii = 0; ii < m_MaxConnectedDispatchers; ii++)
        {
          if (m_DispatcherIDLUT[ii] == DispatcherID)
          {
            idx = ii;
            DispatcherFound = true;

            break;
          }
        }
      }
      else
      {
        DispatcherFound = true;
        idx = DispatcherID;
      }

      if (!DispatcherFound)
      {
        return -1;
      }
    }

    return idx;
  }

  class CSocketSort
  {
  public:
    bool operator() (ISocket *Lhs, ISocket *Rhs)
    {
      return (Lhs->ID < Rhs->ID);
    }
  };


  // specific member variables for message processing
  CCriticalSection m_ConnectionLock;

  int                 m_MaxConnectedDispatchers;
  int                 *m_DispatcherIDLUT;
  CMessageDispatcher  **m_DispatcherArray;

  std::vector<int>                  m_DispatcherIDs;
  std::vector<CMessageDispatcher*>  m_Dispatchers;
  CMessageDispatcher                *m_Master;

  
  // specific member variables for Socket processing
  CCriticalSection  m_SocketLock;
  
  int               m_MaxSockets;
  int               *m_SocketIDLUT;
  ISocket           **m_SocketArray;

  std::vector<int>  m_SocketIDs;
  SocketVector_t    m_Sockets;
  CSocketSort       m_SocketSort;
  
  // specific member variables protocol handling
  IProtocol *m_Protocol;
  

  static int m_UniqueIDCount; // static member variable for object counting
};
