################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/utilities/src/fsl_debug_console.c \
../SDK/platform/utilities/src/fsl_misc_utilities.c \
../SDK/platform/utilities/src/print_scan.c 

OBJS += \
./SDK/platform/utilities/src/fsl_debug_console.o \
./SDK/platform/utilities/src/fsl_misc_utilities.o \
./SDK/platform/utilities/src/print_scan.o 

C_DEPS += \
./SDK/platform/utilities/src/fsl_debug_console.d \
./SDK/platform/utilities/src/fsl_misc_utilities.d \
./SDK/platform/utilities/src/print_scan.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/utilities/src/%.o: ../SDK/platform/utilities/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL26Z128VLH4" -DSD_DISK_ENABLE=1 -DFRDM_KL26Z -D_DEBUG=1 -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/Sources" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/utilities/inc" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/inc" -I"C:\Freescale\KSDK_1.3.0/platform/CMSIS/include" -I"C:\Freescale\KSDK_1.3.0/platform/hal/inc" -I"C:\Freescale\KSDK_1.3.0/platform/osa/inc" -I"C:\Freescale\KSDK_1.3.0/platform/system/inc" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/devices/MKL26Z4/include" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/devices/MKL26Z4/startup" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/Project_Settings/Startup_Code" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/board" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/composite/inc" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/utilities" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/devices" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/osa" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/9555" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/CY3116" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/SDK/platform/CMSIS/Include" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/fatfs" -I"E:\gitrepository\U1P_Embedded\team\xiaowen\SD_SEN_DEMO\KL26_SD_SPI/fatfs/fsl_sd_disk" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


