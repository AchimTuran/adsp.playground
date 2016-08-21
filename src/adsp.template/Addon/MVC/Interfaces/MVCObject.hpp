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


class MVCObject
{
public:
  typedef enum {
    INVALID_OBJECT = -1,

    MODEL_OBJECT,
    VIEW_OBJECT,
    CONTROLLER_OBJECT,

    MAX
  }eMVCObjectType_t;

  MVCObject(eMVCObjectType_t Type, std::string Name, int ID) :
    Type(Type),
    Name(Name),
    ID(ID)
  {
  }

  const int ID;
  const std::string Name;
  const eMVCObjectType_t Type;

  virtual int Create() = 0;
  virtual void Destroy() = 0;
};
