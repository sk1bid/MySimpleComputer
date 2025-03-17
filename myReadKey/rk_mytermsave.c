#include "../myReadKey/myReadKey.h"
#include <unistd.h>

int rk_mytermsave(void)
{
    if (tcgetattr(STDIN_FILENO, &saved_termios) == -1) {
        return -1;
    }
    return 0;
}