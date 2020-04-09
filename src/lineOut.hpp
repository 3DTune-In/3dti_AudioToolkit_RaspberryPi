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

#ifndef LINE_OUT_H
#define LINE_OUT_H

#include<vector> // for array, at()
#include<tuple> // for get()
#include "portaudio.h"
#include <iostream>
using namespace std;

namespace line_out_namespace{
	class CLineOut
		{
		public:
			//Start the lineOut stream with default configuration
			//A sin wave of 440Hz, with a 44100 size of SampleRate.
			CLineOut();
			//Open an lineOut device and configure it to be used with a "bufferSize"
			//User needs to execute this before "start" function
      bool setup(PaDeviceIndex __index, int __iBufferSize, int __iSampleRate);
			//Close the lineOut device of this lineOut variable.
	   	bool close();

			//Start the lineOut streaming.
	  	bool start();
			//Stop the lineOut streaming.
	 		bool stop();

			//play a #A4 440Hz for one second
			bool autoTest();
	private:
	    int iSampleRate;
	    int iBufferSize;
	    int iLeftSenFrecuency;
	    int iRightSenFrecuency;
	    PaStream *stream;
	    vector <float> *lineOutLeftData;
	    vector <float> *lineOutRightData;
	    vector <float> leftData;
	    vector <float> rightData;
	    int uiActualFrame = 0;
	    char caMessage[20];
			int ileftPhase= 0;
			int iRightPhase=0;
      int iFillCircleBuffer = 0;
      int iReadingCircleBuffer = 0;
	    int paCallbackMethod(const void *__inputBuffer, void *__outputBuffer,
					 unsigned long __framesPerBuffer,
					 const PaStreamCallbackTimeInfo* __timeInfo,
					 PaStreamCallbackFlags __statusFlags)
				    {
							 float * fpOut = (float*)__outputBuffer;
							 (void) __timeInfo; /* Prevent unused variable warnings. */
							 (void) __statusFlags;
							 (void) __inputBuffer;
								for(int countActual=0; countActual<__framesPerBuffer;countActual++){
										cout << "Frame actual " << uiActualFrame << " de valor " << (*lineOutLeftData)[uiActualFrame] << endl;
										if(uiActualFrame >= iSampleRate) uiActualFrame-=iSampleRate;
							     	*fpOut++=(*lineOutLeftData)[(uiActualFrame)];  /* left */
							   		*fpOut++=(*lineOutRightData)[(uiActualFrame)];  /* right */
										uiActualFrame++;
							 }
							 return paContinue;
	    }//callback ends

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
					 return ((CLineOut*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
																					     framesPerBuffer,
																					     timeInfo,
																					     statusFlags);
	    }


	    void paStreamFinishedMethod()
	    {
		 		printf( "Stream Completed: %s\n", caMessage );
	    }
	    /*
	     * This routine is called by portlineOut when playback is done.
	     */
	    static void paStreamFinished(void* userData)
	    {
		 		return ((CLineOut*)userData)->paStreamFinishedMethod();
	    }

	};//CLineOut ends
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
}//namespace ends
#endif
