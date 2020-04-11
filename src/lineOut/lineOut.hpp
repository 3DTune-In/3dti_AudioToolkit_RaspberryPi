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
			CLineOut();
			//User needs to execute this before "start" function
      bool setup(PaDeviceIndex __index, int __iBufferSize, int __iSampleRate, int __iNumberOfChannels);
			//Close the lineOut device of this lineOut variable. If the class is closed you cant start or stop it.
	   	bool close();
			//Start the lineOut streaming.
	  	bool start();
			//Stop the lineOut streaming.
	 		bool stop();

			//play a #A5 880Hz for one second, stop one second and then, play a #A4 440Hz for another second.
			bool autoTest();

			int getSampleRate();
			int getBufferSize();
			void getBufferDataAdress(vector <float> * * __data);
			void setBufferDataAdress(vector <float> * __data);
			
	/*******************************************************************************************/
	
	private:
		//Global variables for the class.
	    unsigned int iSampleRate;
	    unsigned int iBufferSize;
		  int iNumberOfChannels;
			unsigned int iActualFrame = 0;
	    char caMessage[20];
	    PaStream *stream;
	    vector <float> *vpfDataPointer;
	    vector <float> *vpfLineOutRightData;
	    vector <float> vfData;
	    vector <float> vfRightData;

			//This callback is called when the system need to, the stram is opened and started.
	    int paCallbackMethod(const void *__inputBuffer, void *__outputBuffer,
					 unsigned long __framesPerBuffer,
					 const PaStreamCallbackTimeInfo* __timeInfo,
					 PaStreamCallbackFlags __statusFlags){
							 float * fpOut = (float*)__outputBuffer;
							 (void) __timeInfo; /* Prevent unused variable warnings. */
							 (void) __statusFlags;
							 (void) __inputBuffer;
							 int iActualChannel;
							 //THERE IS A __framesPerBuffer PER CHANNEL!!!
							 //fpOut READS __framesPerBuffer*iNumberOfChannels floats per callback!!!
								for(unsigned int uiCount=0; uiCount<__framesPerBuffer;uiCount++){
										cout << "Frame\t" << iActualFrame << "\tvalor\t" << ((*vpfDataPointer)[iActualFrame])<<endl ;	
										for(iActualChannel = 0; iActualChannel<iNumberOfChannels; iActualChannel++){										
											*fpOut++=(*vpfDataPointer)[iActualFrame];  /* left */
											iActualFrame++;
										}
										if(iActualFrame >= iSampleRate*iNumberOfChannels) iActualFrame-=(iSampleRate*iNumberOfChannels);				
							 }
							 return paContinue;
	    }//paCallbackMethod ends

	    /* This routine will be called by the PortlineOut engine when CLineOut is needed.
	    ** It may called at interrupt level on some machines so don't do anything
	    ** that could mess up the system like calling malloc() or free().
	    */
	    static int paCallback( const void *inputBuffer, void *outputBuffer,
														 unsigned long framesPerBuffer,
														 const PaStreamCallbackTimeInfo* timeInfo,
														 PaStreamCallbackFlags statusFlags,
														 void *userData )
	    {
					 /* Here we cast userData to CLineOut* type so we can call the instance method paCallbackMethod, we can do that since
					    we called Pa_OpenStream with 'this' for userData */
					 return ((CLineOut*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
																					     framesPerBuffer,
																					     timeInfo,
																					     statusFlags);
	    }//paCallback ends

			/* This routine is called by portlineOut when playback is done.
			*/
	    void paStreamFinishedMethod()
	    {
		 		printf( "Stream Completed: %s\n", caMessage );
	    }
			/* This routine is called by portlineOut when playback is done.
	    */
	    static void paStreamFinished(void* userData)
	    {
		 		return ((CLineOut*)userData)->paStreamFinishedMethod();
	    }

	};//CLineOut ends
	/*******************************************************************************************/
	
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
