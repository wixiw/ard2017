################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.cpp \
../Robot2017.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
main.o \
Robot2017.o

OBJS_AS_ARGS +=  \
main.o \
Robot2017.o

C_DEPS +=  \
main.d \
Robot2017.d

C_DEPS_AS_ARGS +=  \
main.d \
Robot2017.d

OUTPUT_FILE_PATH +=RobotA.elf

OUTPUT_FILE_PATH_AS_ARGS +=RobotA.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 5.3.1
	$(QUOTE)F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -DF_CPU=84000000L -DARDUINO=10612 -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM -DUSBCON  -I"../../Common/0_BSP/CMSIS/Device/ATMEL/sam3xa/include" -I"../../Common/0_BSP/CMSIS/Device/ATMEL" -I"../../Common/0_BSP/CMSIS/CMSIS/Include" -I"../../Common/0_BSP/ArduinoCore/include" -I"..\..\Common\0_BSP\ArduinoCore" -I"..\..\Common\0_BSP\FreeRtos" -I"../../Common/0_BSP" -I"..\..\Common\1_RSP" -I"..\..\Common\2_AL" -I"..\..\Common"  -O1 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c -nostdlib -fno-threadsafe-statics -std=gnu++11 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 5.3.1
	$(QUOTE)F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -static -Wl,-Map="RobotA.map" -Wl,--start-group -lm -lARD  -Wl,--end-group -L"../../Common/Debug" -L"../../Common/0_BSP/ArduinoCore/linker_scripts"  -Wl,--gc-sections -mcpu=cortex-m3 -Tflash.ld -Wl,--cref -Wl,--check-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -u _sbrk -u link -u _close -u _fstat -u _isatty -u _lseek -u _read -u _write -u _exit -u kill -u _getpid   
	@echo Finished building target: $@
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "RobotA.elf" "RobotA.bin"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "RobotA.elf" "RobotA.hex"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "RobotA.elf" "RobotA.eep" || exit 0
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "RobotA.elf" > "RobotA.lss"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "RobotA.elf" "RobotA.srec"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "RobotA.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "RobotA.elf" "RobotA.a" "RobotA.hex" "RobotA.bin" "RobotA.lss" "RobotA.eep" "RobotA.map" "RobotA.srec"
	