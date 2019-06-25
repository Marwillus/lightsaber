#include "OneButton.h"
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <NXPMotionSense.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "test.h"

int y = 4 int x = testFunc(y)

    struct CRGB leds[NUM_LEDS];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//motion
#include <NXPMotionSense.h>
NXPMotionSense imu;
NXPSensorFusion filter;
int motionThreshold = 2;
float lastPitch = 0, lastRoll = 0, lastHeading = 0;

//FastLED
#define NUM_LEDS 86
#define DATA_PIN 7
#define COLOR_ORDER GRB
#define CHIPSET WS2811
#define FRAMES_PER_SECOND 120
// Define the array of leds
CRGB leds[NUM_LEDS];
bool gReverseDirection = false;
CRGBPalette16 currentPalette;

//Sound///////////////////////////
#define MIC_PIN A0 // Microphone is attached to this analog pin
#define LED_PIN 7  // NeoPixel LED strand is connected to this pin
#define N_PIXELS_HALF (NUM_LEDS / 2)
#define SAMPLE_WINDOW 10    // Sample window for average level
#define PEAK_HANG 24        //Time of pause before peak dot falls
#define PEAK_FALL 20        //Rate of falling peak dot
#define PEAK_FALL2 8        //Rate of falling peak dot
#define INPUT_FLOOR 0       //Lower range of analogRead input
#define INPUT_CEILING 150   //Max range of analogRead input, the lower the value the more sensitive (1023 = max)300 (150)
#define DC_OFFSET 0         // DC offset in mic signal - if unusure, leave 0
#define NOISE 150           // Noise/hum/interference in mic signal
#define SAMPLES 60          // Length of buffer for dynamic level adjustment
#define TOP (NUM_LEDS + 2)  // Allow dot to go slightly off scale
#define SPEED .20           // Amount to increment RGB color by each cycle
#define TOP2 (NUM_LEDS + 1) // Allow dot to go slightly off scale
#define LAST_PIXEL_OFFSET NUM_LEDS - 1
#define PEAK_FALL_MILLIS 10 // Rate of peak falling dot
byte
    //  peak      = 0,      // Used for falling dot
    //  dotCount  = 0,      // Frame counter for delaying dot-falling speed
    volCount = 0; // Frame counter for storing past volume data
int
    reading,
    vol[SAMPLES],  // Collection of prior volume samples
    lvl = 10,      // Current "dampened" audio level
    minLvlAvg = 0, // For dynamic adjustment of graph low & high
    maxLvlAvg = 512;
float
    greenOffset = 30,
    blueOffset = 150;
byte peak = 16; // Peak level of column; used for falling dots
//    unsigned int sample;
byte dotCount = 0;     //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot
//CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 currentPalette(OceanColors_p);
CRGBPalette16 targetPalette(CloudColors_p);

//Samples
#define NSAMPLES 64
unsigned int sample;
unsigned int samplearray[NSAMPLES];
unsigned long samplesum = 0;
unsigned int sampleavg = 0;
int samplecount = 0;
//unsigned int sample = 0;
unsigned long oldtime = 0;
unsigned long newtime = 0;

//Ripple variables
int color;
int center = 0;
int step = -1;
int maxSteps = 16;
float fadeRate = 0.80;
int diff;

// cycle variables
int CYCLE_MIN_MILLIS = 2;
int CYCLE_MAX_MILLIS = 1000;
int cycleMillis = 20;
bool paused = false;
long lastTime = 0;
bool boring = true;
bool gReverseDirection = false;
int myhue = 0;

//FOR JUGGLE
uint8_t numdots = 4;  // Number of dots in use.
uint8_t faderate = 2; // How long should the trails be. Very low value = longer trails.
uint8_t hueinc = 16;  // Incremental change in hue between each dot.
uint8_t thishue = 0;  // Starting hue.
uint8_t curhue = 0;
uint8_t thisbright = 255; // How bright should the LED/display be.
uint8_t basebeat = 5;
uint8_t max_bright = 255;

//vu ripple
uint8_t colour;
uint8_t myfade = 255; // Starting brightness.
#define maxsteps 16   // Case statement wouldn't allow a variable.
int peakspersec = 0;
int peakcount = 0;
uint8_t bgcol = 0;
int thisdelay = 20;

//background color
uint32_t currentBg = random(256);
uint32_t nextBg = currentBg;
//CRGBPalette16 currentPalette;
//CRGBPalette16 targetPalette;
TBlendType currentBlending;

