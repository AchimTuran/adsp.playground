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
#include "CompressorMode/CompressorModeModel.hpp"
#include "EnumStrIDs.hpp"


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

  paramVector.push_back(CreateTParameter(float, CSocketCompressorModeIDs, UpdateTauRelease));
  paramVector.push_back(CreateTParameter(float, CSocketCompressorModeIDs, UpdateTauAttack));
  paramVector.push_back(CreateTParameter(float, CSocketCompressorModeIDs, UpdateThreshold));
  paramVector.push_back(CreateTParameter(float, CSocketCompressorModeIDs, UpdateCompressionRatio));
  paramVector.push_back(CreateTParameter(float, CSocketCompressorModeIDs, UpdateKneeWidth));
  paramVector.push_back(CreateTParameter(int, CSocketCompressorModeIDs, UpdateGainCurve));

  if (this->SetParameterVector(paramVector) <= 0)
  {
    // TODO error message
    return -1;
  }

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

int CCompressorModeModel::RequestGainCurve(Message &Msg)
{
  if (!this->SendMsg(static_cast<void*>(&m_GainCurve), sizeof(float), CSocketCompressorModeIDs::UpdateGainCurve))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestTauRelease(Message &Msg)
{
  if (!this->SendMsg(static_cast<void*>(&m_TauRelease), sizeof(float), CSocketCompressorModeIDs::UpdateTauRelease))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestTauAttack(Message &Msg)
{
  if (!this->SendMsg(static_cast<void*>(&m_TauAttack), sizeof(float), CSocketCompressorModeIDs::UpdateTauAttack))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestThreshold(Message &Msg)
{
  if (!this->SendMsg(static_cast<void*>(&m_Threshold), sizeof(float), CSocketCompressorModeIDs::UpdateThreshold))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestCompressionRatio(Message &Msg)
{
  if (!this->SendMsg(static_cast<void*>(&m_CompressionRatio), sizeof(float), CSocketCompressorModeIDs::UpdateCompressionRatio))
  {
    return -1;
  }

  return 0;
}

int CCompressorModeModel::RequestKneeWidth(Message &Msg)
{
  if (!this->SendMsg(static_cast<void*>(&m_KneeWidth), sizeof(float), CSocketCompressorModeIDs::UpdateKneeWidth))
  {
    return -1;
  }

  return 0;
}
