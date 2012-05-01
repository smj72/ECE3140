################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../3140_concur.c \
../process.c \
../uart.c 

ASM_SRCS += \
../3140.asm 

CMD_SRCS += \
../lnk_msp430f2274.cmd 

ASM_DEPS += \
./3140.pp 

OBJS += \
./3140.obj \
./3140_concur.obj \
./process.obj \
./uart.obj 

C_DEPS += \
./3140_concur.pp \
./process.pp \
./uart.pp 

ASM_SRCS_QUOTED += \
"../3140.asm" 

C_SRCS_QUOTED += \
"../3140_concur.c" \
"../process.c" \
"../uart.c" 


# Each subdirectory must supply rules for building sources it contributes
3140.obj: ../3140.asm $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MAX_HOPS=3 --define=MRFI_CC2500 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/boards/EZ430RF" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/common" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/family1" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --preproc_with_compile --preproc_dependency="3140.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

3140_concur.obj: ../3140_concur.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MAX_HOPS=3 --define=MRFI_CC2500 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/boards/EZ430RF" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/common" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/family1" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --preproc_with_compile --preproc_dependency="3140_concur.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

process.obj: ../process.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MAX_HOPS=3 --define=MRFI_CC2500 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/boards/EZ430RF" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/common" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/family1" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --preproc_with_compile --preproc_dependency="process.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MAX_HOPS=3 --define=MRFI_CC2500 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/drivers" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/bsp/boards/EZ430RF" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/common" --include_path="C:/Users/SAN/Dropbox/ECE 3140 Embedded Systems/Workspace/final_radio/radio_example/application/../drivers/mrfi/radios/family1" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --preproc_with_compile --preproc_dependency="uart.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


