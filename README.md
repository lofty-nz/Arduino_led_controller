# LED Light Show - FastLED Demo

This project showcases various animation patterns on an LED strip, using the FastLED library. The patterns are created using the different modes provided by FastLED. You can manually rotate between modes or use automatic mode. The brightness of the LEDs can also be controlled. The voltage level of the battery is monitored, with a warning message if it drops below a certain threshold.

## Requirements
- Arduino Board
- LED Strip (WS2812B Type, GRB Color Order, 70 LEDs)
- Potentiometer connected to pin A5 \(for brightness control\)
- Green button connected to pin 5 \(for mode switch\)
- Blue button connected to pin 4 \(for setting a fixed pattern\)
- Battery connected to pin A0 \(for battery level monitoring\)

## Setup
1. Connect your Arduino to your computer.
2. Open the Arduino IDE, click on `File -> Open` and navigate to the `.ino` file.
3. Click on `Tools -> Board` and select your Arduino board.
4. Click on `Tools -> Port` and select the correct port for your Arduino board.
5. Click on the upload button to upload the code to your Arduino.

## Operation
- Turn the potentiometer to control the brightness of the LEDs.
- Press the green button to switch between different modes.
- Press the blue button to set a fixed pattern on the LED strip.
- When both buttons are pressed simultaneously, the system goes into automatic mode where it cycles through all patterns automatically.
- In automatic mode, it changes patterns every 10 seconds.
- In manual mode, press the green button to switch to the next pattern.

## Included LED Patterns
1. Rainbow
2. Rainbow with Glitter
3. Confetti
4. Sinelon
5. Juggle
6. BPM \(Beats per minute\)

## Battery Level Monitoring
- The system continuously monitors the battery level connected to pin A0. 
- The low voltage threshold is set to 3.85 volts. 
- If the voltage drops below this level, the Arduino will display a warning message.

## Contributors
- [Your Name]\(Your Github Profile Link\)

## License
This project is licensed under the MIT License - see the [LICENSE]\(LICENSE\) file for details.
