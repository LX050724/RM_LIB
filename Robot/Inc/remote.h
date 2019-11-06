#ifndef __remote_H
#define __remote_H

#ifdef __USE_RTOS
#include "freertos_task.h"
#else
#include <user_main.h>
#endif

#define RC_FRAME_LENGTH 18u
#define REMOTE_CONTROLLER_STICK_OFFSET 1024

#define Key_B   	0x8000
#define Key_V			0x4000
#define Key_C			0x2000
#define Key_X			0x1000
#define Key_Z			0x0800
#define Key_G			0x0400
#define Key_F			0x0200
#define Key_R			0x0100
#define Key_E			0x0080
#define Key_Q			0x0040
#define Key_Ctrl	0x0020
#define Key_Shift	0x0010
#define Key_D			0x0008
#define Key_A			0x0004
#define Key_S			0x0002
#define Key_W			0x0001


typedef __packed struct
{
	int16_t ch0;
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;
	int8_t s1;
	int8_t s2;
} Remote;

typedef __packed struct
{
	int16_t x;
	int16_t y;
	int16_t z;
	uint8_t last_press_l;
	uint8_t last_press_r;
	uint8_t press_l;
	uint8_t press_r;
} Mouse;

typedef struct {
	uint16_t W:1;
	uint16_t S:1;
	uint16_t A:1;
	uint16_t D:1;
	uint16_t Shift:1;
	uint16_t Ctrl:1;
	uint16_t Q:1;
	uint16_t E:1;
	uint16_t R:1;
	uint16_t F:1;
	uint16_t G:1;
	uint16_t Z:1;
	uint16_t X:1;
	uint16_t C:1;
	uint16_t V:1;
	uint16_t B:1;
}Key_t;

typedef union
{
	uint16_t S;
	Key_t D;
}Key_U;

typedef struct
{
	Remote rc;
	Mouse mouse;
	Key_U key;
	Key_U Lastkey;
} RC_Ctl_t; //控制数据结构体

//输入模式:遥控器/键盘鼠标/停止运行
typedef enum
{
	REMOTE_INPUT = 1,
	KEY_MOUSE_INPUT = 3,
	STOP = 2,
} InputMode_e;

void Remote_Rx(unsigned char *RxMsg);
void Remote_Zero(void);

extern uint8_t Twist;

extern RC_Ctl_t RC_CtrlData;

__weak void RemoteControlProcess(Remote *rc);
__weak void MouseKeyControlProcess(Mouse *mouse,Key_t key,Key_t Lastkey);
__weak void STOPControlProcess(void);

#endif
