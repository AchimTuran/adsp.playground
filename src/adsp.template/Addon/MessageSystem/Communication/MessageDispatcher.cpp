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



#include "Addon/MessageSystem/Communication/MessageDispatcher.hpp"

#include "include/client.h"

#include <algorithm>

using namespace std;
using namespace ADDON;


int CMessageDispatcher::m_UniqueIDCount = 0;


CMessageDispatcher::CMessageDispatcher(IProtocol *Protocol, std::string Name, int ID/*=-1*/, bool IsMaster/* = false*/) :
  ID(++m_UniqueIDCount),
  Name(Name)
{
  if (!Protocol)
  {
    // TODO throw exception!
  }

  KODI->Log(LOG_DEBUG, "%s, %i, Creating message handler %s with unique ID %i", __FUNCTION__, __LINE__, Name.c_str(), ID);

  m_Protocol = Protocol;
  
  {
    CSingleLock socketLock(m_SocketLock);
    m_MaxSockets  = 0;
    m_SocketIDLUT = NULL;
    m_SocketArray = NULL;
  }

  CSingleLock lock(m_ConnectionLock);
  m_MaxConnectedDispatchers = 0;
  m_DispatcherIDLUT         = NULL;
  m_DispatcherArray         = NULL;

  if (IsMaster)
  {
    m_Master = this;
  }
  else
  {
    m_Master = NULL;
  }
}

CMessageDispatcher::~CMessageDispatcher()
{
  KODI->Log(LOG_DEBUG, "%s, %i, Destroying message handler %s with unique ID %i", __FUNCTION__, __LINE__, Name.c_str(), ID);

  {
    CSingleLock lock(m_ConnectionLock);
    if (m_Master != this && m_Master)
    {
      DisconnectDispatcher(this);
    }
  }

  DestroySockets();

  if (m_Protocol)
  {
    delete m_Protocol;
    m_Protocol = NULL;
  }
}


bool CMessageDispatcher::SetSockets(SocketVector_t &SocketVector)
{
  if (SocketVector.size() <= 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to assign an empty Socket vector to message handler %s!", __FUNCTION__, __LINE__, Name.c_str());
    return false;
  }

  CSingleLock lock(m_SocketLock);
  if (m_Sockets.size() > 0)
  {
    for (int ii = 0; ii < (int)m_Sockets.size(); ii++)
    {
      SocketVector.push_back(m_Sockets[ii]);
    }

    m_SocketIDs.clear();
    m_Sockets.clear();
  }

  // sort the Socket IDs in a ascending consecutive order
  sort(SocketVector.begin(), SocketVector.end(), m_SocketSort);

  int IDDistance = 1;
  for (unsigned int ii = 1; ii < SocketVector.size() && IDDistance == 1; ii++)
  {
    int diff = SocketVector[ii]->ID - SocketVector[ii - 1]->ID;

    if (diff <= 0)
    {// Invalid input! Two equal Socket IDs, which is not supported!
      KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to assign equal Sockets to the message handler %s!", __FUNCTION__, __LINE__, Name.c_str());

      for (size_t ii = 0; ii < SocketVector.size(); ii++)
      {
        if (SocketVector[ii])
        {
          delete SocketVector[ii];
          SocketVector[ii] = NULL;
        }
      }

      return false;
    }

    if (diff > IDDistance)
    {
      IDDistance = diff;
    }
  }

  // if there is only one Socket or the first one has an ID > 0
  // always adjust IDDistance
  if ((SocketVector.size() == 1 && SocketVector[0]->ID > 0) || SocketVector[0]->ID)
  {
    IDDistance = SocketVector[0]->ID;
  }

  // if IDDistance is greater 1, this object will need a LUT (Look Up Table) for its Socket IDs
  if (IDDistance != 1)
  {
    KODI->Log(LOG_DEBUG, "%s, %i, The message handler %s will use a LUT for Socket assignment", __FUNCTION__, __LINE__, Name.c_str());
    m_SocketIDs.reserve(SocketVector.size());
    for (unsigned int ii = 0; ii < SocketVector.size(); ii++)
    {
      m_SocketIDs.push_back(SocketVector[ii]->ID);
    }
  }

  KODI->Log(LOG_DEBUG, "%s, %i, The message handler %s has the following Sockets: ", __FUNCTION__, __LINE__, Name.c_str());
  m_Sockets.reserve(SocketVector.size());
  for (unsigned int ii = 0; ii < SocketVector.size(); ii++)
  {
    KODI->Log(LOG_DEBUG, "%s, %i,  Socket: %s ID: %i", __FUNCTION__, __LINE__, SocketVector[ii]->Name.c_str(), SocketVector[ii]->ID);
    m_Sockets.push_back(SocketVector[ii]);
  }

  if (m_SocketIDs.size() > 0)
  {
    m_SocketIDLUT  = m_SocketIDs.data();
  }
  else
  {
    m_SocketIDLUT = nullptr;
  }
  m_MaxSockets   = m_Sockets.size();
  m_SocketArray  = m_Sockets.data();

  return true;
}

