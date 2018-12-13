#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcpA;
Adafruit_MCP23017 mcpB;
int dripCount = 6;
const int Valve_Count = 10;
const int Light_Count = 10;
const int Button_Count = 10;
const int dripMillisecondsOpen = 100;
const int dripMillisecondsClosed = 100;
const int blinkCount = 3;
const int blinkMillisecondsOn = 500;
const int blinkMillisecondsOff = 500;

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
  int sequenceNumber = Button_Scan();

  //blink the LED at the position of the button press
  Light_Blink(sequenceNumber);

  //keep light on while pouring
  Light_TurnOn(sequenceNumber);

  //pour specified number of drips
  Valve_Drip(sequenceNumber, dripCount);

  //turn the light off
  delay(blinkMillisecondsOn);
  Light_TurnOff(sequenceNumber);

  //sanity check
  Valve_CloseAll();
  Light_TurnOffAll();
}

int Valve_GetPin(int sequenceNumber) {
  switch (sequenceNumber) {
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

Adafruit_MCP23017& Valve_GetChip(int sequenceNumber) {
  return mcpA;
}

void Valve_Setup() {
  for (int i = 1; i <= Valve_Count; ++i) {
    int pin = Valve_GetPin(i);
    Adafruit_MCP23017& chip = Valve_GetChip(i);
    chip.pinMode(pin, OUTPUT);
  }
}

void Valve_Open(int sequenceNumber) {
  int pin = Valve_GetPin(sequenceNumber);
  Adafruit_MCP23017& chip = Valve_GetChip(sequenceNumber);
  chip.digitalWrite(pin, HIGH);
}

void Valve_Close(int sequenceNumber) {
  int pin = Valve_GetPin(sequenceNumber);
  Adafruit_MCP23017& chip = Valve_GetChip(sequenceNumber);
  chip.digitalWrite(pin, LOW);
}

void Valve_CloseAll() {
  for (int i = 1; i <= Valve_Count; ++i) {
    Valve_Close(i);
  }
}

void Valve_Drip(int sequenceNumber, int dripCount) {
  for (int i = 1; i <= dripCount; ++i) {
    Valve_Open(sequenceNumber);
    delay(dripMillisecondsOpen);
    Valve_Close(sequenceNumber);
    delay(dripMillisecondsClosed); 
  }
}


void Light_Setup() {
  for (int i = 1; i <= Light_Count; ++i) {
    int pin = Light_GetPin(i);
    Adafruit_MCP23017& chip = Light_GetChip(i);
    chip.pinMode(pin, OUTPUT);
  }
}

int Light_GetPin(int sequenceNumber) {
  switch (sequenceNumber) {
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

Adafruit_MCP23017& Light_GetChip(int sequenceNumber) {
  if (sequenceNumber <=5) {
    return mcpA;
  }
  else {
    return mcpB;
  }
}

void Light_TurnOn(int sequenceNumber) {
  int pin = Light_GetPin(sequenceNumber);
  Adafruit_MCP23017& chip = Light_GetChip(sequenceNumber);
  return chip.digitalWrite(pin, HIGH);
}

void Light_TurnOff(int sequenceNumber) {
  int pin = Light_GetPin(sequenceNumber);
  Adafruit_MCP23017& chip = Light_GetChip(sequenceNumber);
  return chip.digitalWrite(pin, LOW);
}

void Light_TurnOffAll() {
  for (int i = 1; i <= Light_Count; ++i) {
    Light_TurnOff(i);
  }
}

void Light_Blink(int sequenceNumber) {
  Light_TurnOn(sequenceNumber);
  delay(blinkMillisecondsOn);
  Light_TurnOff(sequenceNumber);
  delay(blinkMillisecondsOff);
}


void Button_Setup() {
  for (int i = 1; i <= Button_Count; ++i) {
    int pin = Button_GetPin(i);
    Adafruit_MCP23017& chip = Button_GetChip(i);
    chip.pinMode(pin, INPUT);
  }
}

int Button_GetPin(int sequenceNumber) {
  switch (sequenceNumber) {
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

Adafruit_MCP23017& Button_GetChip(int sequenceNumber) {
  return mcpB;
}

bool Button_IsPressed(int sequenceNumber) {
  int pin = Button_GetPin(sequenceNumber);
  Adafruit_MCP23017& chip = Button_GetChip(sequenceNumber);
  return chip.digitalRead(pin);
}

int Button_Scan() {
  while (true) {
    for (int i = 1; i <= Button_Count; ++i) {
      if (Button_IsPressed(i)) {
        return i;
      }
    }
  }
}
