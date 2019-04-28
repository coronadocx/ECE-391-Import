/* header file for keyboard */
#ifndef KEYBOARD_H
#define KEYBOARD_H



#define KEYBOARD_CMD_PORT  0x60
#define KEYBOARD_BUFFER_LENGTH 128
#define NUM_KEYS  255
#define LEFTSHIFT 0x2A
#define LEFTSHIFTRELEASED 0xAA
#define RIGHTSHIFT 0x36
#define RIGHTSHIFTRELEASED 0xB6
#define CAPSLOCK 0x3A
#define LEFTCONTROLPRESSED 0x1D
#define LEFTCONTROLRELEASED 0x9D
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define ASCIILOWERCASEA 97
#define ASCIILOWERCASEZ 122
#define ASCII1 49
#define ASCII9 57
#define NUMBERSONKEYBOARD 10
#define ALTPRESSED 0x38
#define ALTRELEASED 0xB8


#define UP_PRESSED              0x48
#define UP_RELEASED             0xC8
#define DOWN_PRESSED            0x50
#define DOWN_RELEASED           0xD0
#define HISTORY_LENGTH          20
#define NUM_TERMINALS           3

void check_input();
void init_keyboard();
void init_history_buffer();
/* This writes a command into the history buffer of the visible terminal*/
void write_history_buffer(int terminal_number, char linebuffer[KEYBOARD_BUFFER_LENGTH]);

#endif