bool CMessageDispatcher::AddSocket(ISocket *Socket)
{
  if (!Socket)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to add an invalid socket to message handler %s!", __FUNCTION__, __LINE__, Name.c_str());
    return false;
  }

  int id = GetSocketID(Socket->ID);
  if (id != -1)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to add socket %s with ID %i, which is already available at message dispatcher %s", __FUNCTION__, __LINE__, Socket->Name.c_str(), Socket->ID, Name.c_str());
    delete Socket;
    return false;
  }

  CSingleLock lock(m_SocketLock);
  m_Sockets.push_back(Socket);

  // sort the Socket IDs in a ascending consecutive order
  sort(m_Sockets.begin(), m_Sockets.end(), m_SocketSort);

  // todo check order!
  m_SocketIDs.clear();
  m_SocketIDs.reserve(m_Sockets.size());
  for (unsigned int ii = 0; ii < m_Sockets.size(); ii++)
  {
    m_SocketIDs[ii] = m_Sockets[ii]->ID;
  }

  KODI->Log(LOG_DEBUG, "%s, %i, Added socket %s with ID %i to message handler %s", __FUNCTION__, __LINE__, Socket->Name.c_str(), Socket->ID, Name.c_str());

  m_SocketIDLUT = m_SocketIDs.data();
  m_MaxSockets  = m_Sockets.size();
  m_SocketArray = m_Sockets.data();

  return false;
}

bool CMessageDispatcher::RemoveSocket(int SocketID)
{
  int id = GetSocketID(SocketID);
  if (id < 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to remove a negative SocketID from message handler %s!", __FUNCTION__, __LINE__, Name.c_str());
    return false;
  }

  CSingleLock lock(m_SocketLock);

  SocketVector_t::iterator iter = m_Sockets.begin() + id;
  delete *iter;
  *iter = NULL;

  m_Sockets.erase(m_Sockets.begin() + id);
  m_SocketIDs.erase(m_SocketIDs.begin() + id);

  if (m_SocketIDs.size() <= 0 || m_Sockets.size() <= 0)
  {

    m_SocketIDLUT  = NULL;
    m_MaxSockets   = 0;
    m_SocketArray  = NULL;
  }
  else
  {
    m_SocketIDLUT  = m_SocketIDs.data();
    m_MaxSockets   = m_Sockets.size();
    m_SocketArray  = m_Sockets.data();
    KODI->Log(LOG_DEBUG, "%s, %i, Removed Socket %s with ID %i", __FUNCTION__, __LINE__, m_SocketArray[id]->Name.c_str(), m_SocketArray[id]->ID);
  }

  return true;
}


