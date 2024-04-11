#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
char id[25] = "";

const char* ssid = "MEGACABLE-24A2";
const char* password =  "9TnZxxFb";
const char* serverUrl = "http://192.168.100.9:3000";

WiFiClient wifiClient;
HTTPClient httpClient;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado con éxito");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Inicialización de NFC
  nfc.begin(); // Simplemente llamar a la función begin() sin comprobar el resultado
  Serial.println("Lector NFC iniciado correctamente");
}
 
void leerIDNFC() {
  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    strncpy(id, tag.getUidString().c_str(), sizeof(id));
    Serial.print("ID del tag NFC: ");
    Serial.println(id);
  }
}

void enviarIDAlServidor() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error: No hay conexión WiFi!");
    return;
  }

  if (!httpClient.begin(wifiClient, serverUrl)) {
    Serial.println("Error al conectar al servidor!");
    return;
  }

  httpClient.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> jsonDocument;
  jsonDocument["id"] = id;

  String jsonString;
  serializeJson(jsonDocument, jsonString);

  int codigo_respuesta = httpClient.POST(jsonString);
  if (codigo_respuesta > 0) {
    Serial.print("Código HTTP: ");
    Serial.println(codigo_respuesta);

    if (codigo_respuesta == 200) {
      String cuerpo_respuesta = httpClient.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(cuerpo_respuesta);
    }
  } else {
    Serial.print("Error al enviar la solicitud HTTP: ");
    Serial.println(httpClient.errorToString(codigo_respuesta));
  }

  httpClient.end();
}

void loop() {
  leerIDNFC();
  enviarIDAlServidor();
  delay(5000);
}

