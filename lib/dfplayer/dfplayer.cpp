#include <Arduino.h>
#include "dfplayer.h"

bool DFPlayer::begin(Stream& serial) {
    this->_serial = &serial;
    this->receive_index = 0;

    return true;
}

void DFPlayer::send(uint8_t command, bool feedback, uint8_t parameter1, uint8_t parameter2) {
    uint8_t buf[10];

    buf[0] = DFPLAYER_START; // Start
    buf[1] = DFPLAYER_VERSION; // Version
    buf[2] = 0x06; // Length
    buf[3] = command; // Command
    buf[4] = feedback; // Command feedback
    buf[5] = parameter1; // Parameter 1
    buf[6] = parameter2; // Parameter 2
    buf[9] = DFPLAYER_END; // End

    // Calculate checksum
    uint16_t sum = 0;
    for (int i=1; i < 7; i++) {
        sum += buf[i];
    }
    sum = -sum;

    buf[7] = (sum >> 8)& 0xFF;
    buf[8] = sum & 0xFF;

    #ifdef DFPLAYER_DEBUG
        Serial.println("DFPlayer Sending: ");
        for (int i=0; i < 10; i++) {
            Serial.print(buf[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
    #endif

    this->_serial->write(buf, 10);
}

void DFPlayer::send(uint8_t command, bool feedback, uint16_t parameter) {
    this->send(command, feedback, (parameter >> 8) & 0xFF, parameter & 0xFF);
}

void DFPlayer::send(uint8_t command, bool feedback) {
    this->send(command, feedback, 0x00);
}

void DFPlayer::send(uint8_t command) {
    this->send(command, 0x00);
}

void DFPlayer::wait() {
    while (true) {
        while (this->_serial->available()) {
            if (this->receive_index == 0) {
                this->receive_buffer[0] = this->_serial->read();

                Serial.print("Received: ");
                Serial.println(this->receive_buffer[this->receive_index], HEX);

                // Check if start byte is present
                if (this->receive_buffer[this->receive_index] == DFPLAYER_START) {
                    this->receive_index++;
                }
            } else {
                this->receive_buffer[this->receive_index] = this->_serial->read();

                switch(this->receive_index) {
                    case 1:
                        if (this->receive_buffer[this->receive_index] != 0xFF) {
                            #ifdef DFPLAYER_DEBUG
                                Serial.println("DFPlayer Invalid version!");
                            #endif

                        }
                        break;
                    case 2:
                        if (this->receive_buffer[this->receive_index] != 0x06) {
                            #ifdef DFPLAYER_DEBUG
                                Serial.println("DFPlayer Invalid length!");
                            #endif

                        }
                        break;
                    case 3:
                        switch (this->receive_buffer[this->receive_index]) {
                            case 0x41:
                                #ifdef DFPLAYER_DEBUG
                                    Serial.println("DFPlayer Received reply");
                                #endif
                                break;
                            case 0x40:
                                #ifdef DFPLAYER_DEBUG
                                    Serial.println("DFPlayer Returned error!");
                                #endif
                                break;
                        }
                        break;
                    case 10:
                        Serial.print("Result: ");
                        for (int i=0; i < 10; i++) {
                            Serial.print(this->receive_buffer[this->receive_index], HEX);
                            Serial.print(" ");
                        }
                        Serial.println("");

                        if (this->receive_buffer[this->receive_index] != DFPLAYER_END) {
                            #ifdef DFPLAYER_DEBUG
                                Serial.println("DFPlayer Invalid end byte");
                            #endif

                        }
                        this->receive_index = 0;
                        return;
                }

                this->receive_index++;
            }
        }
    }
}

void DFPlayer::reset() {
    this->send(0x0C);
}

void DFPlayer::setVolume(uint8_t volume) {
    this->send(0x06, false, volume);
}

void DFPlayer::setSource(int source) {
    this->send(0x09, false, source);
}

void DFPlayer::setEQ(uint8_t eq) {
    this->send(0x07, false, eq);
}

void DFPlayer::start(int num) {
    this->send(0x03, false, num);
}

void DFPlayer::play() {
    this->send(0x0D);
}

void DFPlayer::pause() {
    this->send(0x0E);
}

void DFPlayer::next() {
    this->send(0x01);
}

void DFPlayer::volumeUp() {
    this->send(0x04);
}

void DFPlayer::volumeDown() {
    this->send(0x05);
}