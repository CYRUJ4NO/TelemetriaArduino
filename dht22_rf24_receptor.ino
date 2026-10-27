#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 32 // Ajustado a tu pantalla de 128x32
Adafruit_SSD1306 oled(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

RF24 radio(9, 10); // CE, CSN

// CORRECCIÓN: Sintaxis correcta con corchetes para la librería RF24
const byte direccion[6] = "000001"; 

struct DatosTelemetria {
  float temperatura;
  float humedad;
};

DatosTelemetria datosRecibidos;

void setup() {
  Serial.begin(115200);
  
  // Inicializar pantalla de 128x32 (Dirección I2C común: 0x3C)
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Fallo OLED. Revisa pines 20 (SDA) y 21 (SCL)"));
    while(1);
  }
  
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0,0);
  oled.println(F("Iniciando Radio..."));
  oled.display();

  if (!radio.begin()) {
    oled.clearDisplay();
    oled.println(F("Error SPI (50-52)"));
    oled.display();
    while (1);
  }
  
  radio.setPALevel(RF24_PA_LOW); 
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(1, direccion); // Se pasa el array de la dirección
  radio.startListening();              
  
  oled.clearDisplay();
  oled.println(F("Esperando datos..."));
  oled.display();
}

void loop() {
  if (radio.available()) {
    radio.read(&datosRecibidos, sizeof(datosRecibidos));
    
    oled.clearDisplay();
    
    // Distribución optimizada para 128x32 (Dos columnas, texto tamaño 1)
    oled.setTextSize(1);
    
    // Título superior centrado
    oled.setCursor(20, 0);
    oled.println(F("TELEMETRIA NRF24"));
    oled.drawFastHLine(0, 9, 128, SSD1306_WHITE); // Línea divisoria fina
    
    // Fila de datos (Lado izquierdo Temperatura, Lado derecho Humedad)
    // Temperatura en X=0, Y=16
    oled.setCursor(0, 18);
    oled.print(F("Temp: "));
    oled.print(datosRecibidos.temperatura, 0);
    oled.print((char)247); // Símbolo °
    oled.print(F("C"));
    
    // Humedad en X=68, Y=16
    oled.setCursor(72, 18);
    oled.print(F("Hum: "));
    oled.print(datosRecibidos.humedad, 0);
    oled.print(F("%"));
    
    oled.display();
  }
}
