// Trabalho1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "interface.h"
#include <conio.h>

bool getBitValue(uInt8 value, uInt8 n_bit);
int setBitValue(uInt8 base_val, int bit_nr, int bit_val);
int calib();
void test_func();
void stop_LStation();
void stop_RStation();


/********/

int main()
{
	create_DI_channel(0);
	create_DI_channel(1);
	create_DI_channel(2);
	create_DI_channel(3);
	create_DO_channel(4);
	create_DO_channel(5);

	//test_func();
	
	calib();
	
	
	
	/*
	int p_val1 = 0, p_val2 = 0;
	int c = 0;

	p_val1 = in_port(1);
	out_port(4, 8);
	while (c != 'x') {
		c = getch();
	}
	out_port(4, 0);
	*/

	printf("\nPress a key to exit\n");
	getch();
	close_channels();
	return 0;
}


bool getBitValue(uInt8 value, uInt8 n_bit) {
	return (value & (1 << n_bit));
}

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


/*  Função que calibra o armazém.
Move o braço no eixo y, z e x (por esta ordem) até activar algum sensor.
retorna 0 para controlo.
*/
int calib() {
	/*****Calibração*****/
	int p_val1 = 0, p_val2 = 0;

	p_val1 = in_port(1);
	out_port(4, 8);
	while (p_val1 == 255) {
		p_val1 = in_port(1);
	}
	out_port(4, 0);


	p_val1 = in_port(0);
	p_val2 = in_port(1);
	out_port(4, 1);
	while (p_val1 == 255 && getBitValue(p_val1, 0) && getBitValue(p_val1, 1)) {
		p_val1 = in_port(0);
		p_val2 = in_port(1);
	}
	out_port(4, 0);


	p_val1 = in_port(1);
	p_val2 = in_port(2);
	out_port(4, 64);

	while (getBitValue(p_val2, 6) && getBitValue(p_val2, 4) && getBitValue(p_val2, 2) && getBitValue(p_val2, 0) && getBitValue(p_val1, 6)) {
		p_val1 = in_port(1);
		p_val2 = in_port(2);
	}
	
	out_port(4, 0);

	return 0;
}

/*Funcao de teste*/
void test_func() {

	int c = 0, p_val = 0;
	/////////////////////////////////////////////////////
	// test in_port
	printf("Test in_port\n");
	while (c != 'x') {
		p_val = in_port(0);
		printf("Port Value - %2x\n", p_val);
		printf("qq tecla. x p/ terminar\n");
		c = getch();
	}
	/////////////////////////////////////////////////////
	// test in_port
	printf("Test out_port\n");
	c = getch();
	out_port(5, 1);
	printf("Left going out\n");
	c = getch();
	out_port(5, 0);
	printf("Done! Bye 4 now \n");
}

void stop_LStation() {
	uInt8 P4 = ReadDigitalU8(4);
	uInt8 P5 = ReadDigitalU8(5);
	int Aux;

	Aux = setBitValue(P4, 7, 0);
	WriteDigitalU8(4, Aux);
	Aux = setBitValue(P5, 0, 0);
	WriteDigitalU8(5, Aux);
}

void stop_RStation() {
	uInt8 P5 = ReadDigitalU8(5);
	int Aux;

	Aux = setBitValue(P5, 1, 0);
	WriteDigitalU8(5, Aux);
	P5 = ReadDigitalU8(5);
	Aux = setBitValue(P5, 2, 0);
	WriteDigitalU8(5, Aux);
}