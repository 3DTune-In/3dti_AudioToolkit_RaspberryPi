/**
* \start date  december 2019
* \class line_out
*
* \brief Declaration of class line_out
* \date  April 2020
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

#ifndef LINE_OUT_H
#define LINE_OUT_H

#include<vector> // for array, at()
#include<tuple> // for get()
#include "portaudio.h"
#include <iostream>
#include "./thirdPartyLibs/loguru/loguru.hpp"
using namespace std;

namespace line_out_namespace{
  class CLineOut
    {
    public:
      CLineOut();
      ~CLineOut();
      PaError result() const;
      //Configure the setup for the autotest config using default callback
      bool defaultSetup(PaDeviceIndex __index
        ,int __iBufferSize
        ,int __iSampleRate 
        ,int __iNumberOfChannels);
      bool setup(PaDeviceIndex __index
        ,int __iBufferSize
        ,int __iSampleRate
        ,int __iNumberOfChannels 
        ,int (*__paCallback)( const void *
          , void *
          ,unsigned long 
          ,const PaStreamCallbackTimeInfo*
          ,PaStreamCallbackFlags ,void * ));

      bool close();
      bool start();
       bool pause();
      // play a #A5 880Hz for one second, pause one second, 
      // then play a #A4 440Hz for another second.
      bool autoTest();  //Close the stream at finish;
      
      int getSampleRate();
      int getBufferSize();
      void getBufferDataAdress(vector <float> * * __data);
      void setBufferDataAdress(vector <float> * __data);
    //public ends
    /////////////////////////////////////////////////////////////////
    
    private:
      //Global variables for the class.
      unsigned int iSampleRate;
      unsigned int iBufferSize;
      int iNumberOfChannels;
      unsigned int iActualFrame = 0;
      char caMessage[20] = "Sound stop";
      PaStream *stream;
      vector <float> *vpfDataPointer;
      vector <float> vfData;
      PaError _result;
      // This callback is called when the system need to, 
      // the stream is opened and started.
      int paCallbackMethod(const void *__inputBuffer, void *__outputBuffer,
          unsigned long __framesPerBuffer,
          const PaStreamCallbackTimeInfo* __timeInfo,
          PaStreamCallbackFlags __statusFlags)
      {
        float * fpOut = (float*)__outputBuffer;
        (void) __timeInfo;     // Prevent unused variable warnings.
        (void) __statusFlags;
        (void) __inputBuffer;
        int iActualChannel;
        //THERE IS A __framesPerBuffer PER CHANNEL!!!
        //fpOut READS __framesPerBuffer*iNumberOfChannels floats per callback!!!
        for(unsigned int uiCount=0; uiCount<__framesPerBuffer;uiCount++){
          for(iActualChannel = 0; iActualChannel<iNumberOfChannels; iActualChannel++)
          {
            *fpOut++=(*vpfDataPointer)[iActualFrame]; 
            iActualFrame++;
          }//for ends
          if(iActualFrame >= iSampleRate*iNumberOfChannels) iActualFrame-=(iSampleRate*iNumberOfChannels);
        }//for ends
        return paContinue;
      }//paCallbackMethod ends

      /* This routine will be called by the PortlineOut engine when CLineOut is needed.
      ** It may called at interrupt level on some machines so don't do anything
      ** that could mess up the system like calling malloc() or free().
      */
      static int paCallback( const void *inputBuffer
              ,void *outputBuffer
              ,unsigned long framesPerBuffer
              ,const PaStreamCallbackTimeInfo* timeInfo
              ,PaStreamCallbackFlags statusFlags
              ,void *userData )
      {
        /* Here we cast userData to CLineOut* type so we can call the instance method paCallbackMethod, 
        we can do that since we called Pa_OpenStream with 'this' for userData */
        return ((CLineOut*)userData)->paCallbackMethod(inputBuffer
                        ,outputBuffer,framesPerBuffer
                        ,timeInfo,statusFlags);
      }//paCallback ends

      // This routine is called by portlineOut when playback is done.  
      void paStreamFinishedMethod()
      {
          LOG_F( INFO,"Stream Completed: %s", caMessage );
      }
      
      // This routine is called by portlineOut when playback is done.
      static void paStreamFinished(void* userData)
      {
          return ((CLineOut*)userData)->paStreamFinishedMethod();
      }
    //private ends
    /////////////////////////////////////////////////////////////////
  };//CLineOut ends
}//namespace ends
#endif
