.PHONY: clean
clean:
	rm -rf .pio data

SRC_FILES := $(shell find src lib include \( -name "*.cpp" -o -name "*.h" \))

.pio/libdeps/clock/ElegantOTA/library.json:
	pio pkg install

deps: .pio/libdeps/clock/ElegantOTA/library.json

.pio/build/clock/firmware.bin: platformio.ini ${SRC_FILES} .pio/libdeps/clock/ElegantOTA/library.json
	WIFI_SSID="Sudden Valley" \
	WIFI_PASSWORD="w1r3l3ss" \
	pio run --environment clock --verbose

build: .pio/build/clock/firmware.bin

check: ${SRC_FILES}
	pio check --verbose --skip-packages

upload: .pio/build/clock/firmware.bin
	WIFI_SSID="Sudden Valley" \
	WIFI_PASSWORD="w1r3l3ss" \
	pio run --environment clock --target upload --verbose
