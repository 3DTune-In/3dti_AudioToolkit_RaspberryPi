/**
* \start date	december 2019
* \class line_out
*
* \brief Declaration of class line_out
* \date	April 2020
*
* \authors: Gonzalo Alfonso Bueno Santana
* \b Contact : gonzupi6@gmail.com
* Supervised by , A. Reyes-Lecuona (University of Malaga)
* \b Contact: areyes@uma.es
*
* \b Contributions: (additional authors/contributors can be added here)
*
* \b Project: 3dti_AudioToolkit_RaspberryPi is a deployment of the 3D Tune-In Toolkit (https://github.com/3DTune-In/3dti_AudioToolkit) 
* in Raspberry Pi based devices. It is developed by Gonzalo Alfonso bueno Santana as his Bachelor Thesis in the BEng Electronic Engineering Degree
* at the University of Malaga, School of Telecommunication (http://etsit.uma.es/)
* For a description of the 3D Tune-In Toolkit, see: 
* Cuevas-Rodríguez M, Picinali L, González-Toledo D, Garre C, de la Rubia-Cuestas E, Molina-Tanco L and Reyes-Lecuona A. (2019) 
* 3D Tune-In Toolkit: An open-source library for real-time binaural spatialisation. PLOS ONE 14(3): e0211899. 
* https://doi.org/10.1371/journal.pone.0211899
*
* \b Website: https://github.com/3DTune-In/3dti_AudioToolkit_RaspberryPi
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
//#include "./thirdPartyLibs/loguru/loguru.cpp"
using namespace std;

#define DEFAULT_TONE_FRECUENCY 880
#define NUM_SECONDS 1

namespace line_out_namespace{
	//Start the lineOut stream with default configuration
	//A sin wave of 440Hz, with a 44100 size of SampleRate.
	CLineOut::CLineOut() : stream(0){
	}

	bool CLineOut::setup(PaDeviceIndex __index, int __iBufferSize, int __iSampleRate, int __iNumberOfChannels){
		iNumberOfChannels= (__iNumberOfChannels);
		iSampleRate = (__iSampleRate);
		iBufferSize = (__iBufferSize);
		
    vpfDataPointer=&vfData;
		//Initialize vector with 0s
    for(unsigned int iCount=0; iCount < iSampleRate; iCount++){
			for(int iActualChannel = 0; iActualChannel< iNumberOfChannels; iActualChannel++){
				vfData.push_back(0);
			}
    }


		PaStreamParameters outputParameters;
		outputParameters.device = __index;
		if (outputParameters.device == paNoDevice){
		 LOG_F(ERROR,"ERROR : Device not found");
		 return false;//device not found
		}
		const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(__index);
		if (pInfo != 0) LOG_F(INFO,"Output device name: '%s'\r", pInfo->name);

		outputParameters.channelCount = __iNumberOfChannels;       /* stereo output */
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

		if (err != paNoError){
		 LOG_F(ERROR, "Failed opening the stream device");
		 return false;	 /* Failed to open stream to device !!! */
		}
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

		LOG_F(2,"started\n");
		float fSine = 0;
		int iActualValue = 0;
    for(unsigned int iCount=0; iCount<iSampleRate; iCount++)
    {
    		fSine = (float) sin( ((double)(iCount)/(double)iSampleRate) *DEFAULT_TONE_FRECUENCY* NUMBER_PI * 2. );
				for(int iActualChannel = 0; iActualChannel<iNumberOfChannels; iActualChannel++){
					vfData[iActualValue] =(fSine);
					iActualValue++;
				}
    }
    LOG_F(2,"data created");

    LOG_F(INFO,"playing %d Hz for %d seconds...\n",DEFAULT_TONE_FRECUENCY,NUM_SECONDS);
		CLineOut::start();
		Pa_Sleep( NUM_SECONDS * 1000 );
		CLineOut::stop();
	  LOG_F(INFO,"stop for %d seconds\n",NUM_SECONDS);
    Pa_Sleep( NUM_SECONDS * 1000 );
		iActualValue = 0;
    for(unsigned int iCount=0; iCount<iSampleRate; iCount++)
    {
    		fSine = (float) sin( ((double)(iCount)/(double)iSampleRate) *DEFAULT_TONE_FRECUENCY* 2*NUMBER_PI * 2. );
				for(int iActualChannel = 0; iActualChannel<iNumberOfChannels; iActualChannel++){
					vfData[iActualValue] =(fSine);
					iActualValue++;
				}
    }
    LOG_F(INFO,"playing %d Hz for %d seconds...\n",DEFAULT_TONE_FRECUENCY/2,NUM_SECONDS);
    CLineOut::start();
    Pa_Sleep( NUM_SECONDS * 1000 );
		CLineOut::stop();
		LOG_F(INFO,"stopped\n");
		CLineOut::close();
		LOG_F(INFO,"closed\n");
		LOG_F(INFO,"Test finished! Good job!");
		return true;
	}

	int CLineOut::getSampleRate(){
		return iSampleRate;
	}

	int CLineOut::getBufferSize(){
		return iBufferSize;
	}

	void CLineOut::getBufferDataAdress(vector <float> * * __data){
		*__data = vpfDataPointer;
	}

	void CLineOut:: setBufferDataAdress(vector <float> * __data){
		vpfDataPointer= __data;
	}
}//CLineOut ends
