#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "cmsis_os.h"
#include "calibrate.h"
#include "pid.h"
#include "sys.h"


void execute_task(const void* argu)
{
   //���Ե����ʼ��	
   //test_moto_init();
	// write_led_io(LED_IO1,LED_ON);
   while(1)
  {
   //���Ե�����Ƴ���
   // test_moto_control();
    //osDelay(5);
  }
}
