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



#include "AudioDSP/Streams/IADSPStreamBuilder.hpp"


class CADSPStreamBuilderAll : public IADSPStreamBuilder
{
public:
  CADSPStreamBuilderAll();
  ~CADSPStreamBuilderAll();

  virtual AE_DSP_ERROR ConstructStream(CADSPStream &ADSPStream, const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties);
  virtual AE_DSP_ERROR InitializeStream(CADSPStream &ADSPStream, const AE_DSP_SETTINGS *Settings);

  virtual AE_DSP_ERROR AddMode(CADSPStream &ADSPStream, IADSPMode *ADSPMode);
  virtual AE_DSP_ERROR AddMode(CADSPStream &ADSPStream, unsigned int ModeID);

  virtual AE_DSP_ERROR RemoveMode(CADSPStream &ADSPStream, IADSPMode *ADSPMode);
  virtual AE_DSP_ERROR RemoveMode(CADSPStream &ADSPStream, unsigned int ModeID);
};
