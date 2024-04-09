#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h> // Librería para manipular JSON

const char* ssid = "MEGACABLE-24A2";
const char* password =  "9TnZxxFb";


String id = "E3A91727";
String base_url = "http://192.168.100.9:3000/";
void setup() {
  delay(10);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { // Verificar la conexión WiFi
    delay(500);
    Serial.print(".");
  }

  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status()== WL_CONNECTED) {   // Verificar la conexión WiFi
    HTTPClient http;
    
    // Crear el objeto JSON
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["id"] = id;
    
    // Convertir el JSON a una cadena
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);
    http.begin(base_url);  // Cambia "tu_direccion_ip" por la IP del servidor y el puerto si es necesario
    http.addHeader("Content-Type", "application/json"); // Indicar que los datos son JSON
    
    int codigo_respuesta = http.GET();   // Enviar el JSON
    
    if(codigo_respuesta > 0){
      Serial.print(base_url+id);
      Serial.println("Código HTTP ► " + String(codigo_respuesta));   // Imprimir el código de respuesta HTTP
      if(codigo_respuesta == 200){
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió ▼ ");
        Serial.println(cuerpo_respuesta);
        
        // Analizar la respuesta JSON
        DynamicJsonDocument jsonDoc(200);
        DeserializationError error = deserializeJson(jsonDoc, cuerpo_respuesta);
        if (!error) {
            String redirectUrl = jsonDoc["redirectUrl"];
            if (redirectUrl != "") {
                // Redirigir manualmente a la URL proporcionada
                http.end(); // Liberar recursos antes de redirigir
                http.begin(redirectUrl);
                codigo_respuesta = http.GET(); // o POST según sea necesario
                // Manejar la respuesta según sea necesario
            }
        }
      }
    } else {
     Serial.print("Error enviando POST, código: ");
     Serial.println(codigo_respuesta);
    }

    http.end();  // Liberar recursos
  } else {
     Serial.println("Error en la conexión WIFI");
  }

   delay(2000); // Esperar antes de realizar la siguiente iteración
}
