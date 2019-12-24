# Raspberry Pi Cluster Temperature Monitor

![SSD1306 screen displaying Raspberry Pi temperatures](/rpi-arduino-ssd1306.jpg?raw=true)

## Parts

- Arduino Uno R3
- Raspberry Pi 4 4GB
- UCTRONICS SSD1306 128x64 OLED Display

## Raspberry Pi Software Requirements

- Python 3.7+
- PySerial
- Ansible
- vcgencmd

## Arduino Development Software Requirements

- ArducamSSD1306 library (included)
- Adafruit GFX library
- Lemon 7pt and 8pt GFXFont headers with degree glyph (included)

## Scripts

### ard-serial.py

1. Connects to serial port the Arduino is connected to (requires manually updating the script with the correct device name)
2. Calls Ansible to call `vcgencmd measure_temp` across the Pi cluster and return the temperatures
3. Parses output into sorted (by host/key) dictionary of `{ host: temp }`
4. Creates message of temps with characters denoting the start and end of the message
5. Sends the message serially over the USB port to the Arduino
6. Sleeps before repeating Steps 2-5. This does mean the script is meant to be long-running.  This was done to avoid frequent resets of the Arduino.

### ssd1306-serial-temps.ino

- Displays loading screen after serial port reset (from Python connecting)
- Parses the serial message between starting and ending characters, then splits the message into it's individual temperatures and prints them to the screen

## Miscellany

### Determining Arduino serial device name

1. Run `ls -l /dev/tty* > before`
2. Plug in Arduino
3. Run `ls -l /dev/tty* > after`
4. Run `diff before after`

### Ansible Setup

- Default config
- Hosts file:

  ```yaml
  all:
    children:
      pi:
        vars:
          ansible_user: pi
          ansible_python_interpreter: auto_silent
        hosts:
          192.168.1.21:
          192.168.1.22:
          192.168.1.23:
          192.168.1.24:
  ```

### Arduino Setup

- Font headers are placed inside the ~/Documents/Arduino/libraries/Adafruit_GFX_Library/Fonts
  - Font headers were generated using the fontconvert code included with the Adafruit GFX library (see below)
- Adafruit GFX library was installed using the Arduino IDE library manager: `Tools > Manage Libraries`
- ArducamSSD1306 library was downloaded from https://github.com/supprot/ArducamSSD1306, zipped up and imported as a ZIP library using the Arduino IDE

### fontconvert compilation

1. Modified Makefile to include additional library folders containing freetype headers
2. Modified Makefile to replace `-lfreetype` with full path to the `libfreetype.dylib` file
