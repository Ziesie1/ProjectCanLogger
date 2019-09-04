
#ifndef CANUTILITY_HPP
#define CANUTILITY_HPP

#include <Arduino.h>
#include "can/Canmsg.hpp"

constexpr int CanUtility_CAN_BUFFER_REC_SIZE = 50;

typedef enum
{
    //CAN_1000_KBIT   =   2 funktioniert zurzeit nicht /*transmisson speed of CAN-bus of 1000 kbit/s   */
    CAN_500_KBIT    =   4,  /*transmisson speed of CAN-bus of 500 kbit/s    */
    CAN_400_KBIT    =   5,  /*transmisson speed of CAN-bus of 400 kbit/s    */
    CAN_250_KBIT    =   8,  /*transmisson speed of CAN-bus of 250 kbit/s    */
    CAN_200_KBIT    =   10, /*transmisson speed of CAN-bus of 200 kbit/s    */
    CAN_125_KBIT    =   16, /*transmisson speed of CAN-bus of 125 kbit/s    */
    CAN_100_KBIT    =   20  /*transmisson speed of CAN-bus of 100 kbit/s    */
} CAN_SpeedTypedef;

HAL_StatusTypeDef CanUtility_Init(CAN_SpeedTypedef speed, bool const silent);
HAL_StatusTypeDef CanUtility_DeInit(void);

HAL_StatusTypeDef CanUtility_EnableRecieve(void);
HAL_StatusTypeDef CanUtility_DissableRecieve(void);
HAL_StatusTypeDef CanUtility_RecieveMessage(bool const fifo, Canmsg *const msg);
HAL_StatusTypeDef CanUtility_SendMessage(Canmsg *const msg);
HAL_StatusTypeDef CanUtility_setTransmissionMode(bool const silent);
HAL_StatusTypeDef CanUtility_setTransmissionSpeed(CAN_SpeedTypedef speed);

Canmsg* CanUtility_readFirstMessageFromBuffer(void);
int CanUtility_getbufferCanRecMessagesFillLevel(void);
bool CanUtility_isMessagePending(void);

bool CanUtility_getSilentMode(void);
CAN_SpeedTypedef CanUtility_getTransmissionSpeed(void);
bool CanUtility_isRecieveActive(void);
bool CanUtility_hasFiFoOverflowOccured(void);
bool CanUtility_whereNewMessagesDiscarded(void);
int CanUtility_howManyMessagesWhereDiscarded(void);
void CanUtility_resetDiscardcounter(void);

#endif //CANUTILITY