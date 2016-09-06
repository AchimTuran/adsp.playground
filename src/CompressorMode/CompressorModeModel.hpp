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



#include "adsp.template/Addon/MVC/Interfaces/Model/IModel.hpp"

#include "CompressorModeModelMessages.hpp"

class CCompressorModeModelMessages;


class CCompressorModeModel : public IModel,
                             public CCompressorModeModelMessages
{
  friend class CCompressorModeModelMessages;

public:
  CCompressorModeModel();
  ~CCompressorModeModel();

  virtual int Create();
  virtual void Destroy();

private:
  int RequestModelState(Message &Msg);
  int RequestGainCurve(Message &Msg);
  int RequestTauRelease(Message &Msg);
  int RequestTauAttack(Message &Msg);
  int RequestThreshold(Message &Msg);
  int RequestCompressionRatio(Message &Msg);
  int RequestKneeWidth(Message &Msg);
};
