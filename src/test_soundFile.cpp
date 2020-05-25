#include "./soundFile.hpp"
#include <string>
using namespace std;
const string WAV_PATH = "./thirdPartyLibs/AudioFile/tests/AudioFileTests/test-audio/wav_stereo_24bit_44100.wav";

using namespace std;
using namespace sound_file_namespace;
int main(){

  CSoundFile prueba;
  prueba.setup(WAV_PATH);
  cout << "tamaño de arhivo " << prueba.getFileLength() << endl;
  cout << "tamaño de arhivo en tiempo " << prueba.getTimeFileLenght() << endl;
  for(int x=0; x<10; x++){
      cout << "Muestra actual " << prueba.getActuaalSampleNumber() << endl;
      cout<< endl << prueba.getFrame() << endl;
  }
  cout << "Sample rate del archivo " << prueba.getSampleRate() << endl;
  return 0;
}