/**
* \start date  december 2019
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

#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>

#include "./src/thirdPartyLibs/loguru/loguru.cpp"
#include "./src/portaudio.h"
#include "./src/lineOut.hpp"
#include "./src/soundSource.hpp"
#include "./src/soundFile.hpp"

using namespace line_out_namespace;

#define NUM_SECONDS    (1)     //For each tone.
const char LOG_FOLDER[20] = "./general.log";

string WAV_PATH_1 = "./resources/3DTI_Sample_44.1kHz_MusicJazzPiano.wav";
string WAV_PATH_2 = "./resources/3DTI_Sample_44.1kHz_MusicJazzBass.wav";
string WAV_PATH_3 = "./resources/3DTI_Sample_44.1kHz_MusicJazzDrum.wav";
string WAV_PATH_4 = "./resources/3DTI_Sample_44.1kHz_MusicJazzGuitar.wav";

const int TAM = 4;
string WAV_PATHS[TAM] = {WAV_PATH_1, WAV_PATH_2, WAV_PATH_3, WAV_PATH_4};
int iNumChannels;
int iWAVSampleRate;
int iTotalNumSamples;
int iActualFrame=0;
int iFramesPerBuffer = 512;//default value
bool bLoopMode = false;//default value
CSoundFile audioFile[TAM];


/*******************************************************************/
static int mainCallback( const void *inputBuffer, void *outputBuffer,
            unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,void *userData );

int main(int argc, char* argv[]);

/*******************************************************************/

int main(int argc, char* argv[])
{
  loguru::init(argc,argv);
  // Put every log message in "everything.log":
  loguru::add_file(LOG_FOLDER, loguru::Append, loguru::Verbosity_MAX);
  for(int actualElement = 0; actualElement < TAM; actualElement++){
    audioFile[actualElement].setup(WAV_PATHS[actualElement]);
    iWAVSampleRate = audioFile[actualElement].getSampleRate();
    iNumChannels = audioFile[actualElement].getNumChannels();
    iTotalNumSamples = audioFile[actualElement].getFileLength();
  }


  if(iNumChannels > 2) iNumChannels = 2;
  LOG_F(INFO,"Abriendo archivo wav con %d canales y %d de sampleRate.", iNumChannels, iWAVSampleRate);   
  iNumChannels = 2; 
  CLineOut TestLine;
  if(TestLine.result() != paNoError) {
    LOG_F(ERROR,"ERROR : No se ha podido iniciar portaudio");
    exit(1);
  }
  LOG_F(2, "Configurando la salida de audio.");
  do{
    LOG_F(INFO, "Por favor, introduzca los frames per buffer deseados : ");
    cin >> iFramesPerBuffer;
  }while(iFramesPerBuffer<=0);
  cin.ignore();
  char inputChar;
  do{
    LOG_F(INFO, "Quiere habilitar el modo loop?(y/n) : ");
    cin >> inputChar;
  }while(inputChar != 'y' && inputChar !='n');
  cin.ignore();
  if(inputChar=='y') bLoopMode=true;
  else bLoopMode=false;
  for(int actualElement=0; actualElement<TAM; actualElement++){
    audioFile[actualElement].setLoop(bLoopMode);
  }
  int outputSampleRate;
  const PaDeviceInfo *deviceInfo;
  deviceInfo = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());
  do{
    LOG_F(INFO,"Introduzca el SampleRate del dispositivo %s", deviceInfo->name);
    if(deviceInfo->defaultSampleRate != 0) LOG_F(INFO, "el valor por defecto es: %d",deviceInfo->defaultSampleRate);
    cin >> outputSampleRate;
  }while(outputSampleRate<=0);
  if(iWAVSampleRate != outputSampleRate) LOG_F(WARNING, "Different sample rate between wav file and default device");
  if(!TestLine.defaultSetup(Pa_GetDefaultOutputDevice(), iFramesPerBuffer, outputSampleRate, iNumChannels)){
    LOG_F(ERROR,"ERROR : El setup no ha ido bien");
    exit(1);
  }
  LOG_F(2,"Empezando el autotest.");
  if(!TestLine.autoTest()) LOG_F(ERROR,"ERROR : Autotest falló.");
  LOG_F(INFO,"Saliendo del programa.");
    
  if(!TestLine.setup(Pa_GetDefaultOutputDevice(), iFramesPerBuffer, outputSampleRate, iNumChannels, *mainCallback)){
    LOG_F(ERROR,"ERROR : El setup no ha ido bien");
    exit(1);
  }
  LOG_F(2,"started wav setup");

  TestLine.start();
  LOG_F(INFO,"play for %d seconds",15);
  Pa_Sleep( 15 * 1000 );
  TestLine.pause();
  TestLine.close();
  return 0;
}

int mainCallbackMethod(const void *__inputBuffer, void *__outputBuffer,
      unsigned long __framesPerBuffer, const PaStreamCallbackTimeInfo* __timeInfo,
      PaStreamCallbackFlags __statusFlags)
{
  float * fpOut = (float*)__outputBuffer;
  (void) __timeInfo; /* Prevent unused variable warnings. */
  (void) __statusFlags;
  (void) __inputBuffer;
  float fResult=0;
  //THERE IS A __framesPerBuffer PER CHANNEL!!!
  //fpOut READS __framesPerBuffer*iNumberOfChannels floats per callback!!!
  for(unsigned int uiCount=0; uiCount<__framesPerBuffer;uiCount++){
    for(int iActualChannel = 0; iActualChannel<iNumChannels; iActualChannel++){
      for(int actualFile=0; actualFile<TAM; actualFile++){
        if(iActualChannel < iNumChannels/2 && actualFile < TAM/2){
          fResult+=audioFile[actualFile].getFrame();
        }
        if(iActualChannel >= iNumChannels/2 && actualFile >= TAM/2){
          fResult+=audioFile[actualFile].getFrame();
        }
      }
      *fpOut++= fResult;
      fResult=0;
    }//for ends iActualChannel
  }//for ends frames per buffer
  return paContinue;
}//paCallbackMethod ends

static int mainCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,void *userData )
{
  /* Here we cast userData to CLineOut* type so we can call the instance method 
  paCallbackMethod, we can do that since we called Pa_OpenStream with 'this' for userData */
  return mainCallbackMethod(inputBuffer, outputBuffer,
          framesPerBuffer,timeInfo,statusFlags);
}//paCallback ends
