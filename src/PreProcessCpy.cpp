#include "PreProcessCpy.hpp"


CPreProcessingCpy::CPreProcessingCpy()
{
}


CPreProcessingCpy::~CPreProcessingCpy()
{
}


AE_DSP_ERROR CPreProcessingCpy::ModeCreate(const AE_DSP_SETTINGS &settings, const AE_DSP_STREAM_PROPERTIES &pProperties)
{
  return AE_DSP_ERROR_NO_ERROR;
}


AE_DSP_ERROR CPreProcessingCpy::ModeDestroy()
{
  return AE_DSP_ERROR_NO_ERROR;
}


// Requiered Processing Methods
unsigned int CPreProcessingCpy::ModeProcess(float **Array_in, float **Array_out, unsigned int Samples)
{
  return Samples;
}
