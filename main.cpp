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
#define FRAMES_PER_BUFFER  	(512)
const char LOG_FOLDER[20] = "./general.log";
const char WAV_PATH[120] = "./src/thirdPartyLibs/AudioFile/tests/AudioFileTests/test-audio/wav_stereo_24bit_44100.wav";

#include <stdio.h>
#include <math.h>
#include<vector>
#include <iostream>

using namespace line_out_namespace;

/*******************************************************************/

int main(int argc, char* argv[]);

/*******************************************************************/


int main(int argc, char* argv[]){
		loguru::init(argc,argv);
		// Put every log message in "everything.log":
	  loguru::add_file(LOG_FOLDER, loguru::Append, loguru::Verbosity_MAX);
	  AudioFile<double> audioFile;
	  audioFile.load (WAV_PATH);
		int WAVSampleRate = audioFile.getSampleRate();
		int numChannels = audioFile.getNumChannels();
		if(numChannels > 2) numChannels = 2;
		LOG_F(INFO,"Abriendo archivo wav con %d canales y %d de sampleRate.", numChannels, WAVSampleRate);	  
	  
    ScopedPaHandler paInit;
    if(paInit.result() != paNoError) {
    	LOG_F(ERROR,"ERROR : No se ha podido iniciar portaudio");
    	exit(1);
    }
    CLineOut TestLine;
    LOG_F(2, "Configurando la salida de audio.");
    if(!TestLine.setup(Pa_GetDefaultOutputDevice(), FRAMES_PER_BUFFER, WAVSampleRate, numChannels)){
       LOG_F(ERROR,"ERROR : El setup no ha ido bien");
       exit(1);
    }
    LOG_F(2,"Empezando el autotest.");
    if(!TestLine.autoTest()) LOG_F(ERROR,"ERROR : Autotest falló.");
    LOG_F(INFO,"Saliendo del programa.");

    return 0;
}
