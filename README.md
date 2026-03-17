Folder description
1. STM32L0/: -- Open the code with Keil 5 or above
2. Schematic and Layout as PDF

StateMachine
State:
1. IDLE -- IDLE or Sigfox Protocol/RF Test
2. DEEPSLEEP -- Enter DeepSleep mode
3. BOOT -- Reset all setting and reboot the MCU
4. TRACKING  -- GPS tracking mode, wait until a fix within timeout
5. SETUP -- SETUP
6. WAKEUP
