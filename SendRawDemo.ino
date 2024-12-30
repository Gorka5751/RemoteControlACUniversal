#include <IRremote.hpp>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ---------------------------------------------------------------------------
// CONFIGURACIÓN IR
// ---------------------------------------------------------------------------
#define IR_SEND_PIN 12 // GPIO12 (D6 en NodeMCU)

// Raw data de ejemplo para encender
const uint16_t rawDataON[] = {4350, 4350, 500, 1600, 550, 550, 500, 1600, 550, 1600, 500, 550, 550, 500, 500, 1650, 500, 550, 500, 550, 500, 1650, 500, 550, 550, 500, 550, 1600, 500, 1600, 550, 500, 550, 1600, 550, 500, 550, 1600, 550, 500, 500, 1650, 500, 1600, 550, 1600, 500, 1600, 500, 1650, 500, 1600, 550, 550, 500, 1600, 500, 600, 500, 550, 500, 550, 500, 550, 550, 550, 500, 1600, 500, 550, 550, 550, 500, 550, 500, 1600, 550, 1600, 500, 550, 500, 550, 550, 550, 500, 1600, 500, 1650, 500, 1600, 550, 550, 500, 550, 500, 1600, 550, 1600, 500, 5150, 4350, 4350, 500, 1650, 500, 550, 500, 1600, 550, 1600, 500, 550, 500, 550, 550, 1600, 500, 550, 500, 600, 500, 1600, 500, 550, 500, 600, 500, 1600, 500, 1650, 500, 550, 500, 1600, 550, 550, 500, 1600, 500, 600, 500, 1600, 500, 1600, 550, 1600, 500, 1650, 500, 1600, 500, 1650, 500, 550, 500, 1600, 500, 600, 500, 550, 500, 600, 450, 600, 500, 550, 500, 1600, 500, 550, 550, 550, 500, 550, 500, 1650, 500, 1600, 500, 600, 450, 600, 500, 550, 450, 1700, 450, 1650, 500, 1650, 450, 600, 500, 550, 500, 1650, 450, 1650, 500};

// Raw data de ejemplo para apagar
const uint16_t rawDataOFF[] = {4400, 4300, 550, 1600, 500, 550, 500, 1650, 500, 1600, 500, 550, 550, 550, 500, 1600, 550, 500, 550, 550, 500, 1600, 500, 550, 550, 550, 500, 1600, 550, 1600, 500, 550, 550, 1600, 500, 550, 550, 1550, 550, 1600, 550, 1550, 550, 1600, 500, 550, 500, 1650, 500, 1600, 550, 1600, 550, 500, 550, 500, 550, 550, 500, 550, 500, 1600, 550, 550, 500, 550, 550, 1550, 550, 1600, 550, 1550, 550, 550, 500, 550, 550, 500, 550, 550, 500, 550, 500, 550, 550, 550, 500, 550, 500, 1600, 550, 1600, 500, 1650, 500, 1600, 500, 1650, 450, 5200, 4300, 4400, 500, 1600, 550, 550, 500, 1600, 500, 1650, 500, 550, 500, 550, 500, 1650, 500, 550, 500, 550, 550, 1600, 500, 550, 500, 550, 550, 1600, 500, 1600, 550, 550, 500, 1600, 500, 550, 550, 1600, 500, 1600, 550, 1600, 500, 1650, 500, 550, 500, 1600, 550, 1600, 500, 1600, 550, 550, 500, 550, 500, 550, 500, 550, 550, 1600, 500, 550, 500, 550, 550, 1600, 500, 1650, 500, 1600, 500, 550, 500, 600, 500, 550, 500, 550, 500, 600, 450, 600, 500, 550, 500, 550, 500, 1650, 500, 1600, 500, 1650, 500, 1600, 500, 1650, 500};

// ---------------------------------------------------------------------------
// CONFIGURACIÓN Wi-Fi
// ---------------------------------------------------------------------------
const char* ssid     = "****";
const char* password = "****";

// IP estática (opcional)
IPAddress local_IP(192, 168, 1, 61);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Servidor web en el puerto 80
ESP8266WebServer server(80);

// ---------------------------------------------------------------------------
// INSTANCIAR OBJETOS
// ---------------------------------------------------------------------------
IRsend irsend(IR_SEND_PIN);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Configurar IP estática (opcional)
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Error al configurar la IP estática");
  }

  // Modo Station
  WiFi.mode(WIFI_STA);

  // Auto-reconexión si se pierde WiFi
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);


  // Conexión a la red Wi-Fi
  Serial.println("Conectando a Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConexión establecida!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());


  // -------------------------------------------------------------------------
  // Rutas del servidor
  // -------------------------------------------------------------------------

  // Página principal con botones de ON y OFF
  server.on("/", []() {
    String htmlPage = 
      "<!DOCTYPE html>"
      "<html>"
      "<head><meta charset=\"utf-8\" /><title>Control AC</title></head>"
      "<body>"
      "<h1>Control AC</h1>"
      "<p>Elige una acción:</p>"
      "<button onclick=\"location.href='/on'\">Encender</button>"
      "<button onclick=\"location.href='/off'\">Apagar</button>"
      "</body>"
      "</html>";

    server.send(200, "text/html", htmlPage);
  });

  // Encender (enviar IR 10 veces)
  server.on("/on", []() {
    // Enviamos la señal 10 veces
    for (int i = 0; i < 10; i++) {
  irsend.sendRaw(rawDataON, sizeof(rawDataON) / sizeof(rawDataON[0]), 38);
  Serial.println("IR ON enviado.");
  yield();   // Give Wi-Fi time to handle background tasks
  delay(1000);
}


    // Respuesta al cliente
    String message = "AC encendido (señal enviada 10 veces).";
    server.send(200, "text/plain", message);
  });

  // Apagar (enviar IR 10 veces)
  server.on("/off", []() {
    // Enviamos la señal 10 veces
    for (int i = 0; i < 10; i++) {
  irsend.sendRaw(rawDataOFF, sizeof(rawDataOFF) / sizeof(rawDataOFF[0]), 38);
  Serial.println("IR OFF enviado.");
  yield();   // Give Wi-Fi time to handle background tasks
  delay(1000);
}


    // Respuesta al cliente
    String message = "AC apagado (señal enviada 10 veces).";
    server.send(200, "text/plain", message);
  });

  // Iniciar servidor
  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  // Maneja las solicitudes HTTP
  server.handleClient();

  // If disconnected, attempt reconnection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected, attempting reconnection...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
      delay(100);
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected!");
    } else {
      Serial.println("Still offline after attempt.");
    }
  }
}
