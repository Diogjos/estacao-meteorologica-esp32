#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>

// === Wi-Fi ===
const char* ssid = "DIOGO_SERGIONET987427472";
const char* password = "963852DD";

// === Pinos dos sensores ===
#define DHTPIN 4
#define DHTTYPE DHT11
#define CHUVA_PIN 5 // Sensor digital de chuva no GPIO 5

// === Objetos dos sensores ===
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp; // I2C: SDA=21, SCL=22 (padrão ESP32)
AsyncWebServer server(80);

// === Funções ===
String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) return "--";
  return String(t);
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) return "--";
  return String(h);
}

String readRainSensor() {
  int chuva = digitalRead(CHUVA_PIN);
  return chuva == HIGH ? "Seco" : "Chuva";
}

String readBMP280Pressure() {
  float pressure = bmp.readPressure() / 100.0F;
  return String(pressure, 1);
}

// === HTML Web Page ===
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
  <style>
    html { font-family: Arial; text-align: center; }
    h2 { font-size: 3.0rem; }
    p { font-size: 2.5rem; }
    .units { font-size: 1.2rem; }
    .dht-labels { font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px; }
  </style>
</head>
<body>
  <h2>ESP32 - Monitoramento Climático</h2>
  <p><i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperatura</span> 
    <span id="temperature">%TEMPERATURE%</span><sup class="units">&deg;C</sup></p>
  <p><i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Umidade</span> 
    <span id="humidity">%HUMIDITY%</span><sup class="units">&percnt;</sup></p>
  <p><i class="fas fa-cloud-showers-heavy" style="color:#0077be;"></i> 
    <span class="dht-labels">Chuva</span> 
    <span id="rain">%RAIN%</span></p>
  <p><i class="fas fa-tachometer-alt" style="color:#aa0000;"></i> 
    <span class="dht-labels">Pressão</span> 
    <span id="pressure">%PRESSURE%</span><sup class="units"> hPa</sup></p>
</body>
<script>
setInterval(() => {
  fetch('/temperature').then(r => r.text()).then(t => document.getElementById('temperature').innerHTML = t);
  fetch('/humidity').then(r => r.text()).then(h => document.getElementById('humidity').innerHTML = h);
  fetch('/rain').then(r => r.text()).then(rain => document.getElementById('rain').innerHTML = rain);
  fetch('/pressure').then(r => r.text()).then(p => document.getElementById('pressure').innerHTML = p);
}, 10000);
</script>
</html>
)rawliteral";

// === Processador de Placeholders ===
String processor(const String& var) {
  if (var == "TEMPERATURE") return readDHTTemperature();
  if (var == "HUMIDITY") return readDHTHumidity();
  if (var == "RAIN") return readRainSensor();
  if (var == "PRESSURE") return readBMP280Pressure();
  return String();
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(CHUVA_PIN, INPUT);

  if (!bmp.begin(0x76)) {  // Verifique se o endereço do seu BMP280 é 0x76 ou 0x77
    Serial.println("Sensor BMP280 não encontrado!");
  } else {
    Serial.println("BMP280 iniciado com sucesso.");
  }

  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", readDHTTemperature());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", readDHTHumidity());
  });

  server.on("/rain", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", readRainSensor());
  });

  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", readBMP280Pressure());
  });

  server.begin();
}

void loop() {
  // Não é necessário colocar nada aqui para este projeto
}
