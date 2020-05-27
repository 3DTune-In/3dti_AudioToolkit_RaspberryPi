
CFLAGS =-lrt -lm -lasound -ljack -pthread -ldl -std=c++11
LIB_FOLDER =./src
THIRD_PARTY_FOLDER= ./src/thirdPartyLibs
EXECUTABLE_NAME = PortableAudiospatialization

output: main.o ${LIB_FOLDER}/lineOut.o ${LIB_FOLDER}/soundFile.o ${LIB_FOLDER}/soundSource.o ${THIRD_PARTY_FOLDER}/loguru/loguru.o
	g++ main.o ${LIB_FOLDER}/lineOut.o ${LIB_FOLDER}/soundFile.o ${LIB_FOLDER}/soundSource.o ${LIB_FOLDER}/libportaudio.a  ${LIB_FOLDER}/portaudio.h ${CFLAGS} -o ${EXECUTABLE_NAME}

soundSource.o: ${LIB_FOLDER}/soundSource.cpp ${LIB_FOLDER}/soundSource.hpp
	g++ -c ${LIB_FOLDER}/soundSource.cpp ${LIB_FOLDER}/soundSource.hpp ${CFLAGS}
	
soundFile.o:${LIB_FOLDER}/soundFile.hpp ${LIB_FOLDER}/soundFile.cpp
	g++ -c ${LIB_FOLDER}/soundFile.cpp ${LIB_FOLDER}/soundFile.hpp ${CFLAGS} 

main.o: main.cpp
	g++ -c main.cpp ${CFLAGS}

lineOut.o: ${LIB_FOLDER}/lineOut.cpp ${LIB_FOLDER}/lineOut.hpp
	g++ -c ${LIB_FOLDER}/lineOut.cpp ${LIB_FOLDER}/lineOut.hpp ${CFLAGS}

clean:
	rm ${LIB_FOLDER}/*.o ./*.o ${THIRD_PARTY_FOLDER}/*/*.o
