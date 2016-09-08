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



#include "Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp"


#define CreateTSocketRangeCheck(DispatcherClass, RangeType, DispatcherObj, Min, Max, StringClass, ID) dynamic_cast<ISocket*>(new TSocketRangeCheck<DispatcherClass, RangeType>(DispatcherObj, Min, Max, StringClass::ToString(StringClass::ID), StringClass::ID))


template<class TDispatcher, typename T>
class TSocketRangeCheck : public TSocketClassMethodCallback<TSocketRangeCheck<TDispatcher, T>>
{
public:
  TSocketRangeCheck(TDispatcher *Dispatcher, T MinValue, T MaxValue, std::string Name, int Signal) :
    TSocketClassMethodCallback<TSocketRangeCheck<TDispatcher, T>>(this, &TSocketRangeCheck<TDispatcher, T>::RangeCheck, Name, Signal)
  {
    m_MinValue    = MinValue;
    m_MaxValue    = MaxValue;
    m_Dispatcher  = Dispatcher;
  }

private:
  int RangeCheck(Message &Msg)
  {
    if (Msg.size != sizeof(T))
    {
      // TODO: error code
      return -1;
    }

    T value = *((T*)Msg.data);
    if (!(m_MinValue <= value && value <= m_MaxValue))
    {
      // TODO: error code
      return -1;
    }

    if (!m_Dispatcher->SendMsg(static_cast<void*>(&value), sizeof(T), ISocket::ID))
    {
      // TODO: error code
      return -1;
    }

    return 0;
  }

  TDispatcher *m_Dispatcher;
  T m_MinValue;
  T m_MaxValue;
};
