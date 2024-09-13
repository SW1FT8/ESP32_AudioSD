#include <SD.h>
#include <SPI.h>
#include <driver/dac.h> // Usar driver DAC padrão do ESP32

#define SD_CS_PIN    5          // Pino CS do SD card
#define SAMPLE_RATE  22050       // Taxa de amostragem de 22050 Hz
#define BUFFER_SIZE  512         // Tamanho do buffer para leitura
#define TOTAL_AUDIOS 5           // Total de arquivos de áudio

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

  Serial.println("Digite um número entre 1 e 5 para tocar o áudio correspondente.");
}

void loop() {
  // Verifica se há dados disponíveis no Serial Monitor
  if (Serial.available() > 0) {
    int audioNum = Serial.parseInt(); // Lê o número digitado no Serial Monitor

    // Verifica se o número está dentro do intervalo permitido
    if (audioNum >= 1 && audioNum <= TOTAL_AUDIOS) {
      // Gerar o nome do arquivo de áudio dinamicamente (audio001.wav, audio002.wav, etc.)
      char audioFileName[30];
      sprintf(audioFileName, "/pcmaudio/audio00%d.wav", audioNum);
      
      // Reproduzir o arquivo de áudio
      playAudio(audioFileName);
    } else {
      Serial.println("Número inválido. Digite um número entre 1 e 5.");
    }
  }
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
