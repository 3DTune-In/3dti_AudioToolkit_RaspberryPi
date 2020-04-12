/**
* \start date	december 2019
* \class line_out
*
* \brief Declaration of class line_out
* \date	April 2020
*
* \authors: Gonzalo Alfonso Bueno Santana
* \b Contact : gonzupi6@gmail.com
* Coordinated by , A. Reyes-Lecuona (University of Malaga)
* \b Contact: areyes@uma.es
*
* \b Contributions: (additional authors/contributors can be added here)
*
* \b Project: 3DTI (3D-games for TUNing and lEarnINg about hearing aids) ||
* \b Website: http://3d-tune-in.eu/
*
* \b Copyright: University of Malaga - 2020
*/

/*! \file */

#include "./src/thirdPartyLibs/loguru/loguru.cpp"
//#include "./src/thirdPartyLibs/loguru/loguru.hpp"
#include "./src/portaudio.h"
#include "./src/lineOut.hpp"

#define NUM_SECONDS       	(1) 		//For each tone.
#define SAMPLE_RATE       	(44100)	//22050) //(44100)
#define FRAMES_PER_BUFFER  	(1024)
#define NUM_CHANNELS        (2)

#include <stdio.h>
#include <math.h>
#include<vector> // for array, at()
#include <iostream>

using namespace line_out_namespace;

/*******************************************************************/

int main(int argc, char* argv[]);

/*******************************************************************/


int main(int argc, char* argv[]){
		loguru::init(argc,argv);
		// Put every log message in "everything.log":
	  loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
	  
    ScopedPaHandler paInit;
    if(paInit.result() != paNoError) LOG_F(ERROR,"ERROR : No se ha podido iniciar portaudio");
    
    CLineOut TestLine;
    LOG_F(2, "Configurando la salida de audio.");
    //cout << "Dispositivo principal numero " << Pa_GetDefaultOutputDevice() << endl;
    if(!TestLine.setup(Pa_GetDefaultOutputDevice(), FRAMES_PER_BUFFER, SAMPLE_RATE, NUM_CHANNELS)){
       LOG_F(ERROR,"ERROR : El setup no ha ido bien");
       exit(1);
    }
    LOG_F(2,"Empezando el autotest.");
    if(!TestLine.autoTest()) LOG_F(ERROR,"ERROR : Autotest falló.");
    LOG_F(INFO,"Saliendo del programa.");

    return 0;
}














