// INICIO PORTOS
// Material obtido da disciplina de TRSA
// J. Rosas

#include "stdafx.h"

#include< stdlib.h>
#include <stdio.h>
#include<conio.h>
//#include <iostream.h>		
#include <signal.h>
#include <winsock.h>
#include "interface.h"		


//#define _USE_REAL_KIT_    


 

#define SERV_TCP_PORT	9999				// porta do servidor
#define SERV_HOST_ADDR	"127.0.0.1"	   // endereco do servidor
 


int 		socket_ligado=0;
SOCKET 	sockfd;
struct 	sockaddr_in	serv_addr;
WSADATA	WSAData;
int		WSAErr;



     

void terminar_ligacao(void)   
{  
	socket_ligado=0;	
	if (sockfd) 
		closesocket(sockfd);	// desligar coneccao ao simulador
	WSACleanup();				// fazer house-keeping
} 

void sighandle(int signum) 
{ 
	if(signum){};  //avoid warning
	// funcoes sig_kill (very nice)
	terminar_ligacao();
	printf("Exiting due to signal(%d)\n",signum);
	exit(0); 		
}

void estabelecer_ligacao(void)
{
	socket_ligado=1;	
	
	signal(SIGABRT, sighandle);
	signal(SIGFPE, sighandle);
	signal(SIGILL, sighandle);
	signal(SIGINT, sighandle);
	signal(SIGSEGV, sighandle);
	signal(SIGTERM, sighandle);
	signal(SIGBREAK, sighandle);

	serv_addr.sin_family			= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr(SERV_HOST_ADDR);
	serv_addr.sin_port			= htons(SERV_TCP_PORT);
	if((WSAErr=WSAStartup(0x101,(LPWSADATA)&WSAData))!=0)
	{		
		printf("\nKit not found (%d). Turn it on...",WSAErr);		
		exit(-1);
	}
	sockfd=socket( AF_INET , SOCK_STREAM , 0 );
	if( (int)sockfd < 0 )
	{
		printf("\nKit not found (%d). Turn it on...",WSAErr);		
		exit(-1);
	}
	if( connect( sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
	{
		printf("\nKit not found (%d). Turn it on...",WSAErr);		
		exit(-1);
	}
	else
	{
		printf("\nKit has been found :-) ...");		
	}
}




void sendMessage(char *message)
{
	
	if(!socket_ligado)
		estabelecer_ligacao();

//	Sleep(1);
	if( send(sockfd, message,strlen( message),0)==SOCKET_ERROR)
	{
		int WSERR = WSAGetLastError();
		printf("\nError sending data (%d)...",WSAErr);		
	}	
		
}


void receiveMessage(char *message)
{
	int n;		
	char buf[100];
	if(!socket_ligado)
		estabelecer_ligacao();
	
//	Sleep(1);
	if ( ( n = recv( sockfd, buf, sizeof(buf), 0) ) > 0 )
	{
		buf[n]='\0';	
		strcpy(message,(const char *)buf);
	}
	else if ( n == SOCKET_ERROR )
	{
		int WSERR = WSAGetLastError();
		printf("\nError receiving data (%d)...",WSAErr);		
	}	
	
}





/*****************SIMULATOR ACCESS CODE *******************/
bool sim_create_DI_channel(uInt32 port)
{
	char buff[100];
	sprintf(buff,"%s %d %d","create_DI_channel", port, 0);
	sendMessage(buff);
	receiveMessage(buff); //receive 'done'
	return(true);
}


bool sim_create_DO_channel(uInt32 port)
{
	char buff[100];
	sprintf(buff,"%s %d %d","create_DO_channel", port, 0);
	sendMessage(buff);
	receiveMessage(buff); //receive 'done'
	return(true);
}



uInt8 sim_ReadDigitalU8(uInt32 port)
{
	char buff[100];
	char lixo[100];
	uInt32 value;

	sprintf(buff,"%s %02X %02X","ReadDigitalU8", port, 0);
	
	sendMessage(buff);	

	receiveMessage(buff);

	
	sscanf(buff,"%s %02X %02X",lixo, &port, &value);
	return((uInt8) value);
}



void sim_WriteDigitalU8(uInt32 port, uInt8 data)
{
	char buff[100];
	sprintf(buff,"%s %02X %02X","WriteDigitalU8", port, data);	
	sendMessage(buff);
	
	receiveMessage(buff); //receive 'done'	
}


bool sim_close_channels()
{
	terminar_ligacao();
	return(true);
}





/*****************REAL KIT CODE *******************/


TaskHandle tasks[50];


bool close_channels(void)
{
#ifdef _USE_REAL_KIT_
	return(true);
#else		
	return sim_close_channels();
#endif 

}



bool create_DI_channel(uInt32 port)
{
	#ifdef _USE_REAL_KIT_
		char taskName[100];
		char portName[100];
		//printf("\n1");
		TaskHandle task_handle;
		sprintf(taskName,"task_di__%d",port);
		//printf("\n2");
		sprintf(portName,"Dev1/port%d",port);  
		//printf("\n3");
   		DAQmxCreateTask(taskName,&task_handle);
		//printf("\n4");
		DAQmxCreateDIChan(task_handle,portName,"",DAQmx_Val_ChanForAllLines);
		//printf("\n5");
		DAQmxStartTask(task_handle);
		//printf("\n6");
		tasks[port]=task_handle;			
		//printf("\n7 %x", task_handle);
		return(true);		
	#else		
		return sim_create_DI_channel(port);
#endif 
}



bool create_DO_channel(uInt32 port)
{
	#ifdef _USE_REAL_KIT_
		char taskName[100];
		char portName[100];
		TaskHandle task_handle;	
		sprintf(taskName,"task_di__%d",port);
		sprintf(portName,"Dev1/port%d",port);   
		DAQmxCreateTask(taskName,&task_handle);
		DAQmxCreateDOChan(task_handle,portName,"",DAQmx_Val_ChanForAllLines);
		DAQmxStartTask(task_handle);		
		tasks[port]=task_handle;
#else
		return sim_create_DO_channel(port);
#endif
   return(true);	
}



uInt32 portos[20];


uInt8 ReadDigitalU8(uInt32 port)
{
	uInt8 data=0;
#ifdef _USE_REAL_KIT_
	int32		error_0=0;			
	char		errBuff_0[2048]={'\0'};
	int32		read_0;
	TaskHandle task_handle = tasks[port];
	DAQmxReadDigitalU8(task_handle,1,10.0,DAQmx_Val_GroupByChannel,&data,1,&read_0,NULL);				
#else 
	data=sim_ReadDigitalU8(port);
#endif

/*
	bool flag_error=false;
	if( (port==0) && !(portos[port] & 1<<2) && (data & 1<<2) && (portos[2]&1<<6))
		{printf("\nATTENTION: XX beyond left limit...\n");flag_error=true;}
	if( (port==0) && !(portos[port] & 1<<0) && (data & 1<<0) && (portos[2]&1<<7))
		{printf("\nATTENTION: XX beyond right limit...\n");flag_error=true;}
	if( (port==1) && !(portos[port] & 1<<3) && (data & 1<<3) && (portos[2]&1<<2))
		{printf("\nATTENTION: ZZ below lower limit...\n");flag_error=true;}
	if( (port==0) && !(portos[port] & 1<<6) && (data & 1<<6) && (portos[2]&1<<3))
		{printf("\nATTENTION: ZZ above upper limit...\n");flag_error=true;}
	if( (port==0) && !(portos[port] & 1<<5) && (data & 1<<5) && (portos[2]&1<<4))
		{printf("\nATTENTION: YY beyond OUTSIDE sensor...\n");	flag_error=true;}
	if( (port==0) && !(portos[port] & 1<<3) && (data & 1<<3) && (portos[2]&1<<5))
		{printf("\nATTENTION: YY beyond INSIDE sensor...\n");flag_error=true;}

	portos[port]=data;	
	if(flag_error) {
		WriteDigitalU8(2, 0);
		exit(0);
	}
	*/
	return(data);
}


void WriteDigitalU8(uInt32 port, uInt8 data)
{

	if(ReadDigitalU8(0) & 1<<4) 
	{
      /*if(   (data & 1<<6) || (data & 1<<7))          {
         printf("\nATTENTION: Moving along xx without cage at CENTER...\n");
		 WriteDigitalU8(2, 0);
         exit(0); 
      }
	  */
	  /*
	  if(   (data & 1<<2) || (data & 1<<3))          {
         printf("\nATTENTION: Moving along zz without cage at CENTER...\n");
		 WriteDigitalU8(2, 0);
         exit(0); 
      }	*/  		
	}
    portos[port]=data;


#ifdef _USE_REAL_KIT_ 
		int         error_1=0;
		char        errBuff_1[2048]={'\0'};
		int32		written_1;
		TaskHandle task_handle = tasks[port];		
		DAQmxWriteDigitalU8(task_handle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,&written_1,NULL);	
#else 
		sim_WriteDigitalU8(port, data);		
#endif

}



/**************SI / CELSON ************************/
void init_cpld2()
{
  create_DI_channel(0);
  create_DI_channel(1);
  create_DO_channel(2);
}


unsigned char in_port(int port)
{
   return ReadDigitalU8(port);				
}


void out_port(int port, unsigned char byte)
{
  WriteDigitalU8(port, byte);
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

/*
void putPiece(int col, int line)
{
  unsigned char p = in_port(12+line);
  p = p | 1<<col; 	
  out_port(12+line, p); 	
}



void getPiece(int col, int line)
{
  unsigned char p = in_port(12+line);
  unsigned char mask_on  = 1 << col;	
  unsigned char mask_off = (unsigned char)0xff - mask_on;

  p = p &  mask_off; 	
  out_port(12+line, p); 	
}


void Goto(int col, int line, int action)
{

}

*/
/*******************************************/

