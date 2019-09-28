
#ifndef CANUTILITY_HPP
#define CANUTILITY_HPP

#include <Arduino.h>
#include "can/Canmsg.hpp"

constexpr int CanUtility_CAN_BUFFER_REC_SIZE = 50; 
/*   
    size of the "CanUtility_bufferCanRecMessages"
    also known as recieveBuffer or Empfangsbuffer
*/

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

typedef enum
{
    CAN_TransmissionMode_Normal             =   0,  /*Messages can be send and recieved, furtheremore the controller will send ack bit*/
    CAN_TransmissionMode_Silent             =   1,  /*Messages can be recieved, but neither send nor will the controller send ack bit*/
    CAN_TransmissionMode_Loopback           =   2,  /*Messages can't be send, but can recieved and the controller will send ack bit*/
    CAN_TransmissionMode_Silent_Loopback    =   3   /*Messages can't be send nor recieved and the controller will not send an ack bit*/
} CAN_TransmissionMode;

HAL_StatusTypeDef CanUtility_Init(CAN_SpeedTypedef speed);
HAL_StatusTypeDef CanUtility_DeInit(void);

HAL_StatusTypeDef CanUtility_EnableRecieve(void);
HAL_StatusTypeDef CanUtility_DissableRecieve(void);
HAL_StatusTypeDef CanUtility_RecieveMessage(bool const fifo, Canmsg *const msg);
HAL_StatusTypeDef CanUtility_SendMessage(Canmsg *const msg);
HAL_StatusTypeDef CanUtility_setTransmissionMode(CAN_TransmissionMode const mode);
HAL_StatusTypeDef CanUtility_setTransmissionSpeed(CAN_SpeedTypedef speed);

Canmsg* CanUtility_readFirstMessageFromBuffer(void);
int CanUtility_getbufferCanRecMessagesFillLevel(void);
bool CanUtility_isMessagePending(void);

CAN_TransmissionMode CanUtility_getTransmissionMode(void);
CAN_SpeedTypedef CanUtility_getTransmissionSpeed(void);
bool CanUtility_isRecieveActive(void);
bool CanUtility_hasFiFoOverflowOccured(void);
bool CanUtility_whereNewMessagesDiscarded(void);
int CanUtility_howManyMessagesWhereDiscarded(void);
void CanUtility_resetDiscardcounter(void);

#endif //CANUTILITY