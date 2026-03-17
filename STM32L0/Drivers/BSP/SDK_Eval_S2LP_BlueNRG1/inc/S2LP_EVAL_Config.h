#include <stdint.h>
#include "BlueNRG-1_S2LP_pinmap.h"

#include "S2LP_EVAL_EMB_FLASH.h"
#include "S2LP_EVAL_EEPROM.h"
#include "SDK_EVAL_SPI.h"
#include "S2LP_EVAL_Button.h"

void S2LPEvalIdentification(void);
void S2LPEvalSetVersion(uint8_t ver);
uint8_t S2LPEvalGetVersion(void);
