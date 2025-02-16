# ESP32 Arduino Makefile using arduino-cli

# Board & Port Configuration
BOARD = esp32:esp32:esp32
PORT ?= /dev/cu.usbserial-1110 # CHANGE THIS
BAUD ?= 115200

# Sketch & Build Configuration
SKETCH = hylighter.ino  # Change this to your main .ino file
BUILD_DIR = build

# Arduino CLI
ARDUINO_CLI = arduino-cli
FLAGS = --fqbn $(BOARD) --port $(PORT) --build-path $(BUILD_DIR)

# Targets
all: compile

compile:
	$(ARDUINO_CLI) compile $(FLAGS) --verbose $(SKETCH)

upload: compile
	$(ARDUINO_CLI) upload $(FLAGS)

monitor:
	screen $(PORT) $(BAUD)

clean:
	rm -rf $(BUILD_DIR)

list-ports:
	$(ARDUINO_CLI) board list

