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

#ifndef SOUND_FILE_H
#define SOUND_FILE_H
#endif
#include <string>
#include<vector> // for array, at()
#include <iostream>
#include <mutex>
#include "./thirdPartyLibs/AudioFile/AudioFile.h"
using namespace std;

namespace sound_file_namespace{
  class CSoundFile
    {
      public:
        //Setup the stream file, by default, loop mode is enabled and just select channel 0
        CSoundFile();
        bool setup(string __sFilePath,vector<float> __vfChannelWeight, bool __bLoopMode);
        bool setup(string __sFilePath, vector<float> __vfChannelWeight);
        bool setup(string __sFilePath);

        bool setLoop(bool __bLoopMode);
        bool setActualSample(int __iSampleNumber);
        bool setActualTime(float __fTime);//respecto al sampleRate del archivo.
        bool setChannelWeight( vector<float> __vfChannelWeight);

        int getFileLength();
        float getTimeFileLenght();
        int getActuaalSampleNumber();
        //VIRTUALES Y HEREDADOS
        float getFrame();
        int getSampleRate();
        bool play();
        bool pause();
        bool stop();

      //Public ends
      //////////////////////////////////////////////////////////////////
      private:
        bool bPlay;
        AudioFile<double> audioFile;
        int iNumChannelsInFile;
        int iNumSamplesPerchannelInFile;
        int iSampleRateInFile;
        int iActualSample;
        string sFilePath;
        vector<float> vfChannelWeight;
        bool bLoopMode;
        mutex mtx_getFrame;//pensando en la callback, que es de otra hebra.. y en el interfaz de contro, que es asíncono.
    };//class ends
}//namespace ends