#include <SPI.h>
#include <RF24.h>

#define buttonPin1 4

#define CE_PIN 9
#define CSN_PIN 10

int button1State = 0;

RF24 radio(CE_PIN, CSN_PIN);

const byte address[5] = "00002";

void setup() {
  pinMode(buttonPin1,INPUT_PULLUP);
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);  // RF24_PA_MAX is default.  
  radio.stopListening(); 
}

void loop() {
  button1State = digitalRead(buttonPin1);
  Serial.println(button1State);
  radio.write(&button1State, sizeof(button1State));
  delay(50);
}
