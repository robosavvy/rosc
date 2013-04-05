################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include external_sources/rosc_sys_linux_x86_src/subdir.mk
-include external_sources/rosc_node_src/rosc/system/subdir.mk
-include external_sources/rosc_node_src/rosc/string_res/subdir.mk
-include external_sources/rosc_node_src/rosc/com_xml/parse/sub/subdir.mk
-include external_sources/rosc_node_src/rosc/com_xml/parse/mode/subdir.mk
-include external_sources/rosc_node_src/rosc/com_xml/parse/handler/subdir.mk
-include external_sources/rosc_node_src/rosc/com_xml/parse/subdir.mk
-include external_sources/rosc_node_src/rosc/com_xml/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: rosc_Linux_test

# Tool invocations
rosc_Linux_test: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "rosc_Linux_test" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) rosc_Linux_test
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
