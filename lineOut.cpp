/**
* \class line_out implementation
*
* \brief Declaration of class line_out
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

#ifndef M_PI
#define M_PI  (3.14159265)
#endif
#include<vector> // for array, at()
#include<tuple> // for get()
#include "lineOut.hpp"
#include <math.h>
#include "portaudio.h"

using namespace std;

#define DEFAULT_SAMPLING_RATE 44100
#define DEFAULT_TONE_FRECUENCY 440
#define DEFAULT_RIGHT_PHASE 0
#define DEFAULT_LEFT_PHASE 0

namespace line_out{
	//Start the lineOut stream with default configuration
	//A sin wave of 440Hz, with a 44100 size of SampleRate.
	lineOut::lineOut() : stream(0), left_phase(DEFAULT_LEFT_PHASE), right_phase(DEFAULT_RIGHT_PHASE), iSampleRate(DEFAULT_SAMPLING_RATE), iLeftSenFrecuency(DEFAULT_TONE_FRECUENCY), iRightSenFrecuency(DEFAULT_TONE_FRECUENCY)
	{
	   /* initialise sinusoidal wavetable */
	   for( int i=0; i<iSampleRate; i++ )
	   {
	     lineOutLeftData->push_back ((float) sin( ((double)i/(double)iSampleRate) *iLeftSenFrecuency* M_PI * 2.));
	     lineOutRightData->push_back((float) sin( ((double)i/(double)iSampleRate) *iRightSenFrecuency*M_PI * 2.));
	   }
	}

	//Start the lineOut stream with external data. Needs to specify the sample_rate too.
	lineOut::lineOut(vector <float> * rightData, vector <float> * leftData, int sample_rate): stream(0), left_phase(DEFAULT_LEFT_PHASE), right_phase(DEFAULT_RIGHT_PHASE), iSampleRate(sample_rate), iLeftSenFrecuency(DEFAULT_TONE_FRECUENCY), iRightSenFrecuency(DEFAULT_TONE_FRECUENCY)
	{
	  lineOutLeftData = leftData;
	  lineOutRightData = rightData;
	}

	//Start the lineOut stream with external data. Needs to specify the sample_rate too and let the user to configure right and left phases.
	lineOut::lineOut(vector <float> * rightData, vector <float> * leftData, int sample_rate, int leftPhase, int rightPhase): stream(0), left_phase(leftPhase), right_phase(rightPhase), iSampleRate(sample_rate), iLeftSenFrecuency(DEFAULT_TONE_FRECUENCY), iRightSenFrecuency(DEFAULT_TONE_FRECUENCY)
	{
	  lineOutLeftData = leftData;
	  lineOutRightData = rightData;
	}

	//Reconfigure the sin wave using new parameters filling the complete buffer again.
	//Can be used while running
	bool lineOut::Sine(int sampleRate ,int LeftFrecuency, int RightFrecuency, int leftPhase, int rightPhase, PaStream*  Stream)
	{
	  stream=Stream;
	  iSampleRate=sampleRate;
	  iLeftSenFrecuency=LeftFrecuency;
	  iRightSenFrecuency=RightFrecuency;
	  left_phase= int (((leftPhase)% (360))%(sampleRate));
	  right_phase=int (((rightPhase)% (360))%(sampleRate));

	    /* initialise sinusoidal wavetable */
	    for( int i=0; i<iSampleRate; i++ )
	    {

		 lineOutLeftData->push_back( (float) sin( ((double)i/(double)iSampleRate) *iLeftSenFrecuency* M_PI * 2. ));
		 lineOutRightData->push_back((float) sin( ((double)i/(double)iSampleRate) *iRightSenFrecuency*M_PI * 2.));
	    }
	    return true;
	    //sprintf( message, "No Message" );
	}

	//Reconfigure the sin wave of the left speaker using new frecuency filling the complete buffer again.
	//Can be used while running
	bool lineOut::setFrecuencyLeft ( int frecLeft)
	{
	  if(frecLeft>0){
	    iLeftSenFrecuency = frecLeft;
	    lineOutLeftData->erase(lineOutLeftData->begin(), lineOutLeftData->end());
	  for( int i=0; i<iSampleRate; i++ )
	  {
	    lineOutLeftData->push_back( (float) sin( ((double)i/(double)iSampleRate) *iLeftSenFrecuency* M_PI * 2. ));
	  }
	    return true;
	  }else{
	    return false;
	  }
	}

	//Reconfigure the sin wave of the right speaker using new frecuency filling the complete buffer again.
	//Can be used while running
	bool lineOut::setFrecuencyRight( int frecRight)
	{
	  if(frecRight>0){
	    iRightSenFrecuency = frecRight;
	    lineOutRightData->erase(lineOutRightData->begin(), lineOutRightData->end());
	  for( int i=0; i<iSampleRate; i++ )
	  {
	    lineOutRightData->push_back( (float) sin( ((double)i/(double)iSampleRate) *iRightSenFrecuency* M_PI * 2. ));
	  }
	    return true;
	  }else{
	    return false;
	  }

	}

	//Reconfigure the sin wave using a new sampleRate filling the complete buffer again.
	bool lineOut::setSampleRate ( int sampleRate)
	{
	  if(sampleRate>0){
	    iSampleRate = sampleRate;
	    lineOutRightData->erase(lineOutRightData->begin(), lineOutRightData->end());
	    lineOutLeftData->erase( lineOutLeftData->begin() , lineOutLeftData->end());
	    for( int i=0; i<iSampleRate; i++ )
	    {
	      lineOutLeftData->push_back(  (float) sin( ((double)i/(double)iSampleRate) *iLeftSenFrecuency * M_PI * 2. ));
	      lineOutRightData->push_back( (float) sin( ((double)i/(double)iSampleRate) *iRightSenFrecuency* M_PI * 2. ));
	    }
	    return true;
	  }else{
	    return false;
	  }
	}

	//Open an lineOut device and configure it to be used with a "bufferSize"
	//User needs to execute this before "start" function.
	bool lineOut::open(PaDeviceIndex index, int bufferSize)
	{
	    PaStreamParameters outputParameters;

	    outputParameters.device = index;
	    if (outputParameters.device == paNoDevice) {
		 return false;
	    }

	    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
	    if (pInfo != 0)
	    {
		 printf("Output device name: '%s'\r", pInfo->name);
	    }

	    outputParameters.channelCount = 2;       /* stereo output */
	    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	    outputParameters.hostApiSpecificStreamInfo = NULL;

	    PaError err = Pa_OpenStream(
		 &stream,
		 NULL, /* no input */
		 &outputParameters,
		 iSampleRate,
		 bufferSize,
		 paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		 &lineOut::paCallback,
		 this            /* Using 'this' for userData so we can cast to lineOut* in paCallback method */
		 );

	    if (err != paNoError)
	    {
		 /* Failed to open stream to device !!! */
		 return false;
	    }

	    err = Pa_SetStreamFinishedCallback( stream, &lineOut::paStreamFinished );

	    if (err != paNoError)
	    {
		 Pa_CloseStream( stream );
		 stream = 0;

		 return false;
	    }
	    return true;
	}

	//Close the lineOut device of this lineOut variable.
	bool lineOut::close()
	{
	    if (stream == 0)
		 return false;

	    PaError err = Pa_CloseStream( stream );
	    stream = 0;

	    return (err == paNoError);
	}

	//Start the lineOut streaming.
	bool lineOut::start()
	{
	    if (stream == 0)
		 return false;

	    PaError err = Pa_StartStream( stream );

	    return (err == paNoError);
	}

	//Stop the lineOut streaming.
	bool lineOut::stop()
	{
	    if (stream == 0)
		 return false;

	    PaError err = Pa_StopStream( stream );

	    return (err == paNoError);
	}
}
