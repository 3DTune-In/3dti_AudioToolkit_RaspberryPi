/**
* \class line_out
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

#ifndef LINE_OUT_H
#define LINE_OUT_H

#include<vector> // for array, at()
#include<tuple> // for get()
#include "portaudio.h"

using namespace std;

namespace line_out{
	class lineOut
		{
		public:
			//Start the lineOut stream with default configuration
			//A sin wave of 440Hz, with a 44100 size of SampleRate.
			lineOut();

			//Start the lineOut stream with external data. Needs to specify the sample_rate too.
			lineOut(vector <float> * rightData, vector <float> * leftData, int sample_rate);

			//Start the lineOut stream with external data. Needs to specify the sample_rate too and let the user to configure right and left phases.
			lineOut(vector <float> * rightData, vector <float> * leftData, int sample_rate, int leftPhase, int rightPhase);

			//Reconfigure the sin wave using new parameters filling the complete buffer again.
			//Can be used while running
	    		bool Sine(int sampleRate ,int LeftFrecuency, int RightFrecuency, int leftPhase, int rightPhase, PaStream*  Stream);

			//Reconfigure the sin wave of the left speaker using new frecuency filling the complete buffer again.
			//Can be used while running
	   		bool setFrecuencyLeft ( int frecLeft);

			//Reconfigure the sin wave of the right speaker using new frecuency filling the complete buffer again.
			//Can be used while running
	    		bool setFrecuencyRight( int frecRight);

			//Reconfigure the sin wave using a new sampleRate filling the complete buffer again.
			bool setSampleRate ( int sampleRate);

			//Open an lineOut device and configure it to be used with a "bufferSize"
			//User needs to execute this before "start" function.
	    		bool open(PaDeviceIndex index, int bufferSize);

			//Close the lineOut device of this lineOut variable.
	   		bool close();

			//Start the lineOut streaming.
	  		bool start();

			//Stop the lineOut streaming.
	 		bool stop();

	private:
	    /* The instance callback, where we have access to every method/variable in object of class Sine */
	    int iSampleRate;
	    int iLeftSenFrecuency;
	    int iRightSenFrecuency;
	    PaStream *stream;
	    vector <float> *lineOutLeftData;
	    vector <float> *lineOutRightData;
	    //float sine[];//iSampleRate];
	    int left_phase;
	    int right_phase;
	    char message[20];
	    
	    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer,
		 const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags)
	    {
		 float *out = (float*)outputBuffer;
		 unsigned long i;

		 (void) timeInfo; /* Prevent unused variable warnings. */
		 (void) statusFlags;
		 (void) inputBuffer;

		 for( i=0; i<framesPerBuffer; i++ )
		 {
		     *out++ = lineOutLeftData->at(left_phase);  /* left */
		     *out++ = lineOutRightData->at(right_phase);  /* right */
		     left_phase += 1;
		     if( left_phase >= iSampleRate ) left_phase -= iSampleRate;
		     right_phase += 1;
		     if( right_phase >= iSampleRate ) right_phase -= iSampleRate;
		 }

		 return paContinue;

	    }

	    /* This routine will be called by the PortlineOut engine when lineOut is needed.
	    ** It may called at interrupt level on some machines so don't do anything
	    ** that could mess up the system like calling malloc() or free().
	    */
	    static int paCallback( const void *inputBuffer, void *outputBuffer,
		 unsigned long framesPerBuffer,
		 const PaStreamCallbackTimeInfo* timeInfo,
		 PaStreamCallbackFlags statusFlags,
		 void *userData )
	    {
		 /* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
		    we called Pa_OpenStream with 'this' for userData */
		 return ((lineOut*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
		     framesPerBuffer,
		     timeInfo,
		     statusFlags);
	    }


	    void paStreamFinishedMethod()
	    {
		 printf( "Stream Completed: %s\n", message );
	    }
	    /*
	     * This routine is called by portlineOut when playback is done.
	     */
	    static void paStreamFinished(void* userData)
	    {
		 return ((lineOut*)userData)->paStreamFinishedMethod();
	    }
	    
	};

	class ScopedPaHandler
	{
		public:
		    ScopedPaHandler()
			 : _result(Pa_Initialize())
		    {
		    }
		    ~ScopedPaHandler()
		    {
			 if (_result == paNoError)
			 {
			     Pa_Terminate();
			 }
		    }

		    PaError result() const { return _result; }

		private:
		    PaError _result;
	};
}
#endif
