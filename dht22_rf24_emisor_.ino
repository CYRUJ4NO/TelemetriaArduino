#include <SPI.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// CE = 9, CSN = 10 (Los pines SPI 50, 51 y 52 los maneja la librería automáticamente en el Mega)
RF24 radio(9, 10); 
const byte direccion[6] = "000001";

struct DatosTelemetria {
  float temperatura;
  float humedad;
};

DatosTelemetria misDatos;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  if (!radio.begin()) {
    Serial.println(F("Error: El Mega no se comunica con el nRF24 por los pines 50-52"));
    while (1);
  }
  
  // Configuración para el módulo con Antena Externa (PA+LNA)
  radio.setPALevel(RF24_PA_MAX);      // Potencia de transmisión
  radio.setDataRate(RF24_1MBPS);      // Velocidad equilibrada y compatible
  radio.openWritingPipe(direccion);
  radio.stopListening();              // Modo Emisor
  
  Serial.println(F("Emisor Mega 2560 (Alta Potencia) Listo..."));
}

void loop() {
  // 1. Leer ambos valores del sensor DHT22
  misDatos.temperatura = dht.readTemperature();
  misDatos.humedad = dht.readHumidity();

  // 2. Validar que la lectura sea correcta antes de transmitir
  if (isnan(misDatos.temperatura) || isnan(misDatos.humedad)) {
    Serial.println(F("Error de lectura en DHT22"));
    delay(2000);
    return;
  }

  // 3. ENVIAR LA ESTRUCTURA COMPLETA (Contiene T y H)
  // sizeof(misDatos) le dice a la radio que envíe el paquete completo de 8 bytes (4 bytes por cada float)
  bool exito = radio.write(&misDatos, sizeof(misDatos));

  // 4. Mostrar confirmación en el Monitor Serie del Emisor
  if (exito) {
    Serial.print(F("Transmitido con éxito -> "));
    Serial.print(F("T: "));
    Serial.print(misDatos.temperatura, 1);
    Serial.print(F(" C | H: "));
    Serial.print(misDatos.humedad, 1);
    Serial.println(F(" %"));
  } else {
    Serial.print(F("Datos pendientes de envio -> "));
    Serial.print(F("T: "));
    Serial.print(misDatos.temperatura, 1);
    Serial.print(F(" C | H: "));
    Serial.print(misDatos.humedad, 1);
    Serial.println(F(" %"));
    Serial.println(F("Fallo de envío. El receptor no respondió el ACK."));
  }

  delay(100);
}
