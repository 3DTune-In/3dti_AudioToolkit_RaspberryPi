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
#include "portaudio.h"
#include "lineOut.hpp"
#include<vector> // for array, at()

#define NUM_SECONDS   	(1) 		//For each tone.
#define SAMPLE_RATE   	(44100)	//22050) //(44100)
#define FRAMES_PER_BUFFER  	(62)
#define FRECUENCY    	(440)

#ifndef M_PI
#define M_PI  		(3.14159265)
#endif

using namespace line_out;

/*******************************************************************/

int main(void);

/*******************************************************************/

int main(void)
{
    vector <float> audioLeftData;
    vector <float> audioRightData;
    for( int i=0; i<SAMPLE_RATE; i++ )
    {
        (audioLeftData).push_back( (float) sin( ((double)i/(double)SAMPLE_RATE) *FRECUENCY* M_PI * 2. ));
        (audioRightData).push_back(audioLeftData[i]);
    }
    lineOut sonido(&audioLeftData, &audioRightData,SAMPLE_RATE ); //Modo seno.
    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
    ScopedPaHandler paInit;
    if( paInit.result() != paNoError ) goto error;
    if (sonido.open(Pa_GetDefaultOutputDevice(),FRAMES_PER_BUFFER))
    {
        if (sonido.start())
        {
            printf("Play for %d seconds.\n", NUM_SECONDS );
            Pa_Sleep( NUM_SECONDS * 1000 );
	     if(sonido.setFrecuencyRight(2*FRECUENCY)) Pa_Sleep( NUM_SECONDS * 1000 );
     	     if(sonido.setFrecuencyLeft(FRECUENCY/2)) Pa_Sleep( NUM_SECONDS * 1000 );;
            sonido.stop();
        }
        sonido.close();
    }
    printf("Test finished.\n");
    return paNoError;
error:
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", paInit.result() );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( paInit.result() ) );
    return 1;
}
