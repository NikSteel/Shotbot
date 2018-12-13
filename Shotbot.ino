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

void setup() {
  //set the I2C address for MCP chip A and B
  mcpA.begin(0);
  mcpB.begin(1);

  //Set the button input pins
  Button_Setup();

  //Set the solenoid output pins
  Valve_Setup();
  Valve_CloseAll();

  //Set the LED output pins
  Light_Setup();
  Light_TurnOffAll();
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

int Valve_GetPin(int position) {
  switch (position) {
    case 1:  return 1;
    case 2:  return 2;
    case 3:  return 3;
    case 4:  return 4;
    case 5:  return 5;
    case 6:  return 6;
    case 7:  return 7;
    case 8:  return 8;
    case 9:  return 9;
    case 10: return 10;
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
    case 1:  return 11;
    case 2:  return 12;
    case 3:  return 13;
    case 4:  return 14;
    case 5:  return 15;
    case 6:  return 11;
    case 7:  return 12;
    case 8:  return 13;
    case 9:  return 14;
    case 10: return 15;
  }
}

Adafruit_MCP23017& Light_GetChip(int position) {
  if (position <=5) {
    return mcpA;
  }
  else {
    return mcpB;
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
  }
}

int Button_GetPin(int position) {
  switch (position) {
    case 1:  return 1;
    case 2:  return 2;
    case 3:  return 3;
    case 4:  return 4;
    case 5:  return 5;
    case 6:  return 6;
    case 7:  return 7;
    case 8:  return 8;
    case 9:  return 9;
    case 10: return 10;
  }
}

Adafruit_MCP23017& Button_GetChip(int position) {
  return mcpB;
}

bool Button_IsPressed(int position) {
  int pin = Button_GetPin(position);
  Adafruit_MCP23017& chip = Button_GetChip(position);
  return chip.digitalRead(pin);
}

int Button_Scan() {
  while (true) {
    for (int position = minPosition; position <= maxPosition; ++position) {
      if (Button_IsPressed(position)) {
        return position;
      }
    }
  }
}
