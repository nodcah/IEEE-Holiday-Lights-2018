/*  TopLightsSlowBlink by Nodcah
    Blinks the lights in an random pattern, like twinkling
    holiday lights.
    Some code taken from NeoPixelFunRandomChange
*/

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

//====================================//
//======== IMPORTANT VARIABLES =======//
//====================================//
/*  Example (o represents the LED is turned on):
    ======oooooooooooooooooooooooo=====================
          ^                       ^                    ^
     startPixel               endPixel             PixelCount
*/
const int PixelCount = 200;  // Number of pixels on a strip (default is 150)
const int PixelPin = 2;      // Pin that the LEDS are connected to on Arduino
const int startPixel = 0;    // Pixel number on which to start control (inclusive)
const int endPixel = 200;    // Pixel number on which to end control (exclusive)


//=============================//
//======== Control Code =======//
//=============================//
RgbColor red = RgbColor(100, 0, 0); // (r,g,b)
RgbColor green = RgbColor(0, 100, 0); // (r,g,b)
RgbColor blue = RgbColor(0, 0, 50); // (r,g,b)
RgbColor white = RgbColor(200, 200, 200); // (r,g,b)
RgbColor startColor = green;
RgbColor blinkColor = white;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);


// Animation Code
NeoPixelAnimator animations(PixelCount); // NeoPixel animation management object
uint16_t changePixels[(endPixel - startPixel) / 10]; // pixels to change back after
const uint16_t NOPIXEL = 1000;  // Indicates there isn't a pixel

// simple blend function
void BlendAnimUpdate(const AnimationParam& param) {
  // this gets called for each animation on every time step
  // progress will start at 0.0 and end at 1.0
  // we use the blend function on the RgbColor to mix
  // color based on the progress given to us in the animation
  RgbColor updatedColor;
  updatedColor = RgbColor::LinearBlend(
                   startColor,
                   blinkColor,
                   param.progress);
  // apply the color to the strip
  strip.SetPixelColor(param.index, updatedColor);
}
void BlendAnimUpdateChange(const AnimationParam& param) {
  RgbColor updatedColor;
  updatedColor = RgbColor::LinearBlend(
                   blinkColor,
                   startColor,
                   param.progress);
  // apply the color to the strip
  strip.SetPixelColor(param.index, updatedColor);
}
void PickRandom() {
  // Change back other pixels
  uint16_t pixelToChange = 0;
  while (changePixels[pixelToChange] != NOPIXEL) {
    uint16_t time = random(100, 400);
    animations.StartAnimation(changePixels[pixelToChange], time, BlendAnimUpdateChange);
    pixelToChange++;
  }
  for(int i=0; i<((endPixel - startPixel) / 10); i++){
    changePixels[i] = NOPIXEL;
  }
  // pick random count of pixels to animate
  uint16_t count = random((endPixel - startPixel) / 10);
  int changeCount=0;  // Keeps track of adding pixels to change array
  while (count > 0) {
    // pick a random pixel
    int randomPixel = random(startPixel, endPixel);
    // pick random time 
    uint16_t time = random(100, 400);
    animations.StartAnimation(randomPixel, time, BlendAnimUpdate);
    changePixels[changeCount++] = randomPixel;
    count--;
  }
}


void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  Serial.print("Initializing strip from ");
  Serial.print(startPixel);
  Serial.print(" to ");
  Serial.println(endPixel);
  Serial.flush();  // Wait for transmission to finish

  // Start as blue
  strip.Begin();
  for (int pixel = startPixel; pixel < endPixel; pixel++) {
    strip.SetPixelColor(pixel, startColor);
  }
  strip.Show();
}

void loop() {
  if (animations.IsAnimating()) {
    // the normal loop just needs these two to run the active animations
    animations.UpdateAnimations();
    strip.Show();
  } else {
    // no animations runnning, start some
    PickRandom();
  }
}
