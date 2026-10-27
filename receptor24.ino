#include <SPI.h>
#include <RF24.h>

#define led1 13

#define CE_PIN 9
#define CSN_PIN 10

int buttonState = 0;

RF24 radio(CE_PIN, CSN_PIN);
const byte address[5] = "00002";

void setup() {
  Serial.begin(9600);
  pinMode(led1,OUTPUT);
  digitalWrite(13, LOW);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);  // RF24_PA_MAX is default. 
  radio.startListening();
}

void loop() {
  radio.startListening();
  if (radio.available()){
     radio.read(&buttonState, sizeof(buttonState));
     Serial.println(buttonState);
     digitalWrite(led1, buttonState);
  }
  delay(50);
}
