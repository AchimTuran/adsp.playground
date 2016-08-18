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


template<class T>
class TCreationStatistics
{
public:
  TCreationStatistics()
  {
    m_Created++;
    m_Active++;
  }

  ~TCreationStatistics()
  {
    m_Destroyed++;
    m_Active--;
  }

  static int GetStatisticsCreated()
  {
    return m_Created;
  }

  static int GetStatisticsActive()
  {
    return m_Active;
  }

  static int GetStatisticsDestroyed()
  {
    return m_Destroyed;
  }


private:
  static int m_Created;
  static int m_Active;
  static int m_Destroyed;
};

template<class T>
int TCreationStatistics<T>::m_Created;

template<class T>
int TCreationStatistics<T>::m_Active;

template<class T>
int TCreationStatistics<T>::m_Destroyed;
