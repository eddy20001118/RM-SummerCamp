#ifndef __EXECUTE_TASK_H__
#define __EXECUTE_TASK_H__

#include "stm32f4xx_hal.h"

void execute_task(const void* argu);

void test_moto_init(void);
void test_moto_control(void);
void qidong(void);
void qidongInit(void);
void callback(void);
unsigned char HexToChar(unsigned char bChar);
#endif

