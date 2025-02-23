#include "sc_reg.h"

static int regFlags;

/*
int sc_regInit (void) – инициализирует регистр флагов
значениями по умолчанию;
*/

int sc_regInit(void)
{
    regFlags = 0;
    return 0;
}