#include <motor.h>
#include <test_can.h>

float GetChassisMotorPower(int speed,int current);

inline float GetChassisMotorPower(int speed,int current)   //用转矩电流得到功率值
{
	return (1.571e-7*speed*speed+
					2.248e-6*speed*current+
					2.022e-8*current*current);
}

void RM6623_Receive(RM6623_TypeDef* Dst,uint8_t* Data)
{
	Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
	Dst->TorqueCurrent = (uint16_t)(Data[2] << 8 | Data[3]);
	Dst->SetTorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
}

void RM3510_Receive(RM3510_TypeDef* Dst,uint8_t* Data)
{
	Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
	Dst->Speed = (uint16_t)(Data[2] << 8 | Data[3]);
}

void RM3508_Receive(RM3508_TypeDef* Dst,uint8_t* Data)
{
	Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
	Dst->Speed = (uint16_t)(Data[2] << 8 | Data[3]);
	Dst->TorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
	Dst->temp = Data[6];
	
	int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;
	
	if(diff > 4000)Dst->r--;
	if(diff < -4000)Dst->r++;
	
	Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
	Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
	Dst->Power = GetChassisMotorPower(Dst->Speed,Dst->TorqueCurrent);
	Dst->LsatAngle = Dst->MchanicalAngle;
}

void GM3510_Receive(GM3510_TypeDef* Dst,uint8_t* Data)
{
	Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
	Dst->OutputTorque = (uint16_t)(Data[2] << 8 | Data[3]);
}

void GM6020_Receive(GM6020_TypeDef* Dst,uint8_t* Data)
{
	Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
	Dst->Speed = (uint16_t)(Data[2] << 8 | Data[3]);
	Dst->TorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
	Dst->temp = Data[6];
	
	int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;
	
	if(diff > 4000)Dst->r--;
	if(diff < -4000)Dst->r++;
	
	Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
	Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
	Dst->LsatAngle = Dst->MchanicalAngle;
}

void M2006_Receive(M2006_TypeDef* Dst,uint8_t* Data)
{
	Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
	Dst->Speed = (uint16_t)(Data[2] << 8 | Data[3]);
	
	int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;
	
	if(diff > 4000)Dst->r--;
	if(diff < -4000)Dst->r++;
	
	Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
	Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
	Dst->LsatAngle = Dst->MchanicalAngle;
}

HAL_StatusTypeDef MotorSend(uint8_t can,uint32_t STD_ID,int16_t* Data)
{
	uint8_t temp[8];
	temp[0] = (uint8_t)(Data[0] >> 8);
	temp[1] = (uint8_t)(Data[0] & 0xff);
	temp[2] = (uint8_t)(Data[1] >> 8);
	temp[3] = (uint8_t)(Data[1] & 0xff);
	temp[4] = (uint8_t)(Data[2] >> 8);
	temp[5] = (uint8_t)(Data[2] & 0xff);
	temp[6] = (uint8_t)(Data[3] >> 8);
	temp[7] = (uint8_t)(Data[3] & 0xff);
	if(can == 1)
		return CAN1_Send_Msg(STD_ID,temp);
	else if (can == 2)
		return CAN2_Send_Msg(STD_ID,temp);
	else return HAL_ERROR;
}
