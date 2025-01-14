

#include "Player_Actions.h"


bool isShootBitSet(int number) {
    return (number & SHOOT) != 0; // Check the relevant bit
}


bool isRightBitSet(int number) {
    return (number & MOVE_RIGHT) != 0; // Check the relevant bit
}


bool isLeftBitSet(int number) {
    return (number & MOVE_LEFT) != 0; // Check the relevant bit
}




void fn_vSetShoot(int number)
{
    int bitPosition = 0; // Position to set (0-based, therefore bit 0 is the first one)
    number |= (1 << bitPosition); // Set the bit to 1
}

void fn_vSetRight(int number)
{
    int bitPosition = 1; // Position to set (0-based, therefore bit 0 is the first one)
    number |= (1 << bitPosition); // Set the bit to 1
}

void fn_vSetLeft(int number)
{
    int bitPosition = 2; // Position to set (0-based, therefore bit 0 is the first one)
    number |= (1 << bitPosition); // Set the bit to 1
}






