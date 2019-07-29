
#ifndef CANUTILITY_HPP
#define CANUTILITY_HPP

#include <Arduino.h>

HAL_StatusTypeDef CanUtility_Init(void);
HAL_StatusTypeDef CanUtility_DeInit(void);
HAL_StatusTypeDef CanUtility_EnableRecieve(void);
HAL_StatusTypeDef CanUtility_DissableRecieve(void);

extern CAN_HandleTypeDef CanUtility_hcan;
extern bool CanUtility_CanRecieveActive;

#endif //CANUTILITY