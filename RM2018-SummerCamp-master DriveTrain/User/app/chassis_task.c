/****************************************************************************
 *  Copyright (C) 2018 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

/**
  *********************** (C) COPYRIGHT 2018 DJI **********************
  * @update
  * @history
  * Version     Date              Author           Modification
  * V1.0.0      January-15-2018   ric.luo
  * @verbatim
  *********************** (C) COPYRIGHT 2018 DJI **********************
  */ 
 



#include "chassis_task.h"
#include "detect_task.h"
#include "gimbal_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "keyboard.h"
#include "pid.h"
#include "sys.h"

#include "stdlib.h"
#include "can.h"
#include "cmsis_os.h"
#include "string.h"

imu_t onBoardIMU1;
chassis_t chassis;
float startDirection;
/* 下面为底盘功能任务的函数 */
void chassis_task(const void* argu)
{
  //初始化底盘控制PID参数
  chassis_pid_param_init();
	startDirection = 0;
  //底盘控制任务循环
  uint32_t chassis_wake_time = osKernelSysTick();
	write_led_io(LED_IO1,LED_ON);
	
  while (1)
  {
    //切换底盘状态
    get_chassis_mode();
    switch (chassis.mode)
    {
      case CHASSIS_FOLLOW_GIMBAL:
      {
        chassis_control_information_get();
      }break;
      case CHASSIS_OPEN_LOOP:
      {
        chassis_control_information_get();
      }break;
      
      case CHASSIS_TWIST:
      {
        chassis_control_information_get();
      }break;
			
      default:
      {
        chassis.vy = 0;
        chassis.vx = 0;
        chassis.vw = 0;
      }break;
    }
    
    if (chassis.mode == CHASSIS_RELAX || glb_err.err_list[REMOTE_CTRL_OFFLINE].err_exist)
    {
      send_chassis_moto_zero_current();
    }
    else
    {
      chassis_custom_control();
    }
    
    osDelayUntil(&chassis_wake_time, CHASSIS_PERIOD);
  }
}


uint32_t twist_count;
void get_chassis_mode(void)
{
  chassis.last_mode = chassis.mode;
  
  switch (rc.sw2)
  {
    case RC_UP:
      chassis.mode = CHASSIS_FOLLOW_GIMBAL;
    break;
    
    case RC_MI:
      chassis.mode = CHASSIS_OPEN_LOOP;
    break;

    case RC_DN:
      chassis.mode = CHASSIS_TWIST;
    break;
  }
  
//  switch (rc.sw1)
//  {
//    case RC_UP: //user custom function
//    break;
//    
//    case RC_MI: //user custom function
//    break;

//    case RC_DN:
//    {
//      if (chassis.mode == CHASSIS_FOLLOW_GIMBAL)
//        chassis.mode = CHASSIS_TWIST;
//    }
//    break;
//  }
//  
//  if (rc.sw2 == RC_DN)
//    chassis.mode = CHASSIS_STOP;
//  
//  if (chassis.mode != CHASSIS_TWIST)
//    twist_count = 0;
  
}
/**
  * @brief     initialize chassis motor pid parameter
  * @usage     before chassis loop use this function
  */
void chassis_pid_param_init(void)
{
  //挂起底盘任务
  //osThreadSuspend(NULL);
  
  //底盘PID参数设置
  for (int k = 0; k < 4; k++)
  {
    pid_init(&pid_wheel_spd[k], 7000, 1000, 3.0f, 0, 0);
  }
  //底盘跟随PID参数设置
  pid_init(&pid_chassis_angle, MAX_CHASSIS_VR_SPEED, 50, 12.0f, 0.0f, 0.0f);
  
}

