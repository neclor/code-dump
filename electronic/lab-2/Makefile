CPU = 16F1789

SRCS = lab2-3.asm
EXEC = lab2-3
BUILD = build
OUT = $(BUILD)/$(EXEC)

MCHP     = C:/Program Files/Microchip
AS-DIR   = $(MCHP)/xc8/v3.10/pic-as/bin
AS       = "$(AS-DIR)/pic-as.exe"

IPE-DIR  = $(MCHP)/MPLABX/v6.35/mplab_platform/mplab_ipe
IPECMD   = "$(IPE-DIR)/ipecmd.exe"
DFP-DIR  = $(MCHP)/MPLABX/v6.35/packs/Microchip/PIC12-16F1xxx_DFP/1.9.258/xc8

PROGRAMMER ?= PK3

AS_FLAGS = -mdfp="$(DFP-DIR)" -mcpu=$(CPU) -xassembler-with-cpp -Wa,-a -Wl,-preset_vec=0h \
           -Wl,-pisr_vec=4h
FLASH_ARGS = -T$(PROGRAMMER) -E -M -P$(CPU) -F"$(OUT).hex" -OL

.PHONY: all clean flash

all: $(OUT).hex

$(OUT).hex: $(SRCS) | $(BUILD)
	$(AS) $(AS_FLAGS) $(SRCS) -o $(OUT)

$(BUILD):
	mkdir -p $(BUILD)

flash: $(OUT).hex
	$(IPECMD) $(FLASH_ARGS)

clean:
	-@rm -rf $(BUILD) $(addprefix $(EXEC).,elf hex hxl cmf sym lst rlf d i o) MPLABXLog.xml log.*
