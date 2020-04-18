#include <motor.h>
#include <CANDrive.h>

/**
 * @brief 用转矩电流计算得到功率值
 * @param speed 电机速度
 * @param current 转矩电流
 * @param pcof 参数
 * @return 电机功率
 */
float GetChassisMotorPower(int speed, int current, struct PowerCOF_s *pcof)
{
    return (pcof->ss * speed * speed +
            pcof->sc * speed * current +
            pcof->cc * current * current);
}

/**
 * @brief RM6623数据接收
 * @param Dst RM6623电机数据结构体指针
 * @param Data CAN数据帧指针
 */
void RM6623_Receive(RM6623_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->TorqueCurrent = (uint16_t)(Data[2] << 8 | Data[3]);
    Dst->SetTorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
}

/**
 * @brief RM3510数据接收
 * @param Dst RM3510电机数据结构体指针
 * @param Data CAN数据帧指针
 */
void RM3510_Receive(RM3510_TypeDef *Dst, uint8_t *Data)
{
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->Speed = (int16_t)(Data[2] << 8 | Data[3]);
}

/**
 * @brief 设置RM3508功率计算参数
 * @param Dst RM3510电机数据结构体指针
 * @param cc 电流平方项系数
 * @param sc 电流,转速乘积项系数
 * @param ss 转速平方项系数
 */
void RM3508_SetPowerCOF(RM3508_TypeDef *Dst, float cc, float sc, float ss)
{
    Dst->PowerCOF.cc = cc;
    Dst->PowerCOF.sc = sc;
    Dst->PowerCOF.ss = ss;
}

/**
 * @brief RM3508数据接收
 * @param Dst RM3508电机数据结构体指针
 * @param Data CAN数据帧指针
 */
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

/**
 * @brief GM3510数据接收
 * @param Dst GM3510电机数据结构体指针
 * @param Data CAN数据帧指针
 */
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

/**
 * @brief GM6020数据接收
 * @param Dst GM6020电机数据结构体指针
 * @param Data CAN数据帧指针
 */
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

/**
 * @brief M2006数据接收
 * @param Dst M2006电机数据结构体指针
 * @param Data CAN数据帧指针
 */
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

/**
 * @brief 发送电机控制信号
 * @param can CAN枚举
 * @param STD_ID 标准帧ID
 * @param Data 电机控制信号数组指针
 * @return
 */
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
