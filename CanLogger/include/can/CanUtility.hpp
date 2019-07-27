/* 
Um die CAN-funktion nutzen zu können muss in der Detei "stm32f3xx_hal_conf.h" 
(C:\Users\<User>\.platformio\packages\framework-arduinoststm32\variants\NUCLEO_F303RE)
die Zeile 55 entkommentiert werden ("//" löschen). 
Die Zeile muss dann lauten: "#define HAL_CAN_MODULE_ENABLED"
*/

#ifndef CANUTILITY_HPP
#define CANUTILITY_HPP

#include "can/Canmsg.hpp"

HAL_StatusTypeDef CanUtility_Init(void);
HAL_StatusTypeDef CanUtility_DeInit(void);
HAL_StatusTypeDef CanUtility_EnableRecieve(void);
HAL_StatusTypeDef CanUtility_DissableRecieve(void);

extern CAN_HandleTypeDef CanUtility_hcan;
extern bool CanUtility_CanRecieveActive;

#endif //CANUTILITY