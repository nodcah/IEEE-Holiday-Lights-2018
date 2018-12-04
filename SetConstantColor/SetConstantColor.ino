/*  SetConstantColor by Nodcah
    Sets all or part of an LED strip to a single, constant color.
    Uses library NeoPixelBus by Makuna for LED control.
*/

#include <NeoPixelBus.h>

//====================================//
//======== IMPORTANT VARIABLES =======//
//====================================//
/*  Example (o represents the LED is turned on):
    ======oooooooooooooooooooooooo=====================
          ^                       ^                    ^
     startPixel               endPixel             PixelCount
*/
const int PixelCount = 150;  // Number of pixels on a strip (default is 150)
const int PixelPin = 2;      // Pin that the LEDS are connected to on Arduino
const int startPixel = 0;    // Pixel number on which to start control (inclusive)
const int endPixel = 150;    // Pixel number on which to end control (exclusive)

RgbColor color = RgbColor(100, 0, 0); // (r,g,b), Color to set strip to
//HslColor color = HslColor(0.0f,1.0f,.196f);  // (hue,saturation,value), Color to set strip to


//=============================//
//======== Control Code =======//
//=============================//
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
void setup() {
  Serial.begin(115200);

  Serial.print("Initializing strip from ");
  Serial.print(startPixel);
  Serial.print(" to ");
  Serial.println(endPixel);
  Serial.flush();  // Wait for transmission to finish

  strip.Begin();

  // Setting to  a color
  for (int pixel = startPixel; pixel < endPixel; pixel++) {
     strip.SetPixelColor(pixel, color);
  }
  strip.Show();
}

void loop() {
  // Do nothing
  delay(1000);
}
