// Trabalho1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "interface.h"
#include <conio.h>

bool AUX = 1;
void INIT();
void test_func();
void move_right();
void move_left();
void stop_x();
void stop_y();
void move_inside();
void move_outside();
void stop_z();
void move_up();
void move_down();
void stop_r();
void take_r();
void give_r();
void stop_l();
void take_l();
void give_l();
void stop_all();
bool Planner();
void Dispatch();

struct {
	bool x_left = 0;
	bool x_right = 0;
	bool x_stop = 0;

	void XX (bool left, bool right, bool stop) {
		x_left = left;
		x_right = right;
		x_stop = stop;
	}
} X;

struct {
	bool y_inside = 0;
	bool y_outside = 0;
	bool y_stop = 0;

	void YY(bool inside, bool outside, bool stop) {
		y_inside = inside;
		y_outside = outside;
		y_stop = stop;
	}
} Y;

struct {
	bool z_up = 0;
	bool z_down = 0;
	bool z_stop = 0;

	void ZZ(bool up, bool down, bool stop) {
		z_up = up;
		z_down = down;
		z_stop = stop;
	}
} Z;

struct {
	bool l_give = 0;
	bool l_take = 0;
	bool L_stop = 0;

	void LL(bool give, bool take, bool stop) {
		l_give = give;
		l_take = take;
		L_stop = stop;
	}
} L;

struct {
	bool r_give = 0;
	bool r_take = 0;
	bool R_stop = 0;

	void RR(bool give, bool take, bool stop) {
		r_give = give;
		r_take = take;
		R_stop = stop;
	}
} R;

struct {
	uInt32 P0 = ReadDigitalU8(0);
	uInt32 P1 = ReadDigitalU8(1);
	uInt32 P2 = ReadDigitalU8(2);
	
	bool x0;
	bool x1;
	bool x2;
	bool x3;
	bool x4;
	bool x5;
	bool x6;
	bool x7;
	bool x8;
	bool x9;

	bool z4u;
	bool z4d;
	bool z3u;
	bool z3d;
	bool z2u;
	bool z2d;
	bool z1u;
	bool z1d;
	bool z0u;
	bool z0d;

	bool yinside;
	bool ycenter;
	bool youtside;

	void xSensors() {
		ports.refreshSensors();
		printf("\nSensores do eixo do X:\n");
		printf("x0 - %d \n", x0);
		printf("x1 - %d \n", x1);
		printf("x2 - %d \n", x2);
		printf("x3 - %d \n", x3);
		printf("x4 - %d \n", x4);
		printf("x5 - %d \n", x5);
		printf("x6 - %d \n", x6);
		printf("x7 - %d \n", x7);
		printf("x8 - %d \n", x8);
		printf("x9 - %d \n", x9);
	}

	void zSensors() {
		ports.refreshSensors();
		printf("\nSensores do eixo do Z:");
		printf("\nz4u - %d  ", z4u);
		printf("\nz4d - %d\n", z4d);
		printf("\nz3u - %d  ", z3u);
		printf("\nz3d - %d\n", z3d);
		printf("\nz2u - %d  ", z2u);
		printf("\nz2d - %d\n", z2d);
		printf("\nz1u - %d  ", z1u);
		printf("\nz1d - %d\n", z1d);
		printf("\nz0u - %d  ", z0u);
		printf("\nz0d - %d\n", z0d);
	}

	void ySensors() {
		ports.refreshSensors();
		printf("\nSensores do eixo do Y:");
		printf("\nyinside  - %d ", yinside );
		printf("\nycenter  - %d ", ycenter );
		printf("\nyoutside - %d ", youtside);
	}

	void refreshSensors() {
		P0 = ReadDigitalU8(0);
		P1 = ReadDigitalU8(1);
		P2 = ReadDigitalU8(2);

		x0 = getBitValue(P0, 0);
		x1 = getBitValue(P0, 1);
		x2 = getBitValue(P0, 2);
		x3 = getBitValue(P0, 3);
		x4 = getBitValue(P0, 4);
		x5 = getBitValue(P0, 5);
		x6 = getBitValue(P0, 6);
		x7 = getBitValue(P0, 7);
		x8 = getBitValue(P1, 0);
		x9 = getBitValue(P1, 1);

		z4u = getBitValue(P1, 5);
		z4d = getBitValue(P1, 6);
		z3u = getBitValue(P1, 7);
		z3d = getBitValue(P2, 0);
		z2u = getBitValue(P2, 1);
		z2d = getBitValue(P2, 2);
		z1u = getBitValue(P2, 3);
		z1d = getBitValue(P2, 4);
		z0u = getBitValue(P2, 5);
		z0d = getBitValue(P2, 6);

		yinside = getBitValue(P1, 4);
		ycenter = getBitValue(P1, 3);
		youtside = getBitValue(P1, 2);
	}
} ports;

