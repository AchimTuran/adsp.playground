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
#include "GainMode/GainModeModel.hpp"
#include "EnumStrIDs.hpp"
#include "GainModeDefaults.h"


CGainModeModel::CGainModeModel() :
  IModel(CDispatcherIDs::ToString(CDispatcherIDs::GainModeModel),
         CDispatcherIDs::GainModeModel,
         CADSPModeIDs::PostProcessingModeGain)
{
}


CGainModeModel::~CGainModeModel()
{
}


int CGainModeModel::Create()
{
  IModel::ParameterVector_t paramVector;

  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_FL));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_FR));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_FC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_LFE));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_BL));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_BR));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_FLOC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_FROC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_BC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_SL));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_SR));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TFL));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TFR));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TFC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TBL));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TBR));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_TBC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_BLOC));
  paramVector.push_back(CreateFloatParameter(CSocketGainModeIDs, UpdateGain_BROC));

  if (this->SetParameterVector(paramVector) <= 0)
  {
    // TODO error message
    return -1;
  }

  // set default values
  float fVal = GAIN_DEFAULT;
  this->SetParameter(CSocketGainModeIDs::UpdateGain_FL,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_FR,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_FC,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_LFE,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_BL,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_BR,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_FLOC,  static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_FROC,  static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_BC,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_SL,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_SR,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TFL,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TFR,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TFC,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TC,    static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TBL,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TBR,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_TBC,   static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_BLOC,  static_cast<void*>(&fVal), sizeof(float));
  this->SetParameter(CSocketGainModeIDs::UpdateGain_BROC,  static_cast<void*>(&fVal), sizeof(float));

  if (!CGainModeModelMessages::Create(this))
  {
    // TODO error message
    return -1;
  }

  return 0;
}

void CGainModeModel::Destroy()
{
}

int CGainModeModel::RequestGain(Message &Msg)
{
  if (Msg.signal < CSocketGainModeIDs::RequestGain_FL || Msg.signal > CSocketGainModeIDs::RequestGain_BROC)
  {
    return -1;
  }

  float fVal;
  this->GetParameter(Msg.signal, static_cast<void*>(&fVal), sizeof(float));
  if (!this->SendMsg(static_cast<void*>(&fVal), sizeof(float), Msg.signal - AE_DSP_CH_MAX))
  {
    return -1;
  }

  return 0;
}

