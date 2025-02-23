#include "myTerm.h"
#include <unistd.h>
#include "myTerm.h"

int mt_delline(void)
{
    const char* del = "\033[2K";
    if (write(1, del, 4) == -1) {
        return -1;
    }
    return 0;
}