int main()
{
	INIT();
	while (AUX) {
		AUX = Planner();
		Dispatch();
	}
	printf("\n\n\nPress any key to exit");
	getch();
	close_channels();
	return 0;
}

/*Contem inicialização dos portos e calibração*/
void INIT() {
	/*Inicialização dos canais de Input e Output*/
	create_DI_channel(0);
	create_DI_channel(1);
	create_DI_channel(2);
	create_DI_channel(3);
	create_DO_channel(4);
	create_DO_channel(5);

	/*****Calibração*****/
	/*  Função que calibra o armazém.
	Move o braço no eixo y, z e x (por esta ordem) até activar algum sensor.
	retorna 0 para controlo.
	*/
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

}

/*Contem um conjunto de if's encadeados para controlar o armazem a partir do teclado*/
bool Planner() {

	char c = getch();
	switch (c) {
	case 'd':
		move_right();
		break;
	case 'a':
		move_left();
		break;
	case 's':
		move_down();
		break;
	case 'w':
		move_up();
		break;
	case 'q':
		move_inside();
		break;
	case 'e':
		move_outside();
		break;
	case 't':
		give_l();
		break;
	case 'r':
		take_l();
		break;
	case 'g':
		give_r();
		break;
	case 'f':
		take_r();
		break;
	case ' ': /*Barra de espaço*/
		stop_all();
		break;
	case 27 : /*ESC*/
		stop_all();
		return 0;
		break;
	default:
		break;
	}
	
	return 1;
}

/*Altera o valor dos portos, consoante a instrução dada pelo utilizador*/
void Dispatch() {
	if (X.x_right == 1) 
		out_port(4, 1);
	if (X.x_left == 1)
		out_port(4, 2);
	if (Y.y_inside == 1)
		out_port(4, 16);
	if (Y.y_outside == 1)
		out_port(4, 8);
	if (Z.z_up == 1)
		out_port(4, 32);
	if (Z.z_down == 1)
		out_port(4, 64);
	if (R.r_give == 1)
		out_port(5, 4);
	if (R.r_take == 1)
		out_port(5, 2);
	if (L.l_give == 1) {
		out_port(4, 0);
		out_port(5, 1);
	}
	if (L.l_take == 1) {
		out_port(5, 0);
		out_port(4, 128);
	}
	if (R.R_stop || L.L_stop || X.x_stop || Y.y_stop || Z.z_stop) {
		out_port(5, 0);
		out_port(4, 0);
	}
}

void move_right() {
	if (X.x_right == 1) 
		stop_x();
	else 
		X.XX(0, 1, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void move_left() {
	if (X.x_left == 1)
		stop_x();
	else
		X.XX(1, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void stop_x() {
	X.XX(0, 0, 1);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void move_inside() {
	if (Y.y_inside == 1)
		stop_y();
	else
		Y.YY(1, 0, 0);
	X.XX(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void move_outside() {
	if (Y.y_outside == 1)
		stop_y();
	else
		Y.YY(0, 1, 0);
	X.XX(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void stop_y() {
	X.XX(0, 0, 0);
	Y.YY(0, 0, 1);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void move_up() {
	if (Z.z_up == 1)
		stop_z();
	else 
		Z.ZZ(1, 0, 0);
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void move_down() {
	if (Z.z_down == 1)
		stop_z();
	else
		Z.ZZ(0, 1, 0);
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void stop_z() {
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 1);
	L.LL(0, 0, 0);
	R.RR(0, 0, 0);
}

void give_l() {
	if (L.l_give == 1)
		stop_l();
	else
		L.LL(1, 0, 0);
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	R.RR(0, 0, 0);
}

void take_l() {
	if (L.l_take == 1)
		stop_l();
	else
		L.LL(0, 1, 0);
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	R.RR(0, 0, 0);
}

void stop_l() {
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 1);
	R.RR(0, 0, 0);
}

void give_r() {
	if (R.r_give == 1)
		stop_r();
	else
		R.RR(1, 0, 0);
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
}

void take_r() {
	if (R.r_take == 1)
		stop_r();
	else
		R.RR(0, 1, 0);
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
}

void stop_r() {
	X.XX(0, 0, 0);
	Y.YY(0, 0, 0);
	Z.ZZ(0, 0, 0);
	L.LL(0, 0, 0);
	R.RR(0, 0, 1);
}

void stop_all() {
	stop_x();
	stop_y();
	stop_z();
	stop_l();
	stop_r();
}
