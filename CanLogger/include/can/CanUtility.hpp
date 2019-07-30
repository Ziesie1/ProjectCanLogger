
#ifndef CANUTILITY_HPP
#define CANUTILITY_HPP

#include <Arduino.h>

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

HAL_StatusTypeDef CanUtility_Init(CAN_SpeedTypedef speed);
HAL_StatusTypeDef CanUtility_DeInit(void);
HAL_StatusTypeDef CanUtility_EnableRecieve(void);
HAL_StatusTypeDef CanUtility_DissableRecieve(void);

extern CAN_HandleTypeDef CanUtility_hcan;
extern bool CanUtility_CanRecieveActive;

#endif //CANUTILITY