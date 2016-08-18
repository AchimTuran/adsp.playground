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
#include <stdint.h>


class IParameter
{
public:
  IParameter(std::string Name, int ID, size_t Size) :
    Name(Name),
    ID(ID),
    Size(Size)
  {
  }

  virtual ~IParameter()
  {
  }


  const std::string Name;
  const int ID;
  const size_t Size;


  int Set(int ID, void *Data, size_t Size)
  {
    if (!Data || ID != this->ID || Size != this->Size)
    {
      return -1;
    }

    return Set(Data);
  }

  int Get(int ID, void *Data, size_t Size)
  {
    if (!Data || ID != this->ID || Size != this->Size)
    {
      return -1;
    }

    return Get(Data);
  }

  virtual void* GetDataPtr() = 0;

protected: // protected member methods
  virtual int Set(void *Data) = 0;
  virtual int Get(void *Data) = 0;
};
