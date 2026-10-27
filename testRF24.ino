#include <SPI.h>
#include <RF24.h>
#include <printf.h> // Necesario para mostrar los detalles formateados

RF24 radio(9, 10); // Reemplaza con tus pines CE y CSN

void setup() {
  Serial.begin(115200);
  printf_begin();
  
  if (!radio.begin()) {
    Serial.println(F("¡Error! El microcontrolador no se comunica con el nRF24L01 por SPI. Revisa tus conexiones."));
    while (1); // Detener ejecución
  }

  radio.printDetails(); // Esto te mostrará el estado real de los registros
}

void loop() {
  // Vacío para la prueba de diagnóstico
}
