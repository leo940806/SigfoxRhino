Folder description
1. STM32L0/: Open the code with Keil 5 or above
2. TKT40339_Rhino_VB_SCH_20190430: Schematic and Layout as PDF

StateMachine
State:
1. IDLE -- IDLE or Sigfox Protocol/RF Test
2. DEEPSLEEP -- Enter DeepSleep mode, set RTC alarm
3. BOOT -- Reset all setting and reboot the MCU
4. TRACKING  -- GPS tracking mode, wait until a fix within timeout. Send Sigfox message
5. SETUP -- SETUP
6. WAKEUP -- Wake up process, identify the wake up cause, i) RTC, ii) IMU interrupt. Do corresponding task

MCU(STM32L051K8U6) Peripheral:
1. GPIO: Accelerometer int, for no movement flag
2. GPIO: Reedswitch, magnetic sensing and accessing the device
3. RTC: Schedule alarm with calendar
4. SPI: Speak with S2LP(sub GHz transceiver, Sigfox)
5. Internal Flash: Holding setting non-volitally even the device is powered off or reboot
6. I2C: Accelerometer R/W
7. UART: GPS Chip ZOE-M8Q R/W

MCU Stack Lib
1. SFX_Lib_V2.5.0_FDL_Cortex_M0.a -- Sigfox Stack, Sigfox communication protocol, 
2. libubignsscore.a -- Ubiscale Stack, GPS fix with faster algorithm, 
