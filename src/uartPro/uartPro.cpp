#include "string.h"
#include "uartPro.h"

uartPro::uartPro(byte PRAIn, byte PRBIn, void (*function)(packet)) 
: PRA(PRAIn), PRB(PRBIn)
{
}

void uartPro::decode(byte *dataIn) {

}

packet uartPro::encode(packet) {

}