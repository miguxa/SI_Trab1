
#ifdef _WIN64
#include"NIDAQmx64.h"
#else
#include"NIDAQmx.h"
#endif 

bool create_DI_channel(uInt32 port);
bool create_DO_channel(uInt32 port);
uInt8 ReadDigitalU8(uInt32 port);
void WriteDigitalU8(uInt32 port, uInt8 data);
bool close_channels();



void			init_cpld2();
unsigned char	in_port(int port);
void			out_port(int port, unsigned char byte);
void putPiece(int z, int x);
void getPiece(int z, int x);


bool getBitValue(uInt8 value, uInt8 n_bit);
int setBitValue(uInt8 base_val, int bit_nr, int bit_val);