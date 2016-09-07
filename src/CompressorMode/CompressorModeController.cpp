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



#include "Addon/MVC/Model/TParameter.hpp"
#include "CompressorMode/CompressorModeController.hpp"
#include "EnumStrIDs.hpp"

#define MAX_Compressor 90.0f

CCompressorModeController::CCompressorModeController() :
  IController(CDispatcherIDs::ToString(CDispatcherIDs::CompressorModeController),
              CDispatcherIDs::CompressorModeController,
              CADSPModeIDs::PostProcessingModeCompressor)
{
}


CCompressorModeController::~CCompressorModeController()
{
}


int CCompressorModeController::Create()
{
  if (!CCompressorModeControllerMessages::Create(this))
  {
    // TODO error message
    return -1;
  }

  return 0;
}

void CCompressorModeController::Destroy()
{
}

int CCompressorModeController::SetTauRelease(Message & Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  float tauRelease = *((float*)Msg.data);
  if (tauRelease >= MAX_Compressor || tauRelease <= -MAX_Compressor)
  {
    // TODO: error code
    return -1;
  }

  m_TauRelease = tauRelease;
  if (!this->SendMsg(&m_TauRelease, sizeof(float), CSocketCompressorModeIDs::UpdateTauRelease))
  {
    // TODO: error code
    return -1;
  }
  return 0;
}

int CCompressorModeController::SetTauAttack(Message & Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  float tauAttack = *((float*)Msg.data);
  if (tauAttack >= MAX_Compressor || tauAttack <= -MAX_Compressor)
  {
    // TODO: error code
    return -1;
  }

  m_TauAttack = tauAttack;
  if (!this->SendMsg(&m_TauAttack, sizeof(float), CSocketCompressorModeIDs::UpdateTauAttack))
  {
    // TODO: error code
    return -1;
  }
  return 0;
}

int CCompressorModeController::SetThreshold(Message & Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  float threshold = *((float*)Msg.data);
  if (threshold >= MAX_Compressor || threshold <= -MAX_Compressor)
  {
    // TODO: error code
    return -1;
  }

  m_Threshold = threshold;
  if (!this->SendMsg(&m_Threshold, sizeof(float), CSocketCompressorModeIDs::UpdateThreshold))
  {
    // TODO: error code
    return -1;
  }
  return 0;
}

int CCompressorModeController::SetCompressionRation(Message & Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  float compressionRatio = *((float*)Msg.data);
  if (compressionRatio >= MAX_Compressor || compressionRatio <= -MAX_Compressor)
  {
    // TODO: error code
    return -1;
  }

  m_CompressionRatio = compressionRatio;
  if (!this->SendMsg(&m_CompressionRatio, sizeof(float), CSocketCompressorModeIDs::UpdateCompressionRatio))
  {
    // TODO: error code
    return -1;
  }
  return 0;
}

int CCompressorModeController::SetKneeWidth(Message & Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  float kneeWidth = *((float*)Msg.data);
  if (kneeWidth >= MAX_Compressor || kneeWidth <= -MAX_Compressor)
  {
    // TODO: error code
    return -1;
  }

  m_KneeWidth = kneeWidth;
  if (!this->SendMsg(&m_KneeWidth, sizeof(float), CSocketCompressorModeIDs::UpdateKneeWidth))
  {
    // TODO: error code
    return -1;
  }
  return 0;
}

int CCompressorModeController::SetGainCurve(Message & Msg)
{
  if (Msg.size != sizeof(float))
  {
    // TODO: error code
    return -1;
  }

  int gainCurve = *((int*)Msg.data);
  if (gainCurve >= MAX_Compressor || gainCurve <= -MAX_Compressor)
  {
    // TODO: error code
    return -1;
  }

  m_GainCurve = gainCurve;
  if (!this->SendMsg(&m_GainCurve, sizeof(int), CSocketCompressorModeIDs::UpdateGainCurve))
  {
    // TODO: error code
    return -1;
  }
  return 0;
}

int CCompressorModeController::RequestModelState(Message & Msg)
{
  if (!this->SendMsg(nullptr, 0, CSocketCompressorModeIDs::RequestModelState))
  {
    // TODO: error code
    return -1;
  }

  return 0;
}

