#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "cmsis_os.h"
#include "calibrate.h"
#include "pid.h"
#include "sys.h"

void execute_task(const void* argu)
{
	qidongInit();
	test_moto_init();
   while(1)
  {
		qidong();
		test_moto_control();
  }
}
