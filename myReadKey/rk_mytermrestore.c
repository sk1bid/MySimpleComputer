#include "../myReadKey/myReadKey.h"
#include <unistd.h>

int rk_mytermrestore(void) {
    if (tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios) == -1) {
        return -1;
    }
    return 0;
}