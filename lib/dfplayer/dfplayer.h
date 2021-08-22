#ifndef _dfplayer_h
#define _dfplayer_h

#define DFPLAYER_DEBUG 1

/* Instruction fields */
#define DFPLAYER_START 0x7E
#define DFPLAYER_VERSION 0xFF
#define DFPLAYER_END 0xEF

/* EQ modes */
#define DFPLAYER_EQ_NORMAL 0
#define DFPLAYER_EQ_POP 1
#define DFPLAYER_EQ_ROCK 2
#define DFPLAYER_EQ_JAZZ 3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASE 5

/* Playback sources */
#define DFPLAYER_SOURCE_U 0
#define DFPLAYER_SOURCE_TF 1
#define DFPLAYER_SOURCE_AUX 2
#define DFPLAYER_SOURCE_SLEEP 3
#define DFPLAYER_SOURCE_FLASH 4

class DFPlayer {
    private:
        Stream* _serial;
        uint8_t receive_buffer[10];
        uint8_t receive_index;

        void send(uint8_t command, bool feedback, uint8_t parameter1, uint8_t parameter2);
        void send(uint8_t command, bool feedback, uint16_t parameter);
        void send(uint8_t command, bool feedback);
        void send(uint8_t command);
        void wait();

    public:
        bool begin(Stream& serial);

        void reset();
        void setVolume(uint8_t volume);
        void setSource(int source);
        void setEQ(uint8_t eq);
        void start(int num);
        void play();
        void pause();
        void next();
        void volumeUp();
        void volumeDown();
};

#endif