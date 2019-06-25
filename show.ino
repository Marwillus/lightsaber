
//lightsaber//////////////////////////////

void allBlack()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

void fadeBlack()
{
  for (int i = NUM_LEDS; i > 0; i--)
  {
    leds[i] = CRGB::Black;
    FastLED.delay(10);
    FastLED.show();
    if (mode == 0)
    {
      triggerSound("saberoff.raw");
    }
  }
}

void fadeBlade()
{

  Serial.println("Turn on blade");

  for (int i = 0; i > NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
    FastLED.delay(10);
    FastLED.show();
    triggerSound("saberon.raw");
  }
}

void bladeOn()
{

  for (int i = NUM_LEDS; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Green;
    FastLED.show();
  }

  if (!playHum.isPlaying())
  {
    startHum();
  }
}

void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

//fire///////////////////////////////////////////////
void fire()
{
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  Fire2012(); // run simulation frame

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING 55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void Fire2012()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++)
  {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--)
  {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING)
  {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++)
  {
    CRGB color = HeatColor(heat[j]);
    int pixelnumber;
    if (gReverseDirection)
    {
      pixelnumber = (NUM_LEDS - 1) - j;
    }
    else
    {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

void rainbow()
{
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void pulsing()
{
  int pixels = 4;
  int mini = 5;
  int maxi = 12;
  int tempo = 60;

  // Fade IN
  for (int i = 0; i < pixels; i++)
  {
    for (int k = mini; k < maxi; k++)
    {
      leds[i].r = k;
      FastLED.show();
      FastLED.delay(tempo);
    }
  }
  // Fade OUT
  for (int i = pixels - 1; i > 0; i--)
  {
    for (int k = maxi; k > mini; k--)
    {
      leds[i].r = k;
      FastLED.show();
      FastLED.delay(tempo);
    }
  }
}
