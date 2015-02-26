# cblink

This demo is my first attempt at programming the Pi GPIO.

## Usage

This program expects an LED hooked up to pin 16, and a switch hooked up to pin 17.

```
make && sudo ./blink
```

The program accepts the following commands:

 * on - turn on the LED
 * off - turn off the LED
 * button - turn on the LED, and wait for button press, which turns off the LED
