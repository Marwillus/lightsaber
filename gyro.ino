
void detectMotion() {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();

  }
}



void adjustSpeed() {
  // Take a reading from the Y Pin of the accelerometer and adjust the value so that
  // positive numbers move in one direction, and negative numbers move in the opposite diraction.
  // We use the map function to convert the accelerometer readings, and the constrain function to ensure that it stays within the desired limits
  // The values of 230 and 640 were determined by trial and error and are specific to my accelerometer. You will need to adjust these numbers to suit your module.

  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);


    LEDAccel = constrain(map(my,40, -40 , maxLEDSpeed, -maxLEDSpeed), -maxLEDSpeed, maxLEDSpeed);


    // If the constSpeed variable is "true", then make sure that the speed of the animation is constant by modifying the LEDSpeed and LEDAccel variables.
    if (constSpeed) {
      LEDAccel = 0;
      if (LEDSpeed > 0) {
        LEDSpeed = maxLEDSpeed / 1.1;   // Adjust the LEDSpeed to half the maximum speed in the positive direction
      }
      if (LEDSpeed < 0) {
        LEDSpeed = -maxLEDSpeed / 1.1;  // Adjust the LEDSpeed to half the maximum speed in the negative direction
      }
    }

    // The Speed of the LED animation sequence can increase (accelerate), decrease (decelerate) or stay the same (constant speed)
    LEDSpeed = LEDSpeed + LEDAccel;

    //The following lines of code are used to control the direction of the LED animation sequence, and limit the speed of that animation.
    if (LEDSpeed > 0) {
      LEDPosition++;                                       // Illuminate the LED in the Next position
      if (LEDSpeed > maxLEDSpeed) {
        LEDSpeed = maxLEDSpeed;                            // Ensure that the speed does not go beyond the maximum speed in the positive direction
      }
    }

    if (LEDSpeed < 0) {
      LEDPosition--;                                       // Illuminate the LED in the Prior position
      if (LEDSpeed < -maxLEDSpeed) {
        LEDSpeed = -maxLEDSpeed;                           // Ensure that the speed does not go beyond the maximum speed in the negative direction
      }
    }
  }
}


void swingSound() {
   float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    
  float headingDiff = abs(lastHeading - heading);
  float pitchDiff = abs(lastPitch - pitch);
  if (lastHeading != 0) {
    if (pitchDiff > motionThreshold || headingDiff > motionThreshold) {
      //cyle through swing sounds
      lastSwingSound++;
      if (lastSwingSound > swingSounds) {
        lastSwingSound = 1;
      }
      String swingFile = "swingX.raw";
      swingFile.replace("X", lastSwingSound);
      char charBuf[50];
      swingFile.toCharArray(charBuf, 50);
      triggerSwing(charBuf);  // needs sequence to iterate through
    }
  }
  lastHeading = heading;
  lastPitch = pitch;

}
}

void levelSense() {
  constSpeed = true;
  LEDPosition = constrain(map(filter.getPitch(), -60, 60, 1, NUM_LEDS - 1), 0 , NUM_LEDS - 1);

  //Jitter correction: this will reduce the amount of jitter caused by the accelerometer reading variability
  if (abs(LEDPosition - oldPos) < 2) {
    LEDPosition = oldPos;
  }

  //The following lines of code will ensure the colours remain within the red to green range, with green in the middle and red at the ends.
  hue = map(LEDPosition, 0, NUM_LEDS - 1, 0, 200);
  if (hue > 100) {
    hue = 200 - hue;
  }

  //Illuminate 2 LEDs next to each other
  showLED(LEDPosition, hue, 255, intensity);
  showLED(LEDPosition - 1, hue, 255, intensity);

  //If the position moves, then fade the old LED positions by a factor of 25 (high numbers mean shorter tail)
  fadeLEDs(25);
  oldPos = LEDPosition;
}
void fadeLEDs(int fadeVal) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy( fadeVal );
  }
}

void showLED(int pos, byte LEDhue, byte LEDsat, byte LEDbright) {
  leds[pos] = CHSV(LEDhue, LEDsat, LEDbright);
  FastLED.show();
}
