/**
* \start date	december 2019
* \class line_out
*
* \brief Declaration of class line_out
* \date	April 2020
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


#include "./src/thirdPartyLibs/loguru/loguru.cpp"
#include "./src/portaudio.h"
#include "./src/lineOut.hpp"
#include "src/thirdPartyLibs/AudioFile/AudioFile.h"

#define NUM_SECONDS       	(1) 		//For each tone.
const char LOG_FOLDER[20] = "./general.log";
const char WAV_PATH[120] = "./src/thirdPartyLibs/AudioFile/tests/AudioFileTests/test-audio/wav_stereo_24bit_44100.wav";
AudioFile<double> audioFile;
int iNumChannels;
int iWAVSampleRate;
int iTotalNumSamples;
int iActualFrame=0;
int iFramesPerBuffer = 512;//default value
bool bLoopMode = false;
#include <stdio.h>
#include <math.h>
#include<vector>
#include <iostream>

using namespace line_out_namespace;

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
	  
	audioFile.load (WAV_PATH);
	iWAVSampleRate = audioFile.getSampleRate();
	iNumChannels = audioFile.getNumChannels();
	iTotalNumSamples = audioFile.getNumSamplesPerChannel();
		
	if(iNumChannels > 2) iNumChannels = 2;
	LOG_F(INFO,"Abriendo archivo wav con %d canales y %d de sampleRate.", iNumChannels, iWAVSampleRate);	  
	CLineOut TestLine;
	if(TestLine.result() != paNoError) {
		LOG_F(ERROR,"ERROR : No se ha podido iniciar portaudio");
		exit(1);
	}
	LOG_F(2, "Configurando la salida de audio.");
	do{
		LOG_F(INFO, "Por favor, introduzca los frames per buffer deseados :\n");
		cin >> iFramesPerBuffer;
	}while(iFramesPerBuffer<=0);
	cin.ignore();

	char inputChar;
	do{
		LOG_F(INFO, "Quiere habilitar el modo loop?(y/n) :\n");
		cin >> inputChar;
	}while(inputChar != 'y' && inputChar !='n');
	cin.ignore();
	if(inputChar=='y') bLoopMode=true;
	else bLoopMode=false;

	if(!TestLine.defaultSetup(Pa_GetDefaultOutputDevice(), iFramesPerBuffer, iWAVSampleRate, iNumChannels)){
		LOG_F(ERROR,"ERROR : El setup no ha ido bien");
		exit(1);
	}
	LOG_F(2,"Empezando el autotest.");
	if(!TestLine.autoTest()) LOG_F(ERROR,"ERROR : Autotest falló.");
	LOG_F(INFO,"Saliendo del programa.");
		
	if(!TestLine.setup(Pa_GetDefaultOutputDevice(), iFramesPerBuffer, iWAVSampleRate, iNumChannels, *mainCallback)){
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
	int iActualChannel;
	//THERE IS A __framesPerBuffer PER CHANNEL!!!
	//fpOut READS __framesPerBuffer*iNumberOfChannels floats per callback!!!
	for(unsigned int uiCount=0; uiCount<__framesPerBuffer;uiCount++){
		for(iActualChannel = 0; iActualChannel<iNumChannels; iActualChannel++){
			if(iActualFrame < iTotalNumSamples){
				*fpOut++= audioFile.samples[iActualChannel][iActualFrame];  /* left */
			}else{
				if(bLoopMode){
					iActualFrame = 0;
				  LOG_F(INFO, "Restarting WAV file");
					*fpOut++= audioFile.samples[iActualChannel][iActualFrame];  /* left */
				}else{
					*fpOut++=0;
				}	
			} 
		}//for ends iActualChannel
		if(iActualFrame < iTotalNumSamples) iActualFrame++;
	}//for ends frames per buffer
	return paContinue;
}//paCallbackMethod ends

static int mainCallback( const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,void *userData )
{
	/* Here we cast userData to CLineOut* type so we can call the instance method paCallbackMethod, we can do that since
	we called Pa_OpenStream with 'this' for userData */
	return mainCallbackMethod(inputBuffer, outputBuffer,
					framesPerBuffer,timeInfo,statusFlags);
}//paCallback ends
