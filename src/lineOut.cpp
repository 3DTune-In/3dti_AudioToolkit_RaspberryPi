/**
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

#ifndef NUMBER_PI
#define NUMBER_PI  (3.14159265)
#endif
#include<vector> // for array, at()
#include<tuple> // for get()
#include "lineOut.hpp"
#include <math.h>
#include "portaudio.h"
#include <iostream>
using namespace std;

#define DEFAULT_TONE_FRECUENCY 880
#define NUM_SECONDS 1

namespace line_out_namespace{
	//Start the lineOut stream with default configuration
	//A sin wave of 440Hz, with a 44100 size of SampleRate.
	CLineOut::CLineOut() : stream(0){
	    lineOutLeftData = &leftData;
	    lineOutRightData = &rightData;
	}

	bool CLineOut::setup(PaDeviceIndex __index, int __iBufferSize, int __iSampleRate){
		CLineOut::iSampleRate =(__iSampleRate);
		CLineOut::iBufferSize =(__iBufferSize);
    for(int count=0; count < __iSampleRate; count++){
    	leftData.push_back(0);
    	rightData.push_back(0);
    }
		
		PaStreamParameters outputParameters;
		outputParameters.device = __index;
		if (outputParameters.device == paNoDevice){
		 cout << "aquí";
		 return false;//device not found
		}
		const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(__index);
		if (pInfo != 0) printf("Output device name: '%s'\r", pInfo->name);

		outputParameters.channelCount = 2;       /* stereo output */
		outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		PaError err = Pa_OpenStream(
															 &stream,
															 NULL, /* no input */
															 &outputParameters,
															 __iSampleRate,
															 __iBufferSize,
															 paClipOff,      /* we won't output out of range samples so don't bother clipping them */
															 &CLineOut::paCallback,
															 this            /* Using 'this' for userData so we can cast to lineOut* in paCallback method */
															 );

		if (err != paNoError) return false;	 /* Failed to open stream to device !!! */

		err = Pa_SetStreamFinishedCallback( CLineOut::stream, &CLineOut::paStreamFinished );
		if (err != paNoError)
		{
			  Pa_CloseStream( CLineOut::stream );
			  CLineOut::stream = 0;
	  		return false;
		}
		return true;
	}//setup ends

	//Close the lineOut device of this lineOut variable.
	bool CLineOut::close()
	{
	    if (CLineOut::stream == 0) return false;

	    PaError err = Pa_CloseStream( CLineOut::stream );
	    CLineOut::stream = 0;
	    return (err == paNoError);
	}

	//Start the lineOut streaming.
	bool CLineOut::start()
	{
	    if (CLineOut::stream == 0) return false;
	    PaError err = Pa_StartStream( CLineOut::stream );
	    return (err == paNoError);
	}

	//Stop the lineOut streaming.
	bool CLineOut::stop()
	{
	    if (CLineOut::stream == 0) return false;
	    PaError err = Pa_StopStream( CLineOut::stream );
	    return (err == paNoError);
	}

	bool CLineOut::autoTest(){
	  /*(CLineOut::lineOutLeftData)->reserve(CLineOut::iBufferSize*sizeof(float));
	  (CLineOut::lineOutRightData)->reserve(CLineOut::iBufferSize*sizeof(float));*/
		printf("started\n");
		float fSine = 0;
    for( unsigned int count=0; count<iSampleRate; count++ )
    {
    		fSine = (float) sin( ((double)count/(double)iSampleRate) *DEFAULT_TONE_FRECUENCY* NUMBER_PI * 2. );
        rightData[count]=(fSine);
        //cout << rightData[i] << endl;
        leftData[count] =(fSine);       
    }
    printf("data created\n");
		CLineOut::start();
		
    printf("playing\n");   
		Pa_Sleep( NUM_SECONDS * 1000 );
		CLineOut::stop();
		printf("stopped\n");
		CLineOut::close();
		printf("closed\n");
		return true;
	}
}//CLineOut ends
