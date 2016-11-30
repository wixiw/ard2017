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
SUBDIRS :=  \
../Device_Startup/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Device_Startup/startup_sam3xa.c \
../Device_Startup/system_sam3xa.c \
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_sam3xa.o \
Device_Startup/system_sam3xa.o \
main.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_sam3xa.o \
Device_Startup/system_sam3xa.o \
main.o

C_DEPS +=  \
Device_Startup/startup_sam3xa.d \
Device_Startup/system_sam3xa.d \
main.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_sam3xa.d \
Device_Startup/system_sam3xa.d \
main.d

OUTPUT_FILE_PATH +=UT_FreeRtos.elf

OUTPUT_FILE_PATH_AS_ARGS +=UT_FreeRtos.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../Device_Startup/sam3x8e_flash.ld \
../Device_Startup/sam3x8e_sram.ld \
../Device_Startup/sam3xa_flash.ld \
../Device_Startup/sam3xa_sram.ld


# AVR32/GNU C Compiler







Device_Startup/%.o: ../Device_Startup/%.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 5.3.1
	$(QUOTE)F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAM3X8E__ -DDEBUG -DF_CPU=84000000L -DARDUINO=10612 -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM  -I"F:\Programs\AtmelStudio\7.0\Packs\atmel\SAM3X_DFP\1.0.50\include" -I"F:\Programs\AtmelStudio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"..\..\Common\0_BSP\ArduinoCore" -I"..\..\Common\0_BSP\FreeRtos" -I"..\..\Common\0_BSP" -I"..\..\Common\1_RSP" -I"..\..\Common\2_AL"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c -std=gnu99 --specs=nosys.specs -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 5.3.1
	$(QUOTE)F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -DDEBUG -DF_CPU=84000000L -DARDUINO=10612 -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM  -I"F:\Programs\AtmelStudio\7.0\Packs\atmel\SAM3X_DFP\1.0.50\include" -I"F:\Programs\AtmelStudio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"..\..\Common\0_BSP\ArduinoCore" -I"..\..\Common\0_BSP\FreeRtos" -I"..\..\Common\0_BSP" -I"..\..\Common\1_RSP" -I"..\..\Common\2_AL"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c --specs=nosys.specs -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="UT_FreeRtos.map" -Wl,--start-group -lm -lARD -lArduino -lsam_sam3x8e_gcc_rel -larm_cortexM3l_math  -Wl,--end-group -L"..\\Device_Startup" -L"../../Common/Debug" -L"../../Common/0_BSP/ArduinoCore"  -Wl,--gc-sections -mcpu=cortex-m3 -Tsam3x8e_flash.ld  
	@echo Finished building target: $@
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "UT_FreeRtos.elf" "UT_FreeRtos.bin"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "UT_FreeRtos.elf" "UT_FreeRtos.hex"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "UT_FreeRtos.elf" "UT_FreeRtos.eep" || exit 0
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "UT_FreeRtos.elf" > "UT_FreeRtos.lss"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "UT_FreeRtos.elf" "UT_FreeRtos.srec"
	"F:\Programs\AtmelStudio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "UT_FreeRtos.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "UT_FreeRtos.elf" "UT_FreeRtos.a" "UT_FreeRtos.hex" "UT_FreeRtos.bin" "UT_FreeRtos.lss" "UT_FreeRtos.eep" "UT_FreeRtos.map" "UT_FreeRtos.srec"
	