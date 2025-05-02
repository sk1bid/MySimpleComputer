#include "myBigChars.h"
#include "stdio.h"

int bc_strlen(char* str)
{
    if (str == NULL) {
        return -1;
    }
    int count = 0;
    for (int i = 0; str[i] != '\0';) {
        unsigned char c = (unsigned char)str[i];
        if (c < 0x80) { // 1 byte
            i++;
        } else if ((c & 0xE0) == 0xC0) { // 2 bytes
            if ((str[i + 1] & 0xC0) != 0x80) {
                return -1;
            }
            i += 2;
        } else if ((c & 0xF0) == 0xE0) { // 3 bytes
            if (((str[i + 1] & 0xC0) != 0x80)
                || ((str[i + 2] & 0xC0) != 0x80)) {
                return -1;
            }
            i += 3;
        } else if ((c & 0xF8) == 0xF0) { // 4 bytes
            if (((str[i + 1] & 0xC0) != 0x80) || ((str[i + 2] & 0xC0) != 0x80)
                || ((str[i + 3] & 0xC0) != 0x80)) {
                return -1;
            }
            i += 4;
        } else {
            return -1;
        }
        count++;
    }
    return count;
}