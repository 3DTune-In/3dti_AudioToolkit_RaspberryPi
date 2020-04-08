# TFG-PortableAudiospatialization
An audio spatialitation project centered in Raspberry Pi based devices. The project makes audio spatialitation portable with the help of a raspberry based SoC, a battery, headphones and a TFT screen or similar. It use 3DTI-TK library to make the process.

# Compile the project
To compile the project run on linux that:
~~~
 g++ -Wall sinMain.cpp libportaudio.a lineOut.cpp -lrt -lm -lasound -ljack -pthread -o YOUR_BINARY
~~~

# Audio reproduction
For now on, this project use PortAudio Libs to play an audio stream configurable for each speaker, also configurable in buffer size, frame rate and tone frecuency.

In next steps the project will be able to select the device to play the audio.
