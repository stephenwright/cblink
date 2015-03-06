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
 * blink|start - begin blinking the LED
 * stop - stop blinking

pressing the button will turn on the LED

