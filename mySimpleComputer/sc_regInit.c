#include "sc_reg.h"

static int regFlags;
static int accumulator;
static int instructionCounter;

/*
int sc_regInit (void) – инициализирует регистр флагов
значениями по умолчанию;
*/

int sc_regInit(void)
{
    regFlags = 0;
    return 0;
}