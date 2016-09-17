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



#include "adsp.template/Addon/MVC/Model/TParameter.hpp"
#include "CompressorMode/CompressorModeModel.hpp"
#include "EnumStrIDs.hpp"
#include "CompressorMode/CompressorModeDefaults.h"


CCompressorModeModel::CCompressorModeModel() :
  IModel(CDispatcherIDs::ToString(CDispatcherIDs::CompressorModeModel),
         CDispatcherIDs::CompressorModeModel,
         CADSPModeIDs::PostProcessingModeCompressor)
{
}


CCompressorModeModel::~CCompressorModeModel()
{
}


int CCompressorModeModel::Create()
{
  IModel::ParameterVector_t paramVector;

  paramVector.push_back(CreateFloatParameter(CSocketCompressorModeIDs, UpdateTauRelease));
  paramVector.push_back(CreateFloatParameter(CSocketCompressorModeIDs, UpdateTauAttack));
  paramVector.push_back(CreateFloatParameter(CSocketCompressorModeIDs, UpdateThreshold));
  paramVector.push_back(CreateFloatParameter(CSocketCompressorModeIDs, UpdateCompressionRatio));
  paramVector.push_back(CreateFloatParameter(CSocketCompressorModeIDs, UpdateKneeWidth));
  paramVector.push_back(CreateIntParameter(CSocketCompressorModeIDs,   UpdateGainCurve));

  if (this->SetParameterVector(paramVector) <= 0)
  {
    // TODO error message
    return -1;
  }

  // set default values
  float fVal = COMPRESSOR_TAU_ATTACK_DEFAULT;
  this->SetParameter(CSocketCompressorModeIDs::UpdateTauAttack, static_cast<void*>(&fVal), sizeof(float));
  fVal = COMPRESSOR_TAU_RELEASE_DEFAULT;
  this->SetParameter(CSocketCompressorModeIDs::UpdateTauRelease, static_cast<void*>(&fVal), sizeof(float));
  fVal = COMPRESSOR_KNEE_WIDTH_DEFAULT;
  this->SetParameter(CSocketCompressorModeIDs::UpdateKneeWidth, static_cast<void*>(&fVal), sizeof(float));
  fVal = COMPRESSOR_THRESHOLD_DEFAULT;
  this->SetParameter(CSocketCompressorModeIDs::UpdateThreshold, static_cast<void*>(&fVal), sizeof(float));
  fVal = COMPRESSOR_COMPRESSION_RATIO_DEFAULT;
  this->SetParameter(CSocketCompressorModeIDs::UpdateCompressionRatio, static_cast<void*>(&fVal), sizeof(float));

  if (!CCompressorModeModelMessages::Create(this))
  {
    // TODO error message
    return -1;
  }

  return 0;
}

void CCompressorModeModel::Destroy()
{
}

int CCompressorModeModel::RequestModelState(Message &Msg)
{
  int iVal = 0;
  float fVal = 0.0;

  this->GetParameter(CSocketCompressorModeIDs::UpdateGainCurve, static_cast<void*>(&iVal), sizeof(int));
  if (!this->SendMsg(static_cast<void*>(&iVal), sizeof(int), CSocketCompressorModeIDs::UpdateGainCurve))
  {
    return -1;
  }

  this->GetParameter(CSocketCompressorModeIDs::UpdateTauRelease, static_cast<void*>(&fVal), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateTauRelease))
  {
    return -1;
  }

  this->GetParameter(CSocketCompressorModeIDs::UpdateKneeWidth, static_cast<void*>(&fVal), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateKneeWidth))
  {
    return -1;
  }

  this->GetParameter(CSocketCompressorModeIDs::UpdateCompressionRatio, static_cast<void*>(&fVal), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateCompressionRatio))
  {
    return -1;
  }

  this->GetParameter(CSocketCompressorModeIDs::UpdateThreshold, static_cast<void*>(&fVal), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateThreshold))
  {
    return -1;
  }

  this->GetParameter(CSocketCompressorModeIDs::UpdateTauAttack, static_cast<void*>(&fVal), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&fVal), sizeof(float), CSocketCompressorModeIDs::UpdateTauAttack))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestGainCurve(Message &Msg)
{
  int val = 0;
  this->GetParameter(CSocketCompressorModeIDs::UpdateGainCurve, static_cast<void*>(&val), sizeof(int));
  if (!this->SendMsg(static_cast<void*>(&val), sizeof(int), CSocketCompressorModeIDs::UpdateGainCurve))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestTauRelease(Message &Msg)
{
  float val = 0.0;
  this->GetParameter(CSocketCompressorModeIDs::UpdateTauRelease, static_cast<void*>(&val), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&val), sizeof(float), CSocketCompressorModeIDs::UpdateTauRelease))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestTauAttack(Message &Msg)
{
  float val = 0.0;
  this->GetParameter(CSocketCompressorModeIDs::UpdateTauAttack, static_cast<void*>(&val), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&val), sizeof(float), CSocketCompressorModeIDs::UpdateTauAttack))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestThreshold(Message &Msg)
{
  float val = 0.0;
  this->GetParameter(CSocketCompressorModeIDs::UpdateThreshold, static_cast<void*>(&val), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&val), sizeof(float), CSocketCompressorModeIDs::UpdateThreshold))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestCompressionRatio(Message &Msg)
{
  float val = 0.0;
  this->GetParameter(CSocketCompressorModeIDs::UpdateCompressionRatio, static_cast<void*>(&val), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&val), sizeof(float), CSocketCompressorModeIDs::UpdateCompressionRatio))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestKneeWidth(Message &Msg)
{
  float val = 0.0;
  this->GetParameter(CSocketCompressorModeIDs::UpdateKneeWidth, static_cast<void*>(&val), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&val), sizeof(float), CSocketCompressorModeIDs::UpdateKneeWidth))
  {
    return -1;
  }

  return 0;
}
