/**
 * @file    motor.c
 * @author  yao
 * @date    1-May-2020
 * @brief   电机驱动模块
 * @note    此模块依赖CAN底层驱动模块
 */

#include <motor.h>
#ifdef HAL_CAN_MODULE_ENABLED

/**
 * @brief 用转矩电流计算得到功率值
 * @param[in] speed 电机速度
 * @param[in] current 转矩电流
 * @param[in] pcof 参数
 * @return 电机功率
 */
static inline float GetChassisMotorPower(int speed, int current, struct PowerCOF_s *pcof) {
    return (pcof->ss * speed * speed +
            pcof->sc * speed * current +
            pcof->cc * current * current +
            pcof->constant);
}

void RM6623_Receive(RM6623_TypeDef *Dst, uint8_t *Data) {
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->TorqueCurrent = (uint16_t)(Data[2] << 8 | Data[3]);
    Dst->SetTorqueCurrent = (uint16_t)(Data[4] << 8 | Data[5]);
}

void RM3510_Receive(RM3510_TypeDef *Dst, uint8_t *Data) {
    Dst->MchanicalAngle = (uint16_t)(Data[0] << 8 | Data[1]);
    Dst->Speed = (int16_t)(Data[2] << 8 | Data[3]);
}

void RM3508_Receive(RM3508_TypeDef *Dst, uint8_t *Data) {
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

void GM3510_Receive(GM3510_TypeDef *Dst, uint8_t *Data) {
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

void GM6020_Receive(GM6020_TypeDef *Dst, uint8_t *Data) {
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

void M2006_Receive(M2006_TypeDef *Dst, uint8_t *Data) {
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

HAL_StatusTypeDef MotorSend(CAN_HandleTypeDef *hcan, uint32_t StdId, int16_t *Data) {
    uint8_t temp[8];
    temp[0] = (uint8_t)(Data[0] >> 8);
    temp[1] = (uint8_t)(Data[0] & 0xff);
    temp[2] = (uint8_t)(Data[1] >> 8);
    temp[3] = (uint8_t)(Data[1] & 0xff);
    temp[4] = (uint8_t)(Data[2] >> 8);
    temp[5] = (uint8_t)(Data[2] & 0xff);
    temp[6] = (uint8_t)(Data[3] >> 8);
    temp[7] = (uint8_t)(Data[3] & 0xff);
    return CAN_Send_StdDataFrame(hcan, StdId, temp);
}

int16_t QuickCentering(uint16_t Mch, uint16_t Exp) {
    uint16_t Exp_ = (Exp + 4095) % 8192;
    if (Exp_ < Exp)
        return Mch < Exp_ ? Exp - 8192 : Exp;
    else
        return Mch > Exp_ ? Exp + 8192 : Exp;
}

#endif
