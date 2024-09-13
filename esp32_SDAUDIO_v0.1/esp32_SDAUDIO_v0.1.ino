#include <SD.h>
#include <SPI.h>
#include <driver/dac.h> // Inclui o driver comum para DAC

#define SD_CS_PIN    5          // Pino CS do SD card
#define AUDIO_FILE   "/pcmaudio/audio003.wav" // Nome do arquivo PCM no SD card
#define SAMPLE_RATE  22050       // Taxa de amostragem de 22050 Hz
#define BUFFER_SIZE  512         // Tamanho do buffer para leitura

void setup() {
  Serial.begin(115200);

  // Inicializa o SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Erro ao inicializar o SD card!");
    while (true);
  }

  // Verifica se o arquivo de áudio existe
  if (!SD.exists(AUDIO_FILE)) {
    Serial.println("Arquivo de áudio não encontrado!");
    while (true);
  }

  // Abre o arquivo de áudio
  File audioFile = SD.open(AUDIO_FILE);
  if (!audioFile) {
    Serial.println("Erro ao abrir o arquivo de áudio!");
    while (true);
  }

  // Configura o DAC
  dac_output_enable(DAC_CHANNEL_1);  // Habilita o canal DAC1 para GPIO 25 (novo driver do ESP32)

  // Reproduzir o arquivo de áudio
  Serial.println("Reproduzindo...");
  uint8_t buffer[BUFFER_SIZE];
  size_t bytesRead;

  // Transmitir dados PCM para o DAC
  while (audioFile.available()) {
    bytesRead = audioFile.read(buffer, sizeof(buffer));  // Ler blocos do arquivo PCM
    for (size_t i = 0; i < bytesRead; i++) {
      dac_output_voltage(DAC_CHANNEL_1, buffer[i]);  // Envia os dados PCM para o DAC (usando novo driver)
      delayMicroseconds(45);  // Ajusta a taxa de amostragem para 22050 Hz (1 / 22050 ≈ 45 microssegundos)
    }
  }

  // Fecha o arquivo após a reprodução
  audioFile.close();
  Serial.println("Reprodução concluída!");
}

void loop() {
  // O código principal já está no setup, sem necessidade de loop
}
