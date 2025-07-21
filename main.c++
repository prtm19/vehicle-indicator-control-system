#define LEFT_BTN 3
#define RIGHT_BTN 2
#define LEFT_LED 5
#define RIGHT_LED 4

bool left_on = false;
bool right_on = false;
bool hazard_on = false;

bool leftHandled = false;
bool rightHandled = false;
bool hazardHandled = false;

unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 300;
bool ledState = LOW;

unsigned long leftPressedTime = 0;
unsigned long rightPressedTime = 0;

void setup()
{
    pinMode(LEFT_BTN, INPUT_PULLUP);
    pinMode(RIGHT_BTN, INPUT_PULLUP);
    pinMode(LEFT_LED, OUTPUT);
    pinMode(RIGHT_LED, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    unsigned long currentTime = millis();

    // --- LEFT BUTTON ---
    if (digitalRead(LEFT_BTN) == LOW)
    {
        if (leftPressedTime == 0)
        {
            leftPressedTime = currentTime;
        }

        if ((currentTime - leftPressedTime >= 1000) && !leftHandled && !hazard_on && digitalRead(RIGHT_BTN) == HIGH)
        {
            if (!left_on)
            {
                left_on = true;
                right_on = false;
                Serial.println("LEFT_INDICATOR_ON");
            }
            else
            {
                left_on = false;
                Serial.println("LEFT_INDICATOR_OFF");
            }
            leftHandled = true;
        }
    }
    else
    {
        leftPressedTime = 0;
        leftHandled = false;
    }

    // --- RIGHT BUTTON ---
    if (digitalRead(RIGHT_BTN) == LOW)
    {
        if (rightPressedTime == 0)
        {
            rightPressedTime = currentTime;
        }

        if ((currentTime - rightPressedTime >= 1000) && !rightHandled && !hazard_on && digitalRead(LEFT_BTN) == HIGH)
        {
            if (!right_on)
            {
                right_on = true;
                left_on = false;
                Serial.println("RIGHT_INDICATOR_ON");
            }
            else
            {
                right_on = false;
                Serial.println("RIGHT_INDICATOR_OFF");
            }
            rightHandled = true;
        }
    }
    else
    {
        rightPressedTime = 0;
        rightHandled = false;
    }

    // --- HAZARD MODE (Both Buttons) ---
    if (digitalRead(LEFT_BTN) == LOW && digitalRead(RIGHT_BTN) == LOW)
    {
        if (leftPressedTime != 0 && rightPressedTime != 0 &&
            (currentTime - leftPressedTime >= 1000) &&
            (currentTime - rightPressedTime >= 1000) &&
            !hazardHandled)
        {

            hazard_on = true;
            left_on = false;
            right_on = false;
            Serial.println("HAZARD_ON");
            hazardHandled = true;
        }
    }
    else
    {
        hazardHandled = false;
    }

    // --- EXIT HAZARD MODE ---
    if (hazard_on && ((digitalRead(LEFT_BTN) == LOW && !leftHandled && (currentTime - leftPressedTime >= 1000)) ||
                      (digitalRead(RIGHT_BTN) == LOW && !rightHandled && (currentTime - rightPressedTime >= 1000))))
    {
        hazard_on = false;
        Serial.println("HAZARD_OFF");
    }

    // --- LED BLINKING ---
    if (currentTime - lastBlinkTime >= blinkInterval)
    {
        lastBlinkTime = currentTime;
        ledState = !ledState;

        if (hazard_on)
        {
            digitalWrite(LEFT_LED, ledState);
            digitalWrite(RIGHT_LED, ledState);
        }
        else
        {
            digitalWrite(LEFT_LED, left_on ? ledState : LOW);
            digitalWrite(RIGHT_LED, right_on ? ledState : LOW);
        }
    }
}