bool CMessageDispatcher::SendMsg(int SocketID, int DispatcherID/* = -1*/)
{
  CSingleLock socketLock(m_SocketLock);
  int socketIdx = GetSocketID(SocketID);
  if(socketIdx < 0)
  {
    return false;
  }

  CSingleLock socketArrayLock(m_SocketArray[socketIdx]->Lock);
  if (!m_SocketArray[socketIdx]->HasUpdated)
  {
    return false;
  }

  CSingleLock lock(m_ConnectionLock);
  if (DispatcherID < 0)
  { // send this Socket ID to all connected Dispatchers
    for (int ii = 0; ii < m_MaxConnectedDispatchers; ii++)
    {// TODO: check return value
      m_DispatcherArray[ii]->m_Protocol->SendInMessage(m_SocketArray[socketIdx]->ID,
                                                       m_SocketArray[socketIdx]->Get(),
                                                       m_SocketArray[socketIdx]->Size);
    }

    return true;
  }
  else
  {
    int DispatcherIdx = GetDispatcherIdx(DispatcherID);
    if (DispatcherIdx < 0)
    {
      return false;
    }

    m_SocketArray[socketIdx]->HasUpdated = false;
    return m_DispatcherArray[DispatcherIdx]->m_Protocol->SendInMessage(m_SocketArray[socketIdx]->ID,
                                                                       m_SocketArray[socketIdx]->Get(),
                                                                       m_SocketArray[socketIdx]->Size);
  }

  return false;
}

bool CMessageDispatcher::SendMsg(void *Data, size_t Size, int SocketID, int DispatcherID/* = -1*/)
{
  CSingleLock lock(m_ConnectionLock);

  if (DispatcherID < 0)
  {// send message to all connected dispatchers
    for (int ii = 0; ii < m_MaxConnectedDispatchers; ii++)
    {// TODO: check return value
      m_DispatcherArray[ii]->m_Protocol->SendInMessage(SocketID, Data, Size);
    }

    return true;
  }
  else
  {
    int dispatcherIdx = GetDispatcherIdx(DispatcherID);
    if (dispatcherIdx < 0)
    {
      return false;
    }

    return m_DispatcherArray[dispatcherIdx]->m_Protocol->SendInMessage(SocketID, Data, Size);
  }

  return false;
}


bool CMessageDispatcher::ConnectDispatcher(CMessageDispatcher *Dispatcher)
{
  if (!Dispatcher)
  {
    return false;
  }

  {
    CSingleLock lock(m_ConnectionLock);

    m_Dispatchers.push_back(Dispatcher);
    m_DispatcherIDs.push_back(Dispatcher->ID);

    m_DispatcherIDLUT = m_DispatcherIDs.data();
    m_DispatcherArray = m_Dispatchers.data();
    m_MaxConnectedDispatchers = m_Dispatchers.size();

    CSingleLock parterLock(Dispatcher->m_ConnectionLock);
    if (m_Master == this)
    {
      Dispatcher->m_Master = this;
      KODI->Log(LOG_DEBUG, "%s, %i, Dispatcher %s connected to master dispatcher %s", __FUNCTION__, __LINE__, Dispatcher->Name.c_str(), Name.c_str());
    }
    else
    {
      KODI->Log(LOG_NOTICE, "%s, %i, Dispatcher %s connected to non master dispatcher %s! The message system might not work correctly!", __FUNCTION__, __LINE__, Dispatcher->Name.c_str(), Name.c_str());
    }
  }

  return true;
}

bool CMessageDispatcher::DisconnectDispatcher(CMessageDispatcher *Dispatcher)
{
  if (!Dispatcher)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Invalid input! Tried to disconnect invalid dispatcher at dispatcher %s", __FUNCTION__, __LINE__, Name.c_str());
    return false;
  }

  CSingleLock lock(m_ConnectionLock);
  if (m_Dispatchers.size() <= 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Tried to disconnect dispatcher %s with ID %i from dispatcher %s without any connected dispatchers!", __FUNCTION__, __LINE__, Dispatcher->Name.c_str(), Dispatcher->ID, Name.c_str());
    return false;
  }

  int dispatcherIdx = GetDispatcherIdx(Dispatcher->ID);
  if (dispatcherIdx < 0)
  {
    KODI->Log(LOG_ERROR, "%s, %i, Invalid input! The dispatcher %s with ID %i was not connected to dispatcher %s", __FUNCTION__, __LINE__, Dispatcher->Name.c_str(), Dispatcher->ID, Name.c_str());
    return false;
  }

  m_Dispatchers.erase(m_Dispatchers.begin() + dispatcherIdx);
  m_DispatcherIDs.erase(m_DispatcherIDs.begin() + dispatcherIdx);

  if(m_Dispatchers.size() > 0)
  {
    m_DispatcherIDLUT = m_DispatcherIDs.data();
    m_DispatcherArray = m_Dispatchers.data();
    m_MaxConnectedDispatchers = m_Dispatchers.size();
  }
  else
  {
    m_DispatcherIDLUT = NULL;
    m_DispatcherArray = NULL;
    m_MaxConnectedDispatchers = 0;
  }

  KODI->Log(LOG_DEBUG, "%s, %i, Disconnected dispatcher %s with ID %i from dispatcher %s", __FUNCTION__, __LINE__, Dispatcher->Name.c_str(), Dispatcher->ID, Name.c_str());

  CSingleLock parterLock(Dispatcher->m_ConnectionLock);
  Dispatcher->m_Master = NULL;

  return true;
}

