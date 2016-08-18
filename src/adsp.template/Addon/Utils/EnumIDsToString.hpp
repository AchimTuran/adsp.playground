/*
 *      Copyright (C) 2005-2016 Team KODI
 *      http://kodi.tv
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
 *  along with KODI; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */



#undef BEGIN_ENUM_ID_CLASS
#undef ADD_ENUM_ID
#undef END_ENUM_ID_CLASS


#if !defined(CREATE_ENUM_ID_STRINGS)
#define BEGIN_ENUM_ID_CLASS(ID) class C##ID { public: typedef enum { INVALID_ENUM_ID = -1,
#define ADD_ENUM_ID(ID)         ID,
#define END_ENUM_ID_CLASS(ID)   MAX_ENUM_ID } e##ID; static const char* ToString(e##ID EnumId); private: static const char *m_EnumStrs[MAX_ENUM_ID]; };
#else
#define BEGIN_ENUM_ID_CLASS(ID) const char *C##ID::m_EnumStrs[C##ID::MAX_ENUM_ID] = {
#define ADD_ENUM_ID(ID)         #ID,
#define END_ENUM_ID_CLASS(ID)   };                                                                    \
                                const char* C##ID::ToString(C##ID::e##ID EnumId) {                  \
                                  if(EnumId <= C##ID::INVALID_ENUM_ID) return "INVALID_ENUM_ID";    \
                                  if(EnumId >= C##ID::MAX_ENUM_ID)     return "MAX_ENUM_ID";        \
                                  return m_EnumStrs[EnumId];                                            \
                                }
#endif
