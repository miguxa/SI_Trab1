#include "WarehouseControl.h"
#include "interface.h"


bool getBitValue(uInt8 value, uInt8 n_bit) {
	return (value & (1 << n_bit));
}
/********************************************************************************************************/

int setBitValue(uInt8 base_val, int bit_nr, int bit_val) { // coloca no bit_nr o valor 1 ou 0
	int ret;
	int mask_on = 1 << bit_nr;
	int mask_off = 0xff - (1 << bit_nr);
	if (bit_val) {
		ret = base_val | mask_on;
	}
	else {
		//mask = ~mask;
		ret = base_val & mask_off;
	}
	return ret;
}
/********************************************************************************************************/

void stop_Lstation() {
	uInt8 P4 = ReadDigitalU8(4);
	uInt8 P5 = ReadDigitalU8(5);
	uInt8 Aux;

	Aux = setBitValue(P4, 7, 0);
	WriteDigitalU8(4, Aux);
	Aux = setBitValue(P5, 0, 0);
	WriteDigitalU8(5, Aux);
}
/********************************************************************************************************/

void stop_Rstation() {
	uInt8 P5 = ReadDigitalU8(5);
	uInt8 Aux;

	Aux = setBitValue(P5, 1, 0);
	WriteDigitalU8(5, Aux);
	P5 = ReadDigitalU8(5);
	Aux = setBitValue(P5, 2, 0);
	WriteDigitalU8(5, Aux);
}
/********************************************************************************************************/

void move_inStation(int i) {
	if (i == 1) {
		uInt8 P4 = ReadDigitalU8(4);
		uInt8 Aux;

		Aux = setBitValue(P4, 7, 1);
		WriteDigitalU8(4, Aux);
	}
	if (i == 2) {
		uInt8 P5 = ReadDigitalU8(5);
		uInt8 Aux;

		Aux = setBitValue(P5, 1, 1);
		WriteDigitalU8(5, Aux);
	}
}
/********************************************************************************************************/

void move_outStation(int i) {
	if (i == 1) {
		uInt8 P5 = ReadDigitalU8(5);
		uInt8 Aux;

		Aux = setBitValue(P5, 0, 1);
		WriteDigitalU8(5, Aux);
	}
	if (i == 2) {
		uInt8 P5 = ReadDigitalU8(5);
		uInt8 Aux;

		Aux = setBitValue(P5, 2, 1);
		WriteDigitalU8(5, Aux);
	}
}