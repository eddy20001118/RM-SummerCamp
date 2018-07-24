#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "pid.h"
#include "sys.h"
#include  "gimbal_task.h"
#include "cmsis_os.h"

#define DIG_DISCONNECT  1
#define DIG_CONNECT   0
#define QD1 1
#define QD2 2


int16_t test_moto_speed[4] = {0,0,0,0};
int16_t test_moto_current[4];
int16_t UpperIntake_speed = 0;
int16_t UpperIntake_current;
uint16_t MAX_VALUE[2]={8500,18000}; //1-3508,2-2006


void test_moto_control(void)
{
 /*
  ch1-右侧左右
	ch2-右侧上下
	ch3-左侧左右
	ch4-左侧上下
	*/
   test_moto_speed[0] = rc.ch2/RC_MAX_VALUE*MAX_VALUE[1];  //chanel 2 to control 2006[left]-id1
   test_moto_speed[1] = -rc.ch2/RC_MAX_VALUE*MAX_VALUE[1]; //chanel 2 to control 2006[right]-id2
   test_moto_speed[2] = rc.ch4/RC_MAX_VALUE*MAX_VALUE[0];  //chanel 4 to control lift3508[left]-id3
   test_moto_speed[3] = -rc.ch4/RC_MAX_VALUE*MAX_VALUE[0];  //chanel 4 to control lift3508[right]-id4
	 UpperIntake_speed = rc.ch3/RC_MAX_VALUE*MAX_VALUE[1];    //chanel 3 to control UpperIntake3508-id5

	
    test_moto_current[0] = pid_calc(&pid_2006_Left, moto_chassis[0].speed_rpm, test_moto_speed[0]);  
	  test_moto_current[1] = pid_calc(&pid_2006_Right, moto_chassis[1].speed_rpm, test_moto_speed[1]);
	  test_moto_current[2] = pid_calc(&pid_3508_Left,moto_chassis[2].speed_rpm, test_moto_speed[2]);
	  test_moto_current[3] = pid_calc(&pid_3508_Right,moto_chassis[3].speed_rpm, test_moto_speed[3]);
  
 	UpperIntake_current = pid_calc(&pid_yaw_speed,moto_yaw.speed_rpm,UpperIntake_speed);
   
	 set_test_motor_current(test_moto_current);
	 set_gib_motor_current(UpperIntake_current);
}

void qidongInit(void)
{
	set_digital_io_dir(QD1,IO_OUTPUT);
	set_digital_io_dir(QD2,IO_OUTPUT);
}


void qidong(void)
{	
	if(rc.ch1 > 0){
	write_digital_io(QD1,DIG_CONNECT);
	write_digital_io(QD2,DIG_CONNECT);
	} 
	else{
	write_digital_io(QD1,DIG_DISCONNECT);
	write_digital_io(QD2,DIG_DISCONNECT);
	}
}
  
void test_moto_init(void)
	{
		 pid_init(&pid_2006_Left, 7000, 0, 10, 0, 0);
		 pid_init(&pid_2006_Right, 7000, 0, 10, 0, 0);
		 pid_init(&pid_3508_Left, 7000, 0, 10, 0, 0);
		 pid_init(&pid_3508_Right, 7000, 0, 10, 0, 0);
		 pid_init(&pid_yaw_speed, 7000, 0, 6, 0, 30);
	}	

	