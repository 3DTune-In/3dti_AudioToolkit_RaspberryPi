# 3DTI AudioToolkit in RaspberryPi
3dti_AudioToolkit_RaspberryPi is a deployment of the 3D Tune-In Toolkit (https://github.com/3DTune-In/3dti_AudioToolkit) in Raspberry Pi based devices. The project makes audio spatialitation portable with the help of a raspberry based SoC, a battery, headphones and a TFT screen or similar. 

For a description of the 3D Tune-In Toolkit, see: 

Cuevas-Rodríguez M, Picinali L, González-Toledo D, Garre C, de la Rubia-Cuestas E, Molina-Tanco L and Reyes-Lecuona A. (2019) 3D Tune-In Toolkit: An open-source library for real-time binaural spatialisation. PLOS ONE 14(3): e0211899. https://doi.org/10.1371/journal.pone.0211899This 

# Authors
This project is being developed by Gonzalo Alfonso Bueno Santana as his Bachelor Thesis (TFG), for the BEng Electronic Engineering Degree at the University of Malaga, School of Telecommunication (http://etsit.uma.es/), under the supervision of Arcadio Reyes Lecuona, 

# Compile the project
To compile the project run on linux that:
~~~
 g++ -Wall main.cpp ./src/libportaudio.a ./src/lineOut.cpp -lrt -lm -lasound -ljack -pthread -o YOUR_BINARY
~~~

for that you need to have previously installed PortAudio libs. First of all download the latest versión from here: http://www.portaudio.com/download.html

Then check how to install step by step from here: http://portaudio.com/docs/v19-doxydocs/compile_linux.html
You need to copy the libportaudio.a file to your src folder from here: /usr/local/lib/libportaudio.a 
Also copy the portaudio.h file located in the include subdirectory of the same folder into your src folder.


# Audio reproduction
For now on, this project use PortAudio Libs to play an audio stream configurable for each speaker, also configurable in buffer size, frame rate and tone frecuency.

In next steps the project will be able to select the device to play the audio.