// GUItool: begin automatically generated code
AudioPlaySerialflashRaw playFlash; //xy=213,210
AudioPlaySerialflashRaw playHum;   //xy=214,246
AudioPlaySerialflashRaw playSwing; //xy=216,280
AudioMixer4 mixer1;                //xy=502,247
AudioOutputAnalog dac1;            //xy=730,244
AudioConnection patchCord1(playFlash, 0, mixer1, 0);
AudioConnection patchCord2(playHum, 0, mixer1, 1);
AudioConnection patchCord3(playSwing, 0, mixer1, 2);
AudioConnection patchCord5(mixer1, dac1);
//Audio defintion
#define PROP_AMP_ENABLE 5
#define FLASH_CHIP_SELECT 6
int swingSounds = 4;
int lastSwingSound = swingSounds;
int clashSounds = 0;
int mode = 0;
int submode = 0;
int midpoint = NUM_LEDS / 2;

//Gyro////////////////////////////////////////////////////
boolean constSpeed = false; // constSpeed:  toggle between constant and variable speed.
int LEDSpeed = 1;           // LEDSpeed:    stores the "speed" of the LED animation sequence
int maxLEDSpeed = 50;       // maxLEDSpeed: identifies the maximum speed of the LED animation sequence
int LEDAccel = 0;           // LEDAccel:    stores the acceleration value of the LED animation sequence (to speed it up or slow it down)
int LEDPosition = 72;       // LEDPosition: identifies the LED within the strip to modify (leading LED). The number will be between 0-143.  (Zero to NUM_LEDS-1)
int oldPos = 0;             // oldPos:      holds the previous position of the leading LED
byte hue = 0;               // hue:         stores the leading LED's hue value
byte intensity = 150;       // intensity:   the default brightness of the leading LED
byte bright = 80;           // bright:      this variable is used to modify the brightness of the trailing LEDs
int animationDelay = 0;     // animationDelay: is used in the animation Speed calculation. The greater the animationDelay, the slower the LED sequence.

//One Button
OneButton button(A0, true);

int i = 0;
const int potPin = A4;
int potVal;
int endmode;

//setup///////////////////////////////////////////////////////////////////////////////////////
void setup()
{

  //sanity delay
  delay(500);

  //fastled
  Serial.begin(9600);
  Serial.println("resetting");
  Serial.println();
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  potVal = map(analogRead(potPin), 0, 1023, 0, 255);
  LEDS.setBrightness(potPin);

  //onebutton
  button.attachClick(ClickFunction);
  button.attachDoubleClick(DoubleClickFunction);
  button.attachLongPressStop(pressFunction);
  button.setDebounceTicks(20);

  //motion
  imu.begin();
  filter.begin(100);

  Serial.println("Start prop shield RAW player");
  SPI.begin();
  // Enable the amplifier on the prop shield
  pinMode(PROP_AMP_ENABLE, OUTPUT);
  digitalWrite(PROP_AMP_ENABLE, HIGH);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);
  // Set initial volume
  dac1.analogReference(EXTERNAL);
  mixer1.gain(1, 1.0); //hum
  mixer1.gain(0, 1.0); //flash
  if (!SerialFlash.begin(FLASH_CHIP_SELECT))
  {
    while (1)
    {
      Serial.println("Cannot access SPI Flash chip");
      delay(3000);
    }
  }
} //end setup

//loop////////////////////////////////////////////////////////////////////////////////////
void loop()
{

  //poti brightness
  LEDS.setBrightness(potVal);
  //button detection
  button.tick();
  //motion detection
  if (mode == 2)
  {
    detectMotion();
    return
  }
  if (submode == 0)
  {
    allBlack() return
  }

  switch (mode)
  {

  case 0:

    switch (submode)
    {
    case 1:
      fadeBlade();
      break;

    case 2:
      bladeOn();
      break;

    case 3:
      fireBlade();
      break;

    case 4:
      rainbow();
      break;
    }
    break;

  case 1:

    switch (submode)
    {
    case 1:
      VU1();
      break;

    case 2:
      VU2();
      break;

    case 3:
      VU3();
      break;

    case 4:
      VU4();
      break;
    }
    break;

  case 2:
    switch (submode)
    {
    case 1:
      gyroBlade1();
      break;

    case 2:
      gyroBlade2();
      break;
    }
    break;
  }

} //end loop

void ClickFunction()
{
  Serial.println("click");
  if (endmode == 1)
  {
    submode = 1
  }
  submode++;
  Serial.print("submodus=");
  Serial.print(submode);
}

void DoubleClickFunction()
{
  Serial.println("double click");
  mode++;
  submode = 1;
  Serial.print("modus=");
  Serial.print(mode);
} // myDoubleClickFunction

void pressFunction()
{
  Serial.println("pressed");
  fadeBlack;
  submode = 0;
}

void triggerSound(const char *filename)
{
  playFlash.play(filename);
}
void triggerSwing(const char *filename)
{
  if (playSwing.isPlaying() == 0)
  {
    playSwing.play(filename);
  }
  else
  {
    Serial.println("already swinging");
  }
}

void startHum()
{
  Serial.println("startHum");
  playHum.play("hum1.raw");
  playHum.play("hum2.raw");
  Serial.println(playHum.isPlaying());
}
void stopHum()
{
  playHum.stop();
}
