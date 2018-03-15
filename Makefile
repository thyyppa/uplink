include WifiPassword.incl

TOOLS   = /opt/esp-open-sdk/xtensa-lx106-elf/bin
SDK     = /opt/esp-open-sdk/sdk
ESPTOOL = /usr/bin/esptool
ESPPORT = /dev/ttyUSB0
ESPBAUD = 115200
DEBUG   = -DDEBUG_MSGS
INFO    = -DINFO_MSGS

FIRMWARE = firmware
APP_NAME = uplink

SDK_INCLUDE := $(SDK)/include
SDK_LIB := $(SDK)/lib

LIBS := $(addprefix -l,c gcc hal pp phy net80211 lwip wpa main)

CC	:= $(TOOLS)/xtensa-lx106-elf-gcc
AR	:= $(TOOLS)/xtensa-lx106-elf-ar
LD	:= $(TOOLS)/xtensa-lx106-elf-gcc
LD_SCRIPT := $(SDK)/ld/eagle.app.v6.ld

FW_ADDR_1 = 0x00000
FW_ADDR_2 = 0x40000

CFLAGS	= -Os -std=c11 -g -O2 -Wpointer-arith -Wundef -Werror -Wl,-EL \
		  -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
		  -D__ets__ -DICACHE_FLASH $(DEBUG) $(INFO)
LDFLAGS	= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static

all: mkdirs $(FIRMWARE)/$(APP_NAME) clean

$(APP_NAME).a: main.o tcp.o pins.o wifi.o input_handler.o
	@echo "AR: $@"
	$(AR) cruv $@ $^

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

$(APP_NAME).out: $(APP_NAME).a
	@echo "LD $@"
	$(LD) -L$(SDK_LIB) -T$(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $< -Wl,--end-group -o $@

$(FIRMWARE)/$(APP_NAME): $(APP_NAME).out
	@echo "FW $@"
	$(ESPTOOL) elf2image -o $@ $<

flash: all
	@echo "Flashing to $(ESPPORT)..."
	$(ESPTOOL) -p $(ESPPORT) -b 460800 write_flash -fm dio \
	$(FW_ADDR_1) $(FIRMWARE)/$(APP_NAME)$(FW_ADDR_1).bin \
	$(FW_ADDR_2) $(FIRMWARE)/$(APP_NAME)$(FW_ADDR_2).bin

clean:
	rm -rf *.o *.a *.out

mkdirs:
	rm -rf $(FIRMWARE)
	mkdir -p $(FIRMWARE)

connect:
	screen $(ESPPORT) $(ESPBAUD)