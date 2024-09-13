#include <SD.h>
#include <SPI.h>
#include <driver/dac.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SD_CS_PIN    5             // Pino CS do SD card
#define AUDIO_FILE   "/pcmaudio/audio003.wav" // Nome do arquivo PCM no SD card
#define SAMPLE_RATE  22050          // Taxa de amostragem de 22050 Hz
#define BUFFER_SIZE  512            // Tamanho do buffer para leitura

bool deviceConnected = false;       // Indica se o dispositivo BLE está conectado
bool audioPlayed = false;           // Indica se o áudio já foi reproduzido

// Callback para quando um dispositivo BLE se conecta ou desconecta
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Dispositivo BLE conectado.");
      deviceConnected = true;
      audioPlayed = false;  // Reinicia o estado de reprodução de áudio
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Dispositivo BLE desconectado.");
      deviceConnected = false;
      pServer->getAdvertising()->start();  // Recomeça a publicidade BLE
    }
};

void playAudio(const char* audioFileName);

void setup() {
  Serial.begin(115200);

  // Inicializa o SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Erro ao inicializar o SD card!");
    while (true);
  }

  // Configura o DAC no GPIO 25 (canal DAC1)
  dac_output_enable(DAC_CHANNEL_1);

  // Configura o servidor BLE
  BLEDevice::init("ESP32 Audio Player");    // Nome do dispositivo BLE
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Cria o serviço BLE (UUID genérico)
  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x180A));

  // Cria uma característica BLE
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         BLEUUID((uint16_t)0x2A29),
                                         BLECharacteristic::PROPERTY_READ
                                       );

  // Inicia o serviço e a publicidade BLE
  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Esperando conexão BLE...");
}

void loop() {
  // Se o dispositivo BLE estiver conectado e o áudio ainda não foi reproduzido
  if (deviceConnected && !audioPlayed) {
    playAudio(AUDIO_FILE);  // Reproduzir o arquivo de áudio 'audio003.wav'
    audioPlayed = true;     // Define que o áudio foi reproduzido
  }

  delay(100);  // Pequeno delay para evitar leitura rápida
}

// Função para reproduzir o arquivo de áudio
void playAudio(const char* audioFileName) {
  Serial.print("Reproduzindo: ");
  Serial.println(audioFileName);

  // Verifica se o arquivo de áudio existe
  if (!SD.exists(audioFileName)) {
    Serial.print("Arquivo de áudio não encontrado: ");
    Serial.println(audioFileName);
    return;
  }

  // Abre o arquivo de áudio
  File audioFile = SD.open(audioFileName);
  if (!audioFile) {
    Serial.print("Erro ao abrir o arquivo de áudio: ");
    Serial.println(audioFileName);
    return;
  }

  uint8_t buffer[BUFFER_SIZE];
  size_t bytesRead;

  // Transmitir dados PCM para o DAC
  while (audioFile.available()) {
    bytesRead = audioFile.read(buffer, sizeof(buffer));  // Ler blocos do arquivo PCM
    for (size_t i = 0; i < bytesRead; i++) {
      dac_output_voltage(DAC_CHANNEL_1, buffer[i]);  // Envia os dados PCM para o DAC
      delayMicroseconds(45);  // Ajusta a taxa de amostragem para 22050 Hz
    }
  }

  // Fecha o arquivo após a reprodução
  audioFile.close();
  Serial.print("Reprodução de ");
  Serial.print(audioFileName);
  Serial.println(" concluída.");
}
