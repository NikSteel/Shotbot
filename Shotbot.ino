#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcpA;
Adafruit_MCP23017 mcpB;
int dripCount = 6;
const int dripMillisecondsOpen = 150;
const int dripMillisecondsClosed = 300;
const int blinkCount = 3;
const int blinkMillisecondsOn = 500;
const int blinkMillisecondsOff = 500;
const int minPosition = 1;
const int maxPosition = 10;

// pin 11 of 74HC595 (SHCP)
const int bit_clock_pin = 4;
// pin 12 of 74HC595 (STCP)
const int digit_clock_pin = 3;
// pin 14 of 74HC595 (DS)
const int data_pin = 2;

// digit pattern for a 7-segment display
const byte digit_pattern[10] =
{
  B01111111,  // 0
  B00001101,  // 1
  B10110111,  // 2
  B10011111,  // 3
  B11001101,  // 4
  B11011011,  // 5
  B11111011,  // 6
  B00001111,  // 7
  B11111111,  // 8
  B11011111,  // 9
};

//pins for encoder
const int encoder_movement_pin = 6;
const int encoder_direction_pin = 7;
const int encoder_milliseconds_debounce_delay = 300;

void setup() {
  //set the I2C address for MCP chip A and B
  mcpA.begin(1);
  mcpB.begin(0);

  //Set the button input pins
  Button_Setup();

  //Set the solenoid output pins
  Valve_Setup();
  Valve_CloseAll();

  //Set the LED output pins
  Light_Setup();
  Light_TurnOffAll();

  //set up seven segment display
  pinMode(data_pin, OUTPUT);
  pinMode(bit_clock_pin, OUTPUT);
  pinMode(digit_clock_pin, OUTPUT);  
  update_one_digit(dripCount);

  //set up encoder 
  pinMode(encoder_movement_pin, INPUT_PULLUP);
  pinMode(encoder_direction_pin, INPUT_PULLUP);
}

void loop() {
  //scan for a button press
  int position = Button_Scan();

  //blink the LED at the position of the button press
  Light_Blink(position);

  //keep light on while pouring
  Light_TurnOn(position);

  //pour specified number of drips
  Valve_Drip(position, dripCount);

  //turn the light off
  Light_TurnOff(position);

  //sanity check
  Valve_CloseAll();
  Light_TurnOffAll();
}

//set the value on seven-segment display
void update_one_digit(int data)
{
  int i;
  byte pattern;
  
  // get the digit pattern to be updated
  pattern = digit_pattern[data];

  // turn off the output of 74HC595
  digitalWrite(digit_clock_pin, LOW);
  
  // update data pattern to be outputed from 74HC595
  shiftOut(data_pin, bit_clock_pin, MSBFIRST, pattern);
  
  // turn on the output of 74HC595
  digitalWrite(digit_clock_pin, HIGH);
}

int Valve_GetPin(int position) {
  switch (position) {
    case 1:  return 8;
    case 2:  return 9;
    case 3:  return 10;
    case 4:  return 11;
    case 5:  return 12;
    case 6:  return 13;
    case 7:  return 14;
    case 8:  return 15;
    case 9:  return 0;
    case 10: return 1;
  }
}

Adafruit_MCP23017& Valve_GetChip(int position) {
  return mcpA;
}

void Valve_Setup() {
  for (int position = minPosition; position <= maxPosition; ++position) {
    int pin = Valve_GetPin(position);
    Adafruit_MCP23017& chip = Valve_GetChip(position);
    chip.pinMode(pin, OUTPUT);
  }
}

void Valve_Open(int position) {
  int pin = Valve_GetPin(position);
  Adafruit_MCP23017& chip = Valve_GetChip(position);
  chip.digitalWrite(pin, HIGH);
}

void Valve_Close(int position) {
  int pin = Valve_GetPin(position);
  Adafruit_MCP23017& chip = Valve_GetChip(position);
  chip.digitalWrite(pin, LOW);
}

void Valve_CloseAll() {
  for (int position = minPosition; position <= maxPosition; ++position) {
    Valve_Close(position);
  }
}

void Valve_Drip(int position, int dripCount) {
  for (int i = 1; i <= dripCount; ++i) {
    Valve_Open(position);
    delay(dripMillisecondsOpen);
    Valve_Close(position);
    delay(dripMillisecondsClosed); 
  }
}


void Light_Setup() {
  for (int position = minPosition; position <= maxPosition; ++position) {
    int pin = Light_GetPin(position);
    Adafruit_MCP23017& chip = Light_GetChip(position);
    chip.pinMode(pin, OUTPUT);
  }
}

int Light_GetPin(int position) {
  switch (position) {
    case 1:  return 10;
    case 2:  return 11;
    case 3:  return 12;
    case 4:  return 13;
    case 5:  return 14;
    case 6:  return 15;
    case 7:  return 4;
    case 8:  return 5;
    case 9:  return 6;
    case 10: return 7;
  }
}

Adafruit_MCP23017& Light_GetChip(int position) {
  if (position <=6) {
    return mcpB;
  }
  else {
    return mcpA;
  }
}

void Light_TurnOn(int position) {
  int pin = Light_GetPin(position);
  Adafruit_MCP23017& chip = Light_GetChip(position);
  return chip.digitalWrite(pin, HIGH);
}

void Light_TurnOff(int position) {
  int pin = Light_GetPin(position);
  Adafruit_MCP23017& chip = Light_GetChip(position);
  return chip.digitalWrite(pin, LOW);
}

void Light_TurnOffAll() {
  for (int position = minPosition; position <= maxPosition; ++position) {
    Light_TurnOff(position);
  }
}

void Light_Blink(int position) {
  Light_TurnOn(position);
  delay(blinkMillisecondsOn);
  Light_TurnOff(position);
  delay(blinkMillisecondsOff);
}


void Button_Setup() {
  for (int position = 1; position <= maxPosition; ++position) {
    int pin = Button_GetPin(position);
    Adafruit_MCP23017& chip = Button_GetChip(position);
    chip.pinMode(pin, INPUT);
    chip.pullUp(pin, HIGH);
  }
}

int Button_GetPin(int position) {
  switch (position) {
    case 1:  return 4;
    case 2:  return 5;
    case 3:  return 6;
    case 4:  return 7;
    case 5:  return 0;
    case 6:  return 1;
    case 7:  return 2;
    case 8:  return 3;
    case 9:  return 8;
    case 10: return 9;
  }
}

Adafruit_MCP23017& Button_GetChip(int position) {
  return mcpB;
}

bool Button_IsPressed(int position) {
  int pin = Button_GetPin(position);
  Adafruit_MCP23017& chip = Button_GetChip(position);
  return chip.digitalRead(pin) == LOW;
}

int Button_Scan() {
  while (true) {
    for (int position = minPosition; position <= maxPosition; ++position) {
      if (Button_IsPressed(position)) {
        return position;
      }
    }

    //check for encoder input
    if (digitalRead(encoder_movement_pin) == LOW) {
      if (digitalRead(encoder_direction_pin) == HIGH) {
        dripCount = (10 + dripCount + 1) % 10;
      }
      else {
        dripCount = (10 + dripCount - 1) % 10;
      }
      //update the display
      update_one_digit(dripCount);
      delay(encoder_milliseconds_debounce_delay);
    }
  }
}
