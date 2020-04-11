
CFLAGS =-lrt -lm -lasound -ljack -pthread 
LIB_FOLDER =./src/lineOut
EXECUTABLE_NAME =YOUR_BINARY

output: main.o ${LIB_FOLDER}/lineOut.o
	g++ main.o ${LIB_FOLDER}/lineOut.o ${LIB_FOLDER}/libportaudio.a  ${LIB_FOLDER}/portaudio.h ${CFLAGS} -o ${EXECUTABLE_NAME}

main.o: main.cpp
	g++ -c main.cpp ${CFLAGS}

lineOut.o: ${LIB_FOLDER}/lineOut.cpp ${LIB_FOLDER}/lineOut.hpp
	g++ -c ${LIB_FOLDER}/lineOut.cpp ${LIB_FOLDER}/lineOut.hpp ${CFLAGS}

clean:
	rm ${LIB_FOLDER}/*.o ./*.o
