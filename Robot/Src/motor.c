#include <motor.h>
#include <CANDrive.h>

inline float GetChassisMotorPower(int speed, int current, struct PowerCOF_s *pcof) //用转矩电流得到功率值
{
    return (pcof->ss * speed * speed +
            pcof->sc * speed * current +
            pcof->cc * current * current);
}

void RM6623_Receive(RM6623_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->TorqueCurrent = (uint16_t)(Data[2] << 8 | Data[3]);
    Dst->SetTorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
}

void RM3510_Receive(RM3510_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->Speed = (int16_t)(Data[2] << 8 | Data[3]);
}

void RM3508_SetPowerCOF(RM3508_TypeDef *Dst, float cc, float sc, float ss)
{
    Dst->PowerCOF.cc = cc;
    Dst->PowerCOF.sc = sc;
    Dst->PowerCOF.ss = ss;
}

void RM3508_Receive(RM3508_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->Speed = (int16_t)(Data[2] << 8 | Data[3]);
    Dst->TorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
    Dst->temp = Data[6];

    int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;

    if (diff > 4000)
        Dst->r--;
    if (diff < -4000)
        Dst->r++;

    Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
    Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
    Dst->Power = GetChassisMotorPower(Dst->Speed, Dst->TorqueCurrent, &Dst->PowerCOF);
    Dst->LsatAngle = Dst->MchanicalAngle;
}

void GM3510_Receive(GM3510_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->OutputTorque = (uint16_t)(Data[2] << 8 | Data[3]);

    int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;

    if (diff > 4000)
        Dst->r--;
    if (diff < -4000)
        Dst->r++;

    Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
    Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
    Dst->LsatAngle = Dst->MchanicalAngle;
}

void GM6020_Receive(GM6020_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->Speed = (int16_t)(Data[2] << 8 | Data[3]);
    Dst->TorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
    Dst->temp = Data[6];

    int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;

    if (diff > 4000)
        Dst->r--;
    if (diff < -4000)
        Dst->r++;

    Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
    Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
    Dst->LsatAngle = Dst->MchanicalAngle;
}

void M2006_Receive(M2006_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->Speed = (int16_t)(Data[2] << 8 | Data[3]);

    int16_t diff = Dst->MchanicalAngle - Dst->LsatAngle;

    if (diff > 4000)
        Dst->r--;
    if (diff < -4000)
        Dst->r++;

    Dst->Angle = Dst->r * 8192 + Dst->MchanicalAngle;
    Dst->Angle_DEG = Dst->Angle * 0.0439453125f;
    Dst->LsatAngle = Dst->MchanicalAngle;
}

HAL_StatusTypeDef MotorSend(can_num_e can, uint32_t STD_ID, int16_t *Data)
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
    if (can == can1)
        return CAN1_Send_Msg(STD_ID, temp);
#ifdef CAN2_SUPPORT
    else if (can == can2)
        return CAN2_Send_Msg(STD_ID, temp);
#endif
    else
        return HAL_ERROR;
}
