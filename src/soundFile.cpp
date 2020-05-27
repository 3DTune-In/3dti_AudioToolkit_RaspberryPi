/**
* \start date  december 2019
* \class line_out
*
* \brief Declaration of class line_out
* \date  May 2020
*
* \authors: Gonzalo Alfonso Bueno Santana
* \b Contact : gonzupi6@gmail.com
* Supervised by , A. Reyes-Lecuona (University of Malaga)
* \b Contact: areyes@uma.es
*
* \b Contributions: (additional authors/contributors can be added here)
*
* \b Project: 3dti_AudioToolkit_RaspberryPi is a deployment of the 3D Tune-In Toolkit (https://github.com/3DTune-In/3dti_AudioToolkit)
* in Raspberry Pi based devices. It is developed by Gonzalo Alfonso bueno Santana as his Bachelor Thesis in the BEng Electronic Engineering Degree
* at the University of Malaga, School of Telecommunication (http://etsit.uma.es/)
* For a description of the 3D Tune-In Toolkit, see:
* Cuevas-Rodríguez M, Picinali L, González-Toledo D, Garre C, de la Rubia-Cuestas E, Molina-Tanco L and Reyes-Lecuona A. (2019)
* 3D Tune-In Toolkit: An open-source library for real-time binaural spatialisation. PLOS ONE 14(3): e0211899.
* https://doi.org/10.1371/journal.pone.0211899
*
* \b Website: https://github.com/3DTune-In/3dti_AudioToolkit_RaspberryPi
*
* \b Copyright: University of Malaga - 2020
*/

/*! \file */

#include "./soundFile.hpp"

using namespace std;

CSoundFile::CSoundFile(){}

bool CSoundFile::setup(string __sFilePath, vector<float>  __vfChannelWeight, bool __bLoopMode){
  bPlay= true;
  iActualSample=0;
  audioFile.load (__sFilePath);
  sFilePath = __sFilePath;
  iSampleRate = audioFile.getSampleRate();
  iNumChannelsInFile = audioFile.getNumChannels();
  iNumSamplesPerchannelInFile = audioFile.getNumSamplesPerChannel();
  vfChannelWeight = __vfChannelWeight;
  bLoopMode = __bLoopMode;
}//setup(sFilePath, __vfChannelWeight, __bLoopMode) ends

bool CSoundFile::setup(string __sFilePath, vector<float>  __vfChannelWeight){
  bPlay= true;
  iActualSample=0;
  audioFile.load (__sFilePath);
  sFilePath = __sFilePath;
  iSampleRate = audioFile.getSampleRate();
  iNumChannelsInFile = audioFile.getNumChannels();
  iNumSamplesPerchannelInFile = audioFile.getNumSamplesPerChannel();
  vfChannelWeight = __vfChannelWeight;
  bLoopMode = true;
}//setup(sFilePath, __vfChannelWeight) ends

bool CSoundFile::setup(string __sFilePath){
  bPlay= true;
  iActualSample=0;
  audioFile.load (__sFilePath);
  sFilePath = __sFilePath;
  iSampleRate = audioFile.getSampleRate();
  iNumChannelsInFile = audioFile.getNumChannels();
  iNumSamplesPerchannelInFile = audioFile.getNumSamplesPerChannel();
  for(int iActualChannel=0; iActualChannel<iNumChannelsInFile; iActualChannel++){
    vfChannelWeight.push_back(0.0);
  }
  vfChannelWeight[0]=1.0;
  bLoopMode = true;
  return true;
}//setup(sFilePath) ends

bool CSoundFile::setLoop(bool __bLoopMode){
  bLoopMode=__bLoopMode;
  return true;
}

bool CSoundFile::setActualSample(int __iSampleNumber){
  mtx_getFrame.lock();//pensando en la callback, que es de otra hebra.. y en el interfaz de contro, que es asíncono.
  iActualSample=__iSampleNumber;
  mtx_getFrame.unlock();
  return true;
}

bool CSoundFile::setActualTime(float __fTime){
  if(__fTime*iSampleRate<iNumSamplesPerchannelInFile){
    mtx_getFrame.lock();//pensando en la callback, que es de otra hebra.. y en el interfaz de contro, que es asíncono.
    iActualSample=__fTime*iSampleRate;
    mtx_getFrame.unlock();
    return true;
  }else{
    return false;
  }
}//setActualTime ends

bool CSoundFile::setChannelWeight(vector<float> __vfChannelWeight){
  if(__vfChannelWeight.size()<iNumChannelsInFile){
    mtx_getFrame.lock();//pensando en la callback, que es de otra hebra y en el interfaz de control, que es asíncono.
    vfChannelWeight=__vfChannelWeight;
    mtx_getFrame.unlock();
    return true;
  }else{
    return false;
  }
}//setChannelWeight ends

int CSoundFile::getFileLength(){
  return iNumSamplesPerchannelInFile;
}

float CSoundFile::getTimeFileLenght(){
  return iNumSamplesPerchannelInFile*iSampleRate;
}

int CSoundFile::getActualSampleNumber(){
  return iActualSample;
}

float CSoundFile::getFrame(){
  if(bPlay){
    if(iActualSample < iNumSamplesPerchannelInFile){
      float result=0;
      mtx_getFrame.lock();
      for(int iActualChannel=0; iActualChannel<iNumChannelsInFile; iActualChannel++){
        result += audioFile.samples[iActualChannel][iActualSample]*vfChannelWeight[iActualChannel];
      }//for ends
      iActualSample++;
      mtx_getFrame.unlock();
      return result;
    }else{//if iActualSample > iNumSamplesPerchannelInFile
      if(bLoopMode){
        iActualSample=0;
        float result=0;
        mtx_getFrame.lock();
        for(int iActualChannel=0; iActualChannel<iNumChannelsInFile; iActualChannel++){
          result += audioFile.samples[iActualChannel][iActualSample]*vfChannelWeight[iActualChannel];
        }//for ends
        iActualSample++;
        mtx_getFrame.unlock();
        return result;
      }else{//if bloopMode false
        return 0.0;
      }
    }//else actualSample < iNumSamplesPerChannelInFile ends
  }else{//else bPlay
    return 0.0;
  }
}//getFrame ends

bool CSoundFile::stop(){
  bPlay = false;
  iActualSample=0;
}

int CSoundFile::getNumChannels(){
  return iNumChannelsInFile;
}
