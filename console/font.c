#include "../myBigChars/myBigChars.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int bigchars[18][2]
            = {{1717986942, 2120640102},
                {404240408, 2115508248},
                {2114324094, 2120245344},
                {503735870, 1046873630},
                {1717986918, 101088894},
                {2120245374, 2114323974},
                {2120245374, 2120640102},
                {101058174, 101058054},
                {2120640126, 2120640102},
                {2120640126, 2114323974},
                {2120640126, 1717986918},
                {2087085692, 2087085670},
                {1616928894, 2120245344},
                {1717986940, 2087085670},
                {2120245374, 2120245344},
                {2120245374, 1616928864},
                {2115508224, 1579134},
                {2113929216, 126}};
    int fd = open("font.bin", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening font.bin");
        return -1;
    }
    for (int i = 0; i < 18; i++) {
        if (bc_bigcharwrite(fd, bigchars[i], 1) != 0) {
            close(fd);
            perror("Error writing to font.bin");
            return -1;
        }
    }

    close(fd);
    return 0;
}