#include <Arduino.h>
#include <SoftwareSerial.h>
#include <dfplayer.h>

SoftwareSerial playerSerial(10, 11);
DFPlayer player;

void setup() {
  Serial.begin(9600);

  playerSerial.begin(9600);
  
  player.begin(playerSerial);
  player.reset();
  delay(1000);

  player.setVolume(15);
  player.setSource(DFPLAYER_SOURCE_TF);
  player.setEQ(DFPLAYER_EQ_BASE);
  player.start(1);
}

bool paused;

void loop() {
  while(Serial.available() ) {
    switch(Serial.read()) {
      case 'n':
        player.next();
        break;
      case 'u':
        player.volumeUp();
        break;
      case 'd':
        player.volumeDown();
        break;
      case 'p':
        paused = !paused;

        if (paused) {
          player.pause();
        } else {
          player.play();
        }
    }
  }
}