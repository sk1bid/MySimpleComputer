#ifndef MY_READ_KEY_H
#define MY_READ_KEY_H

enum keys {
    KEY_UP,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_ENTER,
    KEY_ESCAPE,

    KEY_F5,
    KEY_F6,

    KEY_PLUS,
    KEY_MINUS,

    KEY_ZERO = 48,
    KEY_ONE,
    KEY_TWO,
    KEY_THREE,
    KEY_FOUR,
    KEY_FIVE,
    KEY_SIX,
    KEY_SEVEN,
    KEY_EIGHT,
    KEY_NINE,

    KEY_A = 65,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_a = 97,
    KEY_b = 98,
    KEY_c = 99,
    KEY_d = 100,
    KEY_e = 101,
    KEY_f = 102,
    KEY_g = 103,
    KEY_h = 104,
    KEY_i = 105,
    KEY_j = 106,
    KEY_k = 107,
    KEY_l = 108,
    KEY_m = 109,
    KEY_n = 110,
    KEY_o = 111,
    KEY_p = 112,
    KEY_q = 113,
    KEY_r = 114,
    KEY_s = 115,
    KEY_t = 116,
    KEY_u = 117,
    KEY_v = 118,
    KEY_w = 119,
    KEY_x = 120,
    KEY_y = 121,
    KEY_z = 122,
    KEY_OTHER
};
#include <termios.h>
static struct termios saved_termios;

/*
int rk_readkey(enum keys *key);
анализирует последовательность символов (возвращаемых функцией read при чтении с
терминала) и возвращает первую клавишу, которую нажал
пользователь. В качестве параметра в функцию передаётся адрес
переменной, в которую возвращается номер нажатой (enum keys
– перечисление распознаваемых клавиш);
*/

int rk_readkey(enum keys* key);

/*
сохраняет текущие параметры
терминала
*/
int rk_mytermsave(void);

/*
Восстанавливает сохраненные параметры терминала
*/

int rk_mytermrestore(void);

/*
int rk_mytermregime (int regime, int vtime, int
vmin, int echo, int sigint) - переключает терминала
между режимами. Для неканонического режима используются значения второго и
последующего параметров.
*/

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);

/*
– обеспечивает ввод с клавиатуры значения в соответствии с форматом
команд и ограничением на вводимые символы.
*/
int rk_readvalue(int* value, int timeout);
#endif