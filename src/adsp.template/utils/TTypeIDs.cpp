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



#include "adsp.template/utils/TTypeIDs.hpp"

using namespace std;


static const string loc_TypeLUT[TYPE_max] =
{
  "unknown",
  "raw bytes",
  "float",
  "double",
  "long double",
  "bool",
  "char",
  "unsigned char",
  "int",
  "unsigned int",
  "short int",
  "unsigned short int",
  "long int",
  "unsigned long int",
  "long long int",
  "unsigned long long int",
  "short",
  "unsigned short",
  "long",
  "unsigned long",
  "long long",
  "unsigned long long",
  "std::string"
};
