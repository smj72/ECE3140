################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
bsp/bsp.obj: ../bsp/bsp.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/MSP430 Code Generation Tools 3.2.3/bin/cl430" -g --define=__MSP430F2274__ --define=MAX_HOPS=3 --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="Z:/ece3140/radio_example/application/../drivers" --include_path="Z:/ece3140/radio_example/application/../drivers/bsp" --include_path="Z:/ece3140/radio_example/application/../drivers/bsp/drivers" --include_path="Z:/ece3140/radio_example/application/../drivers/bsp/boards/EZ430RF" --include_path="Z:/ece3140/radio_example/application/../drivers/mrfi" --include_path="Z:/ece3140/radio_example/application/../drivers/mrfi/radios/common" --include_path="Z:/ece3140/radio_example/application/../drivers/mrfi/radios/family1" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/MSP430 Code Generation Tools 3.2.3/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="bsp/bsp.pp" --obj_directory="bsp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


