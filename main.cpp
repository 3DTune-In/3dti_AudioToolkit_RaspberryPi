/**
* \class line_out
*
* \brief main program that play a couple of tones.
* \date	december 2019
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
* \b Copyright: University of Malaga - 2019
*
* \b Licence: This copy of 3dti_AudioToolkit is licensed to you under the terms described in the 3DTI_AUDIOTOOLKIT_LICENSE file included in this distribution.
*
* \b Acknowledgement: This project has received funding from the European Union's Horizon 2020 research and innovation programme under grant agreement No 644051
*/

/*! \file */

#include <stdio.h>
#include <math.h>
#include "./lineOut/portaudio.h"
#include "./lineOut/lineOut.hpp"
#include<vector> // for array, at()
#include <iostream>
#define NUM_SECONDS   	(1) 		//For each tone.
#define SAMPLE_RATE   	(44100)	//22050) //(44100)
#define FRAMES_PER_BUFFER  	(1024)


using namespace line_out_namespace;

/*******************************************************************/

int main(void);

/*******************************************************************/

int main(void)
{
    ScopedPaHandler paInit;
    if(paInit.result() != paNoError) cout << "ERROR INICIANDO PORTAUDIO";
    printf("creator\n");
    CLineOut TestLine;
    printf("setup\n");
    cout << "Dispositivo principal numero " << Pa_GetDefaultOutputDevice() << endl;
    if(!TestLine.setup(Pa_GetDefaultOutputDevice(), FRAMES_PER_BUFFER, SAMPLE_RATE)) cout << "Error 2" << endl;
    printf("autotest\n");
    if(!TestLine.autoTest()) cout << "Error 3" << endl;
    printf("Test finished.\n");
    return 0;
}
