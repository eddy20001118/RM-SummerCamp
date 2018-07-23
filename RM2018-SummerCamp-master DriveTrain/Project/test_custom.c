#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "pid.h"
#include "sys.h"
#include  "gimbal_task.h"
#include "cmsis_os.h"
int16_t test_moto_speed[4] = {0,0,0,0};
int16_t test_moto_current[4];
uint16_t MAX_VALUE=8500;
uint8_t CHASSIS_X;
uint8_t CHASSIS_Y;
uint8_t CHASSIS_R;

void test_moto_control(void)
{

   //电机的速度给定
   CHASSIS_X=(rc.ch1/RC_MAX_VALUE)*MAX_VALUE;
	 CHASSIS_Y=(rc.ch2/RC_MAX_VALUE)*MAX_VALUE;
	 CHASSIS_R=(rc.ch3/RC_MAX_VALUE)*MAX_VALUE;
/***
	         1 %++++++% 0
               ++++
               ++++
           2 %++++++% 3
	 **/
	/**
   test_moto_speed[0] = CHASSIS_X-CHASSIS_Y+CHASSIS_R;
   test_moto_speed[1] = CHASSIS_X+CHASSIS_Y+CHASSIS_R;
   test_moto_speed[2] = -CHASSIS_X+CHASSIS_Y+CHASSIS_R;
   test_moto_speed[3] = -CHASSIS_X-CHASSIS_Y+CHASSIS_R;
   
	//闭环计算电机电流
    test_moto_current[0] = pid_calc(&pid_test_moto_RightFont, moto_chassis[0].speed_rpm, test_moto_speed[0]);
	  test_moto_current[1] = pid_calc(&pid_test_moto_LeftFont, moto_chassis[1].speed_rpm, test_moto_speed[1]);
	  test_moto_current[2] = pid_calc(&pid_test_moto_LeftRear,moto_chassis[2].speed_rpm, test_moto_speed[2]);
	  test_moto_current[3] = pid_calc(&pid_test_moto_RightRear,moto_chassis[3].speed_rpm, test_moto_speed[3]);
   
   //发送电机的电流
   send_chassis_moto_current(test_moto_current);
			*/
}
   //电机初始化参数设定
	void test_moto_init(void)
		{
			/**
			 pid_init(&pid_test_moto_RightFont, 7000, 0, 3, 0, 0);
			 pid_init(&pid_test_moto_LeftFont, 7000, 0, 3, 0, 0);
			 pid_init(&pid_test_moto_LeftRear, 7000, 0, 3, 0, 0);
			 pid_init(&pid_test_moto_RightRear, 7000, 0, 3, 0, 0);
*/
			}	
