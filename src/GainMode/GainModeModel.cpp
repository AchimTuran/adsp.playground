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
#include "GainMode/GainModeModel.hpp"
#include "EnumStrIDs.hpp"


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

  paramVector.push_back(CreateTParameter(float, CSocketGainModeIDs, UpdateMainGain));

  if (this->SetParameterVector(paramVector) <= 0)
  {
    // TODO error message
    return -1;
  }

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

int CGainModeModel::RequestMainGain(Message &Msg)
{
  if (!this->SendMsg(static_cast<float*>(&m_MainGain), sizeof(float), CSocketGainModeIDs::UpdateMainGain))
  {
    return -1;
  }

  return 0;
}

