#pragma once


#define SHOOT		0b0001
#define MOVE_RIGHT	0b0010
#define MOVE_LEFT	0b0100


bool isShootBitSet(int number);
bool isRightBitSet(int number);
bool isLeftBitSet(int number);

void fn_vSetShoot(int number);
void fn_vSetRight(int number);
void fn_vSetLeft(int number);