void CMessageDispatcher::ProcessMessage()
{
  Message *msg = NULL;
  if (m_Protocol && m_Protocol->ReceiveInMessage(&msg))
  {
    int SocketID = GetSocketID(msg->signal);
    if (SocketID >= 0)
    {
      CSingleLock lock(m_SocketLock);
      int err = m_SocketArray[SocketID]->Set(*msg);
      if (err != 0)
      {
        // TODO reply with error
      }
    }
    else
    {
      // TODO reply with error
    }

    msg->Release();
    msg = NULL;
  }
}

void CMessageDispatcher::ProcessMessages()
{
  Message *msg = NULL;
  while (m_Protocol && m_Protocol->ReceiveInMessage(&msg))
  {
    int socketID = GetSocketID(msg->signal);
    if (socketID >= 0)
    {
      CSingleLock lock(m_SocketLock);
      int err = m_SocketArray[socketID]->Set(*msg);
      if (err != 0)
      {
        // TODO reply with error
      }
    }
    else
    {
      // TODO reply with error
    }

    msg->Release();
    msg = NULL;
  }
}

void CMessageDispatcher::ProcessConnectedMessage()
{
  CSingleLock lock(m_ConnectionLock);

  for (int ii = 0; ii < m_MaxConnectedDispatchers; ii++)
  {
    m_DispatcherArray[ii]->ProcessMessage();
  }
}

void CMessageDispatcher::ProcessConnectedMessages()
{
  CSingleLock lock(m_ConnectionLock);

  for (int ii = 0; ii < m_MaxConnectedDispatchers; ii++)
  {
    m_DispatcherArray[ii]->ProcessMessages();
  }
}
//bool CMessageDispatcher::RemoveDispatcher(CMessageDispatcher *Dispatcher)
//{
//  if (!Dispatcher)
//  {
//    return false;
//  }
//
//  int DispatcherID = GetDispatcherIdx(Dispatcher->ID);
//  if (DispatcherID < 0)
//  {
//    return false;
//  }
//
//  {
//    CSingleLock lock(m_ConnectionLock);
//    m_Dispatchers.erase(m_Dispatchers.begin() + DispatcherID);
//    m_DispatcherIDs.erase(m_DispatcherIDs.begin() + DispatcherID);
//
//    m_DispatcherIDLUT          = m_DispatcherIDs.data();
//    m_DispatcherArray          = m_Dispatchers.data();
//    m_MaxConnectedDispatchers  = m_Dispatchers.size();
//  }
//
//  return true;
//}


void CMessageDispatcher::DestroySockets()
{
  CSingleLock lock(m_SocketLock);

  if (m_Sockets.size() > 0)
  {
    for (SocketVector_t::iterator iter = m_Sockets.begin(); iter != m_Sockets.end(); ++iter)
    {
      if (*iter)
      {
        delete *iter;
      }

      *iter = NULL;
    }

    m_Sockets.clear();
    m_SocketArray = NULL;

    m_SocketIDs.clear();
    m_SocketIDLUT = NULL;
  }
}


bool CMessageDispatcher::ReceiveMsg(Message *Msg)
{
  if (!Msg)
  {
    return false;
  }

  if (!m_Protocol->SendInMessage(Msg->signal, Msg->data, Msg->size))
  {
    return false;
  }

  //TODO: find a better solution to release or store the message here!
  Msg->Release();
  return true;
}
