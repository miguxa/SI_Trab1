// Trabalho1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "interface.h"


/*  Função que calibra o armazém.
Move o braço no eixo y, z e x (por esta ordem) até activar algum sensor.
retorna 0 para controlo.
*/
int calib() {
	/*****Calibração*****/

	/*Inicializa as variáveis P1, P2 e P3 com o valor dos portos 1, 2 e 4 respectivamente*/
	uInt8 P1 = ReadDigitalU8(1);
	uInt8 P2 = ReadDigitalU8(2);
	uInt8 P4 = ReadDigitalU8(4);
	uInt8 Aux = 0;

	/* Exemplo explicativo
	Aux = setBitValue(PC, A, B);
	WriteDigitalU8(C, Aux);
	lê o valor existente em PC e escreve no bit A o valor B.
	depois de alterado o valor de PC, escreve de novo no porto C.
	*/
	Aux = setBitValue(P4, 3, 1);
	WriteDigitalU8(4, Aux);
	/*condição do sensor "conveyor at center"*/
	while (getBitValue(P1, 3))
		P1 = ReadDigitalU8(1);
	Aux = setBitValue(P4, 3, 0);
	WriteDigitalU8(4, Aux);


	Aux = setBitValue(P4, 6, 1);
	WriteDigitalU8(4, Aux);
	/*condição do sensor "z+N"*/
	while (getBitValue(P2, 6) && getBitValue(P2, 4) && getBitValue(P2, 2) && getBitValue(P2, 0) && getBitValue(P1, 6)) {
		P1 = ReadDigitalU8(1);
		P2 = ReadDigitalU8(2);
	}
	Aux = setBitValue(P4, 6, 0);
	WriteDigitalU8(4, Aux);


	Aux = setBitValue(P4, 0, 1);
	WriteDigitalU8(4, Aux);
	/*condição do sensor "x+N"*/
	while (in_port(0) == 255 && getBitValue(P1, 0) && getBitValue(P1, 1)) {
		P1 = ReadDigitalU8(1);
	}
	Aux = setBitValue(P4, 0, 0);
	WriteDigitalU8(4, Aux);

	return 0;
}

/********/

/********/

int main()
{
	printf("Hello");

	create_DI_channel(0);
	create_DI_channel(1);
	create_DI_channel(2);
	create_DI_channel(3);
	create_DO_channel(4);
	create_DO_channel(5);
	
	calib();

	/*
	uInt8 P = ReadDigitalU8(4); //lê o valor do porto 4

	//imprime o bit X da variável P
	for (int X = 7; X >= 0; X--) {
		printf("%d\n", getBitValue(P, X));
	}

	getchar();

	//lê o que está na variável P e mete no bit 3, o valor 1
	uInt8 A = setBitValue(P, 3, 1);
	//escreve o valor de A no porto 0
	WriteDigitalU8(0, A);
	*/
	getchar();

	return 0;
}