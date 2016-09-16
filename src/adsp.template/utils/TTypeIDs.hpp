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


#define ADD_TYPE(T, ID)    const std::string  TTypeIDs<T, ID>::TypeIDStr = loc_TypeLUT[ID]; \
                           const TypeIDs_t    TTypeIDs<T, ID>::TypeID = ID;

typedef enum
{
  TYPE_invalid = -2,

  TYPE_custom = -1,

  TYPE_unkown = 0,
  TYPE_raw_bytes,
  TYPE_float,
  TYPE_double,
  TYPE_long_double,
  TYPE_bool,
  TYPE_char,
  TYPE_unsigned_char,
  TYPE_int,
  TYPE_unsigned_int,
  TYPE_short_int,
  TYPE_unsigned_short_int,
  TYPE_long_int,
  TYPE_unsigned_long_int,
  TYPE_long_long_int,
  TYPE_unsigned_long_long_int,
  TYPE_short,
  TYPE_unsigned_short,
  TYPE_long,
  TYPE_unsigned_long,
  TYPE_long_long,
  TYPE_unsigned_long_long,
  TYPE_std_string,

  TYPE_max
}TypeIDs_t;

template<typename T, TypeIDs_t ID=TYPE_unkown, int CustomType=-1>
class TTypeIDs
{
public:
  static const std::string TypeIDStr;
  static const TypeIDs_t TypeID;
  static const int CustomID;
};

extern const std::string loc_TypeLUT[TYPE_max];

template<typename T, TypeIDs_t ID, int CustomType>
const std::string TTypeIDs<T, ID, CustomType>::TypeIDStr = loc_TypeLUT[TYPE_unkown];

template<typename T, TypeIDs_t ID, int CustomType>
const TypeIDs_t TTypeIDs<T, ID, CustomType>::TypeID = TYPE_unkown;

template<typename T, TypeIDs_t ID, int CustomType>
const int TTypeIDs<T, ID, CustomType>::CustomID = CustomType;

ADD_TYPE(unsigned char, TYPE_raw_bytes)
ADD_TYPE(float, TYPE_float)
ADD_TYPE(double, TYPE_double)
ADD_TYPE(long double, TYPE_long_double)
ADD_TYPE(bool, TYPE_bool)
ADD_TYPE(char, TYPE_char)
ADD_TYPE(unsigned char, TYPE_unsigned_char)
ADD_TYPE(int, TYPE_int)
ADD_TYPE(unsigned int, TYPE_unsigned_int)
ADD_TYPE(short int, TYPE_short_int)
ADD_TYPE(unsigned short int, TYPE_unsigned_short_int)
ADD_TYPE(long int, TYPE_long_int)
ADD_TYPE(unsigned long int, TYPE_unsigned_long_int)
ADD_TYPE(long long int, TYPE_long_long_int)
ADD_TYPE(unsigned long long int, TYPE_unsigned_long_long_int)
ADD_TYPE(short, TYPE_short)
ADD_TYPE(unsigned short, TYPE_unsigned_short)
ADD_TYPE(long, TYPE_long)
ADD_TYPE(unsigned long, TYPE_unsigned_long)
ADD_TYPE(long long, TYPE_long_long)
ADD_TYPE(unsigned long long, TYPE_unsigned_long_long)
ADD_TYPE(std::string, TYPE_std_string)
