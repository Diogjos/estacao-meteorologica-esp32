# 🌦️ Estação Meteorológica com ESP32 e Servidor Web

Este projeto utiliza um ESP32 conectado a sensores de **temperatura e umidade (DHT11)**, **pressão atmosférica (BMP280)** e **sensor de chuva**, com exibição dos dados em um **servidor web local** via Wi-Fi.


## 📌 Componentes Utilizados

- ESP32 DevKit v1
- Sensor DHT11 (temperatura e umidade)
- Sensor de chuva (módulo digital)
- Sensor de pressão BMP280 (I2C)
- Protoboard e cabos jumper
- Fonte USB

---

## 🔌 Esquema de Ligações

| Sensor        | Pino do sensor | ESP32 GPIO | Observações                    |
|---------------|----------------|-------------|-------------------------------|
| DHT11         | DATA           | GPIO 4      | VCC = 3.3V, GND = GND         |
| Sensor de chuva | D0           | GPIO 5      | VCC = 3.3V, GND = GND         |
| BMP280 (I2C)  | SDA / SCL      | GPIO 21 / 22| VCC = 3.3V, GND = GND         |


## 📡 Como Funciona

- O ESP32 conecta-se à rede Wi-Fi
- Os dados dos sensores são lidos periodicamente
- Um servidor HTTP local é criado
- Ao acessar o IP do ESP32, os dados são exibidos em tempo real no navegador


## 📲 Instalação

### 1. Pré-requisitos

- [Arduino IDE](https://www.arduino.cc/en/software)
- Suporte ao ESP32:
  - Adicionar essa URL em **Preferências > URLs adicionais de placas**:  
    ```
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```
  - Vá em **Placa > Gerenciador de Placas**, procure **esp32**, instale.

### 2. Bibliotecas necessárias

Instale via **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas**:

- `DHT sensor library` (Adafruit)
- `Adafruit Unified Sensor`
- `Adafruit BMP280`
- `Adafruit BusIO` (dependência)
- `WiFi` e `WebServer` (já inclusas com ESP32)

