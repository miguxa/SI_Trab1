#include "interface.h"

bool getBitValue(uInt8 value, uInt8 n_bit);
int setBitValue(uInt8 base_val, int bit_nr, int bit_val);

void stop_Lstation();
void stop_Rstation();
void move_inStation(int i);
void move_outStation(int i);
