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

using namespace std;

#define DEFAULT_SAMPLING_RATE 44100
#define DEFAULT_TONE_FRECUENCY 440

namespace line_out_namespace{
	//Start the lineOut stream with default configuration
	//A sin wave of 440Hz, with a 44100 size of SampleRate.
	CLineOut::CLineOut() : stream(0){}

	bool CLineOut::setup(PaDeviceIndex __index, int __iBufferSize, int __iSampleRate):iSampleRate(__iSampleRate), iBufferSize(__iBufferSize), {
		PaStreamParameters outputParameters;
		outputParameters.device = index;
		if (outputParameters.device == paNoDevice) return false;//device not found
		const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
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
															 &lineOut::paCallback,
															 this            /* Using 'this' for userData so we can cast to lineOut* in paCallback method */
															 );

		if (err != paNoError) return false;	 /* Failed to open stream to device !!! */

		err = Pa_SetStreamFinishedCallback( stream, &lineOut::paStreamFinished );
		if (err != paNoError)
		{
			  Pa_CloseStream( stream );
			  stream = 0;
	  		return false;
		}
		return true;
	}//setup ends

	//Close the lineOut device of this lineOut variable.
	bool CLineOut::close()
	{
	    if (stream == 0) return false;

	    PaError err = Pa_CloseStream( stream );
	    stream = 0;
	    return (err == paNoError);
	}

	//Start the lineOut streaming.
	bool CLineOut::start()
	{
	    if (stream == 0) return false;
	    PaError err = Pa_StartStream( stream );
	    return (err == paNoError);
	}

	//Stop the lineOut streaming.
	bool CLineOut::stop()
	{
	    if (stream == 0) return false;
	    PaError err = Pa_StopStream( stream );
	    return (err == paNoError);
	}

	bool CLineOut::autoTest(){
    for( int i=0; i<iSampleRate; i++ )
    {
        (audioLeftData).push_back( (float) sin( ((double)i/(double)iSampleRate) *DEFAULT_TONE_FRECUENCY* M_PI * 2. ));
        (audioRightData).push_back(audioLeftData[i]);
    }
		CLineAudio::start();
		Pa_Sleep( NUM_SECONDS * 1000 );
		CLineAudio::stop();
		CLineAudio::close();
	}
}//CLineOut ends