#include "env.h"

#include <blynk.h>
#include "oled-wing-adafruit.h"
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"

VCNL4040 proximitySensor;
OledWingAdafruit display;

#define SENSED_FAR_PIN D5
#define SENSED_MEDIUM_PIN D6
#define SENSED_CLOSE_PIN D7

#define VIRTUAL_LIGHT V1
#define VIRTUAL_PROX V0

SYSTEM_THREAD(ENABLED);
// SYSTEM_MODE(MANUAL);

uint16_t test = 0;

bool buttonPressedAB = false; // false shows the proximity results and LEDs to turn on and true shows the light level to be displayed
bool blynkOrDisplay = true;   // false shows on the display and true shows on the blynk dashboard

void setup()
{
    Blynk.begin(BLYNK_AUTH_TOKEN);
    Serial.begin(9600);

    display.setup();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);

    while (!Serial.isConnected())
    {
    }

    Wire.begin();

    proximitySensor.begin();
    proximitySensor.powerOnProximity();
    proximitySensor.powerOnAmbient();
    proximitySensor.enableWhiteChannel();

    // if (proximitySensor.begin() == false)
    // {
    //     Serial.println("Device not found. Please check wiring.");
    //     while (1); // Freeze!
    // }

    pinMode(SENSED_FAR_PIN, OUTPUT);
    pinMode(SENSED_MEDIUM_PIN, OUTPUT);
    pinMode(SENSED_CLOSE_PIN, OUTPUT);
}
void loop()
{
    Serial.println("Hello World");
    Serial.println(test);
    test++;
    display.loop();
    Blynk.run();
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    // digitalWrite(SENSED_FAR_PIN, HIGH);
    // digitalWrite(SENSED_MEDIUM_PIN, HIGH);
    // digitalWrite(SENSED_CLOSE_PIN, HIGH);

    checkButtons();

    if (buttonPressedAB && blynkOrDisplay)
    {
        // light is being shown on Blynk
        Blynk.virtualWrite(VIRTUAL_LIGHT, proximitySensor.getAmbient());
    }
    if (!buttonPressedAB && blynkOrDisplay)
    {
        // proximity is being shown on Blynk
        Blynk.virtualWrite(VIRTUAL_PROX, proximitySensor.getProximity());
        // flashLED();
    }
    if (buttonPressedAB && !blynkOrDisplay)
    {
        // light is being shown on the OLED
        display.println(proximitySensor.getAmbient());
        display.display();
    }
    if (!buttonPressedAB && !blynkOrDisplay)
    {
        // proximity is being shown on the OLED
        display.println(proximitySensor.getProximity());
        display.display();
        // flashLED();
    }
    Serial.println(proximitySensor.getProximity());
    Serial.println(proximitySensor.getAmbient());
}

void checkButtons()
{
    if (display.pressedA())
    {
        buttonPressedAB = false;
    }
    if (display.pressedB())
    {
        buttonPressedAB = true;
    }
    if (display.pressedC())
    {
        blynkOrDisplay = !blynkOrDisplay;
    }
}

void flashLED()
{
}