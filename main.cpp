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
#include "./src/lineOut/portaudio.h"
#include "./src/lineOut/lineOut.hpp"

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

int main(void);

/*******************************************************************/

int main(void){
    ScopedPaHandler paInit;
    if(paInit.result() != paNoError) cout << "ERROR : No se ha podido iniciar portaudio";
    CLineOut TestLine;
    cout << "Dispositivo principal numero " << Pa_GetDefaultOutputDevice() << endl;
    if(!TestLine.setup(Pa_GetDefaultOutputDevice(), FRAMES_PER_BUFFER, SAMPLE_RATE, NUM_CHANNELS)){
       cout << "ERROR : El setup no ha ido bien" << endl;
       exit(1);
    }
    printf("Starting autotest\n");
    if(!TestLine.autoTest()) cout << "ERROR : Autotest fail..." << endl;
    printf("Program finished.\n");

    return 0;
}














