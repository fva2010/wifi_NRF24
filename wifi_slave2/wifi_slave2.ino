#include <SPI.h>
#include "RF24.h"
RF24 radio(9, 10);

const uint64_t pipes[2] = {0xF0F0F0F000LL, 0xF0F0F0F0FFLL};
// адреса каналов приема и передачи

void setup(){
  
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  radio.begin();  
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);
  radio.setRetries(15,15);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
}

void loop(){
  if(radio.available()){
    
    int data[32];
    radio.read(&data, 32);
    if(data[0] == 1)
      digitalWrite(8, LOW);
    
    radio.stopListening();
    radio.write(&data, 32);
    radio.startListening();
  }
  delay(300);
}
