# RM_LIB —— Robomaster整合库

***！！！README文档不再更新，详细参见 [Doxygen在线文档](https://www.kdrobot.top/document/RM_LIB/html/index.html)！！！***

注意使用的HAL库的版本大于1.19.0,推荐1.24.x

## 配置方法

>+ 添加全局宏定义__USE_RTOS决定是否使用FreeRTOS的标志决定了CAN是否使用临界区保护以及巴特沃斯滤波器动态内存分配使用的函数
>+ 添加全局宏定义WatchDoglength，并赋值看门狗最大数量启用看门狗
>+ 添加全局宏定义CAN2_SUPPORT，启用CAN2支持

## 使用方法

### CHASSIS_MODULE底盘模块

>+ `ChassisSpeed_Ref_t` 速度矢量结构体  
> 内有三个成员
> forward_back_ref 前进方向分量
> left_right_ref 左右方向分量
> rotate_ref 旋转分量
>
>+ `Chassis_Motor_Speed` 电机速度结构体  
> 内有4个成员分别是4个底盘电机>的速度
>
>+ `void ChassisMotorSpeedClean(ChassisSpeed_Ref_t* ref)`  
>将传入的速度矢量清零
>
>+ `__weak void PID_Expect(Chassis_Motor_PID_Expect* motor,ChassisSpeed_Ref_t* ref)`
>由速度矢量计算电机速度,该函数是弱函数,可以重写以适应不同的底盘

### FILTER_MODULE二阶阶卡尔曼滤波器模块

**!该模块依赖DSP库**

>+ `kalman_filterII_t`  
>二阶卡尔曼滤波器结构体
>
>+ `void kalmanII_Init(kalman_filter_t *I)`  
>初始化二阶卡尔曼滤波器
>
>+ `float* KalmanII_Filter(kalman_filter_t *I, float signal1, float signal2)`  
>二阶卡尔曼滤波器,signal是传入的两个信号,返回值为滤波结果,是长度为2的float数组指针

### KALMAN_MODULE一阶卡尔曼滤波器模块

>+ `kalman_filter_t`  
>一阶卡尔曼滤波器结构体
>
>```C
>/**
>  * @brief 初始化一个卡尔曼滤波器
>  * @param[out] p 滤波器
>  * @param[in] T_Q 系统噪声协方差
>  * @param[in] T_R 测量噪声协方差
>  */
>void kalman_Init(kalman_filter_t *p, float T_Q, float T_R);
>
>/**
>  * @brief 卡尔曼滤波器
>  * @param[in] p 滤波器
>  * @param[in] dat 待滤波信号
>  * @retval 滤波后的信号
>  */
>float Kalman_Filter(kalman_filter_t *p, float dat)
>```
>
>
### IIRFilter_MODULE IIR滤波器模块

> 实现过程完全与硬件无关，可直接调用，任意移植。
> 先由Matlab的fdatool Filter Designer工具箱设计一个IIR直接II型，二阶节巴特沃斯滤波器，然后导出C语言头使用时先定义一个IIRFilter_t对象，然后调用IIRFilterInit将其根据传入的节数用Matlab导出的系数初始化每次读取到传感器数据后即可调用IIRFilter来对数据进行滤波。使用完成后使用IIRFilterIIRDelete将内存释放，仅支持二阶节。
>
> + IIRFilter_t
>   巴特沃斯滤波器结构体
>
>  ```C
> /**
>  * @brief N阶IIR滤波器初始化
>  * @param filter 滤波器
>  * @param Sections 滤波器节数
>  * @param Matlab_NUM Matlab导出头文件NUM系数
>  * @param Matlab_DEN Matlab导出头文件DEN系数
>  */
> void IIRFilterInit(IIRFilter_t *filter, uint32_t Sections,
>                 const float (*Matlab_NUM)[3],
>                 const float (*Matlab_DEN)[3]);
>   
> /**
>  * @brief 释放IIR滤波器内存
>  * @param filter IIR滤波器
>  */
> void IIRFilterDelete(IIRFilter_t *filter);
>   
> /**
>  * @brief N阶IIR滤波器
>  * @param filter 滤波器
>  * @param input 信号输入
>  * @return 滤波信号输出
>  */
> float IIRFilter(IIRFilter_t *filter, float input);
>  ```



### MOTOR_MODULE电机模块

*** !该模块依赖CAN模块 ***

>> #### RM3508电机  
>>
>>>`RM3508_TypeDef` 电机数据结构体  
>>>
>>>+ MchanicalAngle 机械角度  
>>>+ Speed 转速
>>>+ TorqueCurrent 转矩电流
>>>+ temp 温度
>>>+ Power 功率
>>>+ Angle 连续化机械角度
>>>+ Angle_DEG 连续化角度制角度
>>>+ PowerCOF 计算功率所用的系数,由MATLAB拟合
>>>
>>
>>>`void RM3508_SetPowerCOF(RM3508_TypeDef *Dst, float cc, float sc, float ss, float constant)` 设置计算功率所用系数  
>>>
>>>+ Dst 3508电机数据结构体指针  
>>>+ cc 电流平方项系数
>>>+ sc 转速电流乘积项系数
>>>+ ss 速度平方项系数
>>>+ constant 常数项
>>
>>>`void RM3508_Receive(RM3508_TypeDef* Dst,uint8_t* Data)` RM3508数据接收
>>>
>>>+ Dst RM3508电机数据结构体指针  
>>>+ Data CAN数据帧数组指针
>
>> #### GM6020电机
>>
>>>`GM6020_TypeDef` 电机数据结构体
>>>
>>>+ MchanicalAngle 机械角度  
>>>+ Speed 转速  
>>>+ TorqueCurrent 转矩电流  
>>>+ temp 温度  
>>>+ Angle 连续化机械角度  
>>>+ Angle_DEG 连续化角度制角度  
>>
>>>`void GM6020_Receive(GM6020_TypeDef* Dst,uint8_t* Data)` GM6020数据接收
>>>
>>>+ Dst GM6020电机数据结构体指针  
>>>+ Data CAN数据帧数组指针  
>
>> #### RM3510电机
>>
>>> `RM3510_TypeDef` 电机数据结构体  
>>>
>>>+ MchanicalAngle 机械角度  
>>>+ Speed 转速
>>
>>>`void RM3510_Receive(RM3510_TypeDef* Dst,uint8_t* Data)` RM3510数据接收
>>>
>>>+ Dst RM3510电机数据结构体指针  
>>>+ Data CAN数据帧数组指针
>
>> #### GM3510电机
>>
>>> `GM3510_TypeDef` 电机数据结构体  
>>>
>>>+ MchanicalAngle 机械角度  
>>>+ OutputTorque 输出扭矩
>>>+ Angle 连续化机械角度
>>>+ Angle_DEG 连续化角度制角度
>>
>>> `void GM3510_Receive(GM3510_TypeDef* Dst,uint8_t* Data)` GM3510数据接收
>>>
>>>+ Dst GM3510电机数据结构体指针  
>>>+ Data CAN数据帧数组指针
>
>> #### M2006电机
>>
>>> `M2006_TypeDef` 电机数据结构体  
>>>
>>>+ MchanicalAngle 机械角度
>>>+ Speed 转速
>>>+ Angle 连续化机械角度  
>>>+ Angle_DEG 连续化角度制角度 
>>
>>> `void M2006_Receive(M2006_TypeDef* Dst,uint8_t* Data)` M2006数据接收
>>>
>>>+ Dst M2006电机数据结构体指针  
>>>+ Data CAN数据帧数组指针
>
>> #### RM6623电机
>>
>>> `RM6623_TypeDef` 电机数据结构体  
>>>
>>>+ MchanicalAngle 机械角度  
>>>+ TorqueCurrent 转矩电流  
>>>+ SetTorqueCurrent 设定转矩电流  
>>>+ Angle 连续化机械角度  
>>>+ Angle_DEG 连续化角度制角度
>>
>>>`void RM6623_Receive(RM6623_TypeDef* Dst,uint8_t* Data)` RM6623数据接收
>>>
>>>+ Dst RM6623电机数据结构体指针  
>>>+ Data CAN数据帧数组指针  
>
>`HAL_StatusTypeDef MotorSend(can_num_e can,uint32_t STD_ID,int16_t* Data)` 发送控制数据
>
>+ can CAN序号,可以是can1或can2
>+ STD_ID 标准帧ID
>+ Data 电机控制数据数组指针
>+ 返回值为是否成功

### PID_MODULE

> #### 位置式PID  
>
>> `PID PID数据结构体`  
>>
>>+ Kp 比例系数  
>>+ Ki 积分系数  
>>+ Kd 微分系数  
>>+ limit 积分限幅
>>+ pid_out PID输出
>
>>`void PID_Control(float current, float expected, PID *data)` PID计算
>>
>>+ current 实际值
>>+ expected期望值
>>+ data PID数据结构体指针  

> #### 带史密斯预估器位置式PID
>
>>`PID_Smis` 带史密斯预估器位置式PID数据结构体  
>>
>>+ Kp 比例系数  
>>+ Ki 积分系数  
>>+ Kd 微分系数  
>>+ limit 积分限幅
>>+ pid_out PID输出
>
>>`void PID_Control_Smis(float current, float expected, PID_Smis *data, float speed)` PID计算
>>
>>+ current 实际值
>>+ expected期望值
>>+ data PID数据结构体指针  
>>+ speed 真实速度

> #### 增量式PID
>
>> `PID_ADD` 增量式PID数据结构体
>>
>>+ Kp 比例系数  
>>+ Ki 积分系数  
>>+ Kd 微分系数  
>>
>>`float PID_Increment(float current, float expect, PID_ADD *parameter)` PID计算
>>
>>+ current 实际值
>>+ expected期望值
>>+ parameter PID数据结构体指针  
>>

### RAMP_MODULE 斜坡函数模块

>`Ramp_Typedef` 斜坡句柄
>
>+ 正确的初始化方式为`Ramp_Typedef <变量名> = {.RampTime = <设置的斜坡时间>};`其余的不要动

>`__weak uint32_t Get_TimerTick(void)` 获得时钟计数函数  
>
>+ 使用前需重写该函数,斜坡时间的单位由该时钟决定
>

>`float Slope(Ramp_Typedef* Ramp)` 获得斜坡系数
>
>+ 得到在设置斜坡时间内匀速增大的0-1的系数

>`void ResetSlope(Ramp_Typedef* Ramp)` 复位斜坡
>
>+ 为了重新进入斜坡,使用完需要释放  
>

### REMOTE_MODULE 遥控器模块

>`RC_FRAME_LENGTH` 遥控器串口帧长度(字节)

>`REMOTE_CONTROLLER_STICK_OFFSET` 遥杆零点偏置

>`RC_Ctl_t` 遥控器数据结构体 已经定义好一个`RC_CtrlData`,无需再次定义
>> `Remote rc` 摇杆数据结构体
>>+ ch0 通道0
>>+ ch1 通道1
>>+ ch2 通道2
>>+ ch3 通道3
>
>> `Mouse mouse` 鼠标数据结构体
>>+ x x轴速度
>>+ x y轴速度
>>+ x z轴速度
>>+ last_press_l 上一次按下左键标志位
>>+ last_press_r 上一次按下右键标志位
>>+ press_l 按下左键标志位
>>+ press_r 按下右键标志位
>
>>`Key_U key` 键盘数据位域
>>
>>+ 各个按键按下标志位
>
>>`Key_U Lastkey` 上一次键盘数据位域

>`void Remote_Rx(unsigned char *RxMsg)` 遥控器数据处理函数
>
>+ RxMsg 串口数据包数组指针

> `void RemoteClear(void)` 遥控器恢复默认值

>`__weak void RemoteControlProcess(Remote *rc)` 摇杆模式处理函数
>
>+ rc 摇杆数据结构体
>+ 将此函数重写达到自己的目的

>`__weak void MouseKeyControlProcess(Mouse *mouse,Key_t key,Key_t Lastkey)` 键鼠模式处理函数
>
>+ key 键盘数据位域
>+ Lastkey 上一次键盘数据位域
>+ 将此函数重写达到自己的目的

>`__weak void STOPControlProcess(void)` 急停处理函数
>
>+ 将此函数重写达到自己的目的

### CANDRIVE_MODULE

>`can_num_e` CAN枚举  
> can1 = 1  
> can2 = 2  

>`void CanFilter_Init(CAN_HandleTypeDef* hcan)` CAN过滤器初始化
>初始化CAN过滤器,无过滤规则
>
>+ hcan CAN句柄  

> #### CAN1变量及函数
> `CAN_RxHeaderTypeDef CAN1_Rx` CAN1接收控制,详见HAL库说明
> `CAN_TxHeaderTypeDef CAN1_Tx` CAN1发送控制,详见HAL库说明
> `uint8_t CAN1_buff[8]` CAN1数据缓冲区  
>
> >`HAL_StatusTypeDef CAN1_Send_Msg(uint32_t StdId, uint8_t *msg)` CAN1发送数据
> >
> >+ StdId 标准帧ID
> >+ msg 数据包数组首地址
> >
> >*若使能CAN2,有相同的函数及变量的CAN2版本*
> >
> >使能CAN2的方法为在全局变量中添加`CAN2_SUPPORT`宏
>

如果使用了实时系统需要添加`__USE_RTOS`全局宏定义来配置临界区保护

### WATCHDOG_MODULE 看门狗模块


>`WatchDog_TypeDef` 看门狗句柄

>`void Feed_Dog(WatchDogp handle)` 喂狗
>
>+ handle 看门狗句柄指针

>`void WatchDog_Init(WatchDogp handle,uint32_t Life);` 看门狗初始化
>
>+ handle 看门狗句柄指针
>+ Life 生存时间,毫秒

>`void WatchDog_CallBack(WatchDogp handle)` 看门狗超时回调函数
>
>+ handle 看门狗句柄指针

>`void FeedDog_CallBack(WatchDogp handle)` 看门狗喂狗回调函数
>
>+ handle 看门狗句柄指针

使用看门狗时需要在全局宏定义中定义`WatchDoglength`并赋值一个整数作为最大看门狗数量，该值不能小于实际看门狗数量
看门狗结构体不能在局部变量中定义，要在全局变量中定义。

### RMQueue 队列容器模块

先使用RMQueueInit将队列初始化使用完需要RMQueueDelete释放队列内存如果未释放内存把队列对象丢失会造成内存泄漏
>`RMQueue_Handle` 队列句柄
>
>```C
>/**
> * @brief 初始化队列
> * @param handle 队列句柄
> * @param typeSize 元素大小
> * @param depth 队列深度
> * @return 成功分配内存返回RM_SUCCESS，否则返回RM_ERROR
> */
>RM_Status RMQueueInit(RMQueue_Handle *handle, uint32_t typeSize, uint32_t depth);
>
>/**
> * @brief 将数据压入队列
> * @param handle 队列句柄
> * @param dataPtr 数据指针
> * @return 成功返回RM_SUCCESS， 队列满返回RM_ERROR
> */
>RM_Status RMQueuePush(RMQueue_Handle *handle, void *dataPtr);
>
>/**
> * @brief 弹出队列首元素
> * @param handle 队列句柄
> * @return 队列首元素指针
> */
>void *RMQueuePop(RMQueue_Handle *handle);
>
>/**
> * @brief 删除队列，释放内存
> * @param handle 队列句柄
> */
>void RMQueueDelete(RMQueue_Handle *handle);
>
>/**
> * @brief 清除队列中的内容
> * @param handle 队列句柄
> */
>static inline void RMQueueClear(RMQueue_Handle *handle) {
>    handle->head = 0;
>    handle->end = 0;
>}
>
>/**
> * @brief 返回队列的大小
> * @param handle 队列句柄
> * @return 队列大小
> */
>static inline uint32_t RMQueueSize(RMQueue_Handle *handle) {
>    return handle->size;
>}
>
>/**
> * @brief 判断队列为空
> * @param handle 队列句柄
> * @return 队列为空返回真
> */
>static inline RM_Status RMQueueIsEmpty(RMQueue_Handle *handle) {
>    return handle->size == 0 ? RM_YES : RM_NO;
>}
>```

### Cortex-M4平台DSP库添加方法

1. 添加全局宏定义`ARM_MATH_CM4`和`__TARGET_FPU_VFP`，前者启用DSP库，后者启用FPU
2. 添加包含路径`Drivers\CMSIS\DSP\Include`包含DSP库头文件
3. 添加预编译库`Drivers\CMSIS\Lib\ARM\arm_cortexM4lf_math.lib`到源文件中

