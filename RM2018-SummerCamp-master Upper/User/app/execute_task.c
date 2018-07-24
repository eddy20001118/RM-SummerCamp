#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "cmsis_os.h"
#include "calibrate.h"
#include "pid.h"
#include "sys.h"
uint8_t recv[3];
uint8_t p=0,i=0,d=0;
void callback(void){
	p = HexToChar(recv[0]);
	i = HexToChar(recv[1]);
	d = HexToChar(recv[2]);
}

unsigned char HexToChar(unsigned char bChar){
    if((bChar>=0x30)&&(bChar<=0x39))
        bChar -= 0x30;
    else if((bChar>=0x41)&&(bChar<=0x46))//大写字母
        bChar -= 0x37;
    else if((bChar>=0x61)&&(bChar<=0x66))//小写字母
        bChar -= 0x57;
    else bChar = 0xff;
    return bChar;
}

void execute_task(const void* argu)
{
	qidongInit();
	test_moto_init();
	uart_init(USER_UART2,9600,WORD_LEN_8B,STOP_BITS_1,PARITY_NONE);
  uart_recv_callback_register(USER_UART2,callback);
	//uart_receive_start(USER_UART2,recv,3);
	while(1)
  {
		uart_receive_start(USER_UART2,recv,3);
		qidong();
		test_moto_control();
  }
}
