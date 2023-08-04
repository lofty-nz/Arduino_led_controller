#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#define POT_PIN     A5
#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    70
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

#define MIN_BRIGHTNESS 10//minimum brightness level
#define MAX_BRIGHTNESS 180 //maximum brightness level
#define BRIGHTNESS_INCREMENT 20 //increment of brightness change
const int greenbuttonPin = 5;  // the number of the pushbutton pin
const int BlueButtonPin = 4;

const int batteryPin = A0; // Analog pin connected to the battery
const float referenceVoltage = 5; // Arduino's reference voltage (3.3V for most 3.3V boards, 5V for most 5V boards)
const int numReadings = 10; // Number of readings to average
int readings[numReadings]; // Array to store the last 'numReadings' readings
int readIndex = 0; // Index of the current reading
int total = 0; // Sum of the last 'numReadings' readings
int average = 0; // Average of the last 'numReadings' readings
bool initialized = false; // Flag to check if the readings array is initialized
float lastVoltage = 0; // Store the last displayed voltage value
const float LOW_VOLTAGE_THRESHOLD = 3.85; // Set the low voltage threshold value

// Variables will change:
int ledState = HIGH;        // the current state of the output pin
int greenButtonState;            // the current reading from the input pin
int lastgreenButtonState = LOW;  // the previous reading from the input pin
int blueButtonState;            // the current reading from the input pin
int lastBlueButtonState = LOW; 
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int currentBrightness = MIN_BRIGHTNESS; //current brightness level
uint8_t gMinTwinkleValue = 128; // Adjust this value to change the minimum twinkle value
uint8_t gFadeFactor = 999; // Adjust this value to change the fade factor

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 
unsigned long last_green_button_press_time = 0; // Store the last time the green button was pressed
bool automaticMode = true; // Automatic mode by default



void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(9600);
  pinMode(batteryPin, INPUT);
  pinMode(greenbuttonPin, INPUT_PULLUP);
  pinMode(BlueButtonPin, INPUT_PULLUP);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);   // set master brightness control
  Serial.print("setup complete");  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };


void loop()
{
    int vPinReading = analogRead(batteryPin);
  int greenReading = digitalRead(greenbuttonPin);
  int blueReading = digitalRead(BlueButtonPin);

  // Read potentiometer value and map it to the desired brightness range
  int potValue = analogRead(POT_PIN);
  int mappedBrightness = map(potValue, 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  FastLED.setBrightness(mappedBrightness);
  Serial.print(" POT=");
  Serial.print(potValue);
Serial.print(" brightness=");
Serial.print(mappedBrightness);
 Serial.print(" buttons=");
  if (greenReading == LOW && blueReading == LOW) {
    Serial.print("GB ");
    automaticMode = true;
     fill_solid(leds, NUM_LEDS, CRGB::Green);
     FastLED.show();
     delay(1000);  

  } else if (greenReading == LOW) {
    Serial.print("G0 ");
    automaticMode = false;  //put stick in manual mode
    nextPattern();  
    Serial.print(" AUTO=");
  Serial.println(automaticMode);        //advance current pattern

  } else if (blueReading == LOW) {
     Serial.print("0B ");
     fill_solid(leds, NUM_LEDS, CRGB::White);
     FastLED.show();
     delay(8000);  
  } else {
    Serial.println("00 ");
  }




  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  if (automaticMode == true){
    EVERY_N_SECONDS( 10 ) { nextPattern(); }
  } // change patterns periodically
   Serial.print(" AUTO= ");
  Serial.print(automaticMode);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

