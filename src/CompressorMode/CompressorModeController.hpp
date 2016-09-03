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



#include "adsp.template/Addon/MVC/Interfaces/Controller/IController.hpp"

#include "CompressorMode/CompressorModeControllerMessages.hpp"

class CPostProcessCompressorModeControllerMessages;


class CCompressorModeController : public IController,
                                  public CCompressorModeControllerMessages
{
  friend class CCompressorModeControllerMessages;

public:
  CCompressorModeController();
  ~CCompressorModeController();

  virtual int Create();
  virtual void Destroy();

private:
  int SetTauRelease(Message &Msg);
  int SetTauAttack(Message &Msg);
  int SetThreshold(Message &Msg);
  int SetCompressionRation(Message &Msg);
  int SetKneeWidth(Message &Msg);
  int SetGainCurve(Message &Msg);

private:
  int   m_GainCurve;
  float m_TauRelease;
  float m_TauAttack;
  float m_Threshold;
  float m_CompressionRatio;
  float m_KneeWidth;
};