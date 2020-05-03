
CFLAGS =-lrt -lm -lasound -ljack -pthread -ldl -std=c++11
LIB_FOLDER =./src
THIRD_PARTY_FOLDER= ./src/thirdPartyLibs
EXECUTABLE_NAME = PortableAudiospatialization

output: main.o ${LIB_FOLDER}/lineOut.o ${THIRD_PARTY_FOLDER}/loguru/loguru.o
	g++ main.o ${LIB_FOLDER}/lineOut.o ${LIB_FOLDER}/libportaudio.a  ${LIB_FOLDER}/portaudio.h ${CFLAGS} -o ${EXECUTABLE_NAME}

main.o: main.cpp
	g++ -c main.cpp ${CFLAGS}

lineOut.o: ${LIB_FOLDER}/lineOut.cpp ${LIB_FOLDER}/lineOut.hpp
	g++ -c ${LIB_FOLDER}/lineOut.cpp ${LIB_FOLDER}/lineOut.hpp ${CFLAGS}

clean:
	rm ${LIB_FOLDER}/*.o ./*.o ${THIRD_PARTY_FOLDER}/*/*.o
