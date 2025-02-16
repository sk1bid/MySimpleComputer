#include "sc_reg.h"

extern int instructionCounter;

int sc_icounterInit(void){
    instructionCounter = 0;
    return 0;
}