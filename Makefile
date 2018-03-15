# Don't forget to add your wifi config! Be sure to remove the "example-" from the filename.
include WifiPassword.incl

APP_NAME = uplink

# SDK Location
TOOLS   = /opt/esp-open-sdk/xtensa-lx106-elf/bin
SDK     = /opt/esp-open-sdk/sdk
SDK_INCLUDE := $(SDK)/include
SDK_LIB := $(SDK)/lib

# ESP Info
ESPTOOL = /usr/bin/esptool
ESPPORT = /dev/ttyUSB0
ESPBAUD = 115200

# Log levels
DEBUG   = -DDEBUG_MSGS
INFO    = -DINFO_MSGS

# Output location
FIRMWARE = firmware

# Additional libraries
LIBS := $(addprefix -l,c gcc hal pp phy net80211 lwip wpa main)

# Compiler locations
CC	:= $(TOOLS)/xtensa-lx106-elf-gcc
AR	:= $(TOOLS)/xtensa-lx106-elf-ar
LD	:= $(TOOLS)/xtensa-lx106-elf-gcc
LD_SCRIPT := $(SDK)/ld/eagle.app.v6.ld

# Firmware addresses (probably don't ever touch these)
FW_ADDR_1 = 0x00000
FW_ADDR_2 = 0x40000

# Set up compiler and linker flags
CFLAGS	= -Os -std=c11 -g -O2 -Wpointer-arith -Wundef -Werror -Wl,-EL \
		  -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
		  -D__ets__ -DICACHE_FLASH $(DEBUG) $(INFO)
LDFLAGS	= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static

# Make the output directory, compile the program, then clean up
all: mkdirs $(FIRMWARE)/$(APP_NAME) clean

# Create an archive. If you add a module be sure to include it at the end of this list!
$(APP_NAME).a: main.o tcp.o pins.o wifi.o input_handler.o
	@echo "AR: $@"
	$(AR) cruv $@ $^

# Compile all the things...
main.o: user/user_main.c user/user_main.h include/ide_defs.h user/user_config.h
	@echo "CC: $@"
	$(CC) -I$(SDK_INCLUDE) -Iinclude $(CFLAGS) -DWIFI_SSID='$(WIFI_SSID)' -DWIFI_PASSWORD='$(WIFI_PASSWORD)' -DESPBAUD=$(ESPBAUD) -c $< -o $@

input_handler.o: user/input_handler.c user/input_handler.h include/ide_defs.h user/user_config.h
	@echo "CC: $@"
	$(CC) -I$(SDK_INCLUDE) -Iinclude $(CFLAGS) -c $< -o $@

tcp.o: include/tcp.c include/tcp.h include/ide_defs.h user/user_config.h user/input_handler.h
	@echo "CC: $@"
	$(CC) -I$(SDK_INCLUDE) -Iinclude $(CFLAGS) -c $< -o $@

pins.o: include/pins.c include/pins.h include/ide_defs.h user/user_config.h
	@echo "CC: $@"
	$(CC) -I$(SDK_INCLUDE) -Iinclude $(CFLAGS) -c $< -o $@

wifi.o: include/wifi.c include/wifi.h include/ide_defs.h user/user_config.h
	@echo "CC: $@"
	$(CC) -I$(SDK_INCLUDE) -Iinclude $(CFLAGS) -DWIFI_SSID='$(WIFI_SSID)' -DWIFI_PASSWORD='$(WIFI_PASSWORD)' -c $< -o $@

# Link it all up using the SDK's linker script
$(APP_NAME).out: $(APP_NAME).a
	@echo "LD $@"
	$(LD) -L$(SDK_LIB) -T$(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $< -Wl,--end-group -o $@

# Use esptool to create the final firmware (two files will be created, one for each address)
$(FIRMWARE)/$(APP_NAME): $(APP_NAME).out
	@echo "FW $@"
	$(ESPTOOL) elf2image -o $@ $<

# Compile the app and then flash it onto the esp8266
flash: all
	@echo "Flashing to $(ESPPORT)..."
	$(ESPTOOL) -p $(ESPPORT) -b 460800 write_flash -fm dio \
	$(FW_ADDR_1) $(FIRMWARE)/$(APP_NAME)$(FW_ADDR_1).bin \
	$(FW_ADDR_2) $(FIRMWARE)/$(APP_NAME)$(FW_ADDR_2).bin

# Use screen to connect to the esp
connect:
	screen $(ESPPORT) $(ESPBAUD)

# Some housekeeping
clean:
	rm -rf *.o *.a *.out

mkdirs:
	rm -rf $(FIRMWARE)
	mkdir -p $(FIRMWARE)