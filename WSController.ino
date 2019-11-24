#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

#define LED_PIN 8
#define SHUNT_PIN 9
#define POWER_PIN 7
#define EMPTY 350
#define ON 0
#define OFF 1
#define DEBUG 0
const int maxOfLeds=200;
int battVolts;
int led = 0;
int currentPin = A1;
int current = 0;
long average = 0;
int numOfLeds = 0;
long later = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(maxOfLeds, LED_PIN, NEO_GRB + NEO_KHZ800);
WS2812FX ws2812fx=WS2812FX(maxOfLeds, LED_PIN, NEO_GRB + NEO_KHZ800);

#define IPIN 2
volatile bool pressed = 0;
int counter = 0;

int getBandgap(void) // Returns actual value of Vcc (x 100)
   {
    // For 168/328 boards
    const long InternalReferenceVoltage = 1100L;  // Adjust this value to your boards specific internal BG voltage x1000
       // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
       // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
    ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
    delay(50);  // Let mux settle a little to get a more stable A/D conversion
       // Start a conversion
    ADCSRA |= _BV( ADSC );
       // Wait for it to complete
    while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
       // Scale the value
    int results = (((InternalReferenceVoltage * 1023L) / ADC) + 5L) / 10L; // calculates for straight line value
    return results;
   }

void buttonPressed() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    pressed = 1;
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, ON);
    if (DEBUG)
      Serial.begin(57600);
    pinMode(SHUNT_PIN, OUTPUT);
    digitalWrite(SHUNT_PIN, 0);
    pinMode(IPIN, INPUT_PULLUP);
    pixels.begin();
    pixels.setBrightness(250);
    pixels.show();
    Serial.println("Setup finished!");
    for (int i=0; i < maxOfLeds; i++) {
        pixels.setPixelColor(i,255,255,255);
        if (i>0)
            pixels.setPixelColor(i-1, 0, 0, 0);
        pixels.show();
        delay(10);
        for (int j=0; j<5; j++) {
            current += analogRead(currentPin);
            //delay(2);
        }
        average = (i*average + current)/(i+1);
        if (DEBUG) {
          Serial.print(i);
          Serial.print(":    ");
          Serial.print(current);
          Serial.print("   -   ");
          Serial.print(average);
          Serial.println();
        }
        if (current < (average - average/5)) {
          if (DEBUG){
            Serial.print("Es sind ");
            Serial.print(i);
            Serial.println(" LEDs!");
          }
          numOfLeds = i;
          break;
        }
        current = 0;
    }
    battVolts = getBandgap();
    if (battVolts < EMPTY)
      digitalWrite(POWER_PIN, OFF);
      if (DEBUG) {
        Serial.print("Voltage: ");
        Serial.println(battVolts);
      }
    //ws2812fx = WS2812FX(numOfLeds, LED_PIN, NEO_GRB + NEO_KHZ800);
    ws2812fx.setLength(numOfLeds);
    ws2812fx.init();
    if (DEBUG)
      ws2812fx.setBrightness(10);
    else
      ws2812fx.setBrightness(130);
    ws2812fx.setSpeed(1000); // größer = langsamer
    ws2812fx.setMode(counter);
    ws2812fx.start();
    if (DEBUG) {
      Serial.print("Counter = ");
      Serial.println(counter);
    }
    digitalWrite(SHUNT_PIN, 1);
    attachInterrupt(digitalPinToInterrupt(IPIN), buttonPressed, FALLING);

}

void loop() {
    // put your main code here, to run repeatedly:
    ws2812fx.service();
    if ((millis() / 60000) > later) {
      later++;
      battVolts = getBandgap();
      if (battVolts < EMPTY)
        digitalWrite(POWER_PIN, OFF);
    }
    if (pressed && (digitalRead(IPIN) == 0)) {
      counter++;
      pressed = 0;
      if (DEBUG) {
        Serial.print("Counter = ");
        Serial.println(counter);
      }
      switch (counter) {
        case 1:
          ws2812fx.stop();
          ws2812fx.setColor(255, 80, 0);
          ws2812fx.start();
          break;
        case 2:
          ws2812fx.stop();
          ws2812fx.setColor(255, 150, 0);
          ws2812fx.start();
          break;
        case 3:
          ws2812fx.stop();
          ws2812fx.setColor(0, 255, 0);
          ws2812fx.start();
          break;
        case 4:
          ws2812fx.stop();
          ws2812fx.setColor(0, 255, 127);
          ws2812fx.start();
          break;
        case 5:
          ws2812fx.stop();
          ws2812fx.setColor(0, 0, 255);
          ws2812fx.start();
          break;
        case 6:
          ws2812fx.stop();
          ws2812fx.setColor(255, 0, 255);
          ws2812fx.start();
          break;
        case 7:
          ws2812fx.stop();
          ws2812fx.setMode(7);
          ws2812fx.start();
          break;
        case 8:
          ws2812fx.stop();
          ws2812fx.setMode(8);
          ws2812fx.start();
          break;
        case 9:
          ws2812fx.stop();
          ws2812fx.setMode(10);
          ws2812fx.start();
          break;
        case 10:
          ws2812fx.stop();
          ws2812fx.setMode(11);
          ws2812fx.start();
          break;
        case 11:
          ws2812fx.stop();
          ws2812fx.setMode(12);
          ws2812fx.start();
          break;
        case 12:
          digitalWrite(POWER_PIN, OFF);
          ws2812fx.stop();
          ws2812fx.setMode(0);
          ws2812fx.setColor(255, 0, 0);
          ws2812fx.start();
          counter = 0;
          break;
        }
    }

}
