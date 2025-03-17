#include "../myReadKey/myReadKey.h"
#include <unistd.h>
#include <termios.h>


int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint) {
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        return -1;
    }
    if (regime == 0) { // Non-canonical mode
        term.c_lflag &= ~(ICANON | (echo ? 0 : ECHO) | (sigint ? 0 : ISIG));
        term.c_cc[VMIN] = vmin;
        term.c_cc[VTIME] = vtime;
    } else { // Canonical mode
        term.c_lflag |= ICANON | ECHO | ISIG;
    }
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        return -1;
    }
    return 0;
}