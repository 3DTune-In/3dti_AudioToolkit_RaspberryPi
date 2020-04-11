# TFG-PortableAudiospatialization
An audio spatialitation project centered in Raspberry Pi based devices. The project makes audio spatialitation portable with the help of a raspberry based SoC, a battery, headphones and a TFT screen or similar. It use 3DTI-TK library to make the process.

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
