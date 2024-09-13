# ESP32 BLE Audio Player

Este projeto é um **reprodutor de áudio** baseado no **ESP32** que utiliza **Bluetooth Low Energy (BLE)** para controlar a reprodução de arquivos de áudio armazenados em um cartão SD. Quando um dispositivo BLE, como um smartphone, se conecta ao ESP32, um arquivo de áudio específico é reproduzido automaticamente através do DAC integrado do ESP32.

## Funcionalidades

- **Conexão BLE**: O ESP32 se comporta como um servidor BLE. Dispositivos BLE podem se conectar e iniciar a reprodução do áudio.
- **Reprodução de áudio PCM**: Arquivos de áudio no formato PCM (22050 Hz, 8 bits) são armazenados no cartão SD e reproduzidos via DAC.
- **SD Card**: Arquivos de áudio são lidos de um cartão SD utilizando a interface SPI.
- **DAC integrado**: O ESP32 utiliza o DAC (Canal 1 - GPIO 25) para reproduzir áudio diretamente em um alto-falante.
- **Manutenção de conexão BLE**: O dispositivo BLE permanece conectado durante a reprodução do áudio e não é desconectado automaticamente.

## Componentes Utilizados

- **ESP32**: Controlador principal que gerencia BLE, DAC, e leitura do SD Card.
- **Cartão SD**: Armazena os arquivos de áudio no formato PCM.
- **Alto-falante**: Conectado ao DAC do ESP32 para reprodução do áudio.
- **Amplificador PAM8403**: Usado para amplificar o som do DAC (opcional, dependendo da saída de áudio).

## Como Funciona

1. Ao iniciar, o ESP32 começa a fazer publicidade BLE com o nome **ESP32 Audio Player**.
2. Quando um dispositivo BLE se conecta ao ESP32, o arquivo de áudio `audio003.wav` é reproduzido automaticamente.
3. Após a reprodução, o dispositivo permanece conectado até que seja desconectado manualmente.
4. O áudio é reproduzido diretamente pelo DAC (Digital-to-Analog Converter) do ESP32.

## Formato dos Arquivos de Áudio

- Os arquivos de áudio devem estar no formato **WAV** com as seguintes especificações:
  - **Formato PCM**: Unsigned 8 bits.
  - **Taxa de amostragem**: 22050 Hz.
- Coloque os arquivos no diretório `pcmaudio/` no cartão SD.

## Requisitos

- **Arduino IDE** com a biblioteca ESP32 configurada.
- **Bibliotecas**:
  - `SD.h` (para leitura do cartão SD).
  - `SPI.h` (para comunicação SPI com o SD Card).
  - `BLEDevice.h` (para funcionalidade BLE).
  - `driver/dac.h` (para controle do DAC).

## Como Usar

1. Coloque seus arquivos de áudio no formato PCM (ex: `audio003.wav`) no diretório `/pcmaudio/` do cartão SD.
2. Carregue o código no ESP32 usando a Arduino IDE.
3. Conecte-se ao dispositivo **ESP32 Audio Player** via BLE.
4. Ao conectar, o arquivo de áudio será reproduzido automaticamente.

## Possíveis Expansões

- Adicionar suporte a múltiplos arquivos de áudio, permitindo que o usuário selecione qual arquivo tocar via BLE.
- Implementar comandos via BLE para controle de volume e reprodução.
- Suporte para diferentes formatos de áudio, como MP3 ou WAV em outras frequências.

