/*
new topic completely: i have usb midi music keyboard, it frustrates me that there are only 61 keys and no easy way
to tranpose, so what if we got a nano and some sturdy spst switches to mount on either end of the keyboard
that would send key transpose to the midi usb from the nano?
what could we build for this?

Arduino Nano: The controller.

2 x SPST Push-Button Switches: One for octave up, one for octave down.

2 x 10kΩ Resistors: For pull-down resistors.

USB Mini-B Cable: To connect the Nano to your music keyboard.

Revised Arduino Sketch (Code)
This code uses a variable to track the current octave shift and sends a new command each time a button is pressed. It also includes a check to prevent transposing beyond a reasonable range (e.g., +/- 2 octaves).

C++
*/
#include <MIDIUSB.h>

// Pins for the two switches
const int PIN_UP = 2;
const int PIN_DOWN = 3;

int currentOctaveShift = 0; // Starts at 0, goes from -2 to +2
bool buttonUpState = false;
bool buttonDownState = false;

// IMPORTANT: Replace these with the actual SysEx commands from your manual.
// These are examples. A common format has a single byte that changes to set the transpose value.
// You need to find which byte to change for your keyboard.
// Here we assume the 9th byte (index 8) controls the transpose value.
uint8_t sysexCommand[] = {0xF0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x00, 0x00, 0xF7};

void setup() {
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
}

void loop() {
  // Octave UP button logic
  if (digitalRead(PIN_UP) == LOW) {
    if (!buttonUpState) {
      if (currentOctaveShift < 2) {
        currentOctaveShift++;
        // The value to send will be the new octave shift (e.g., 1 for +1 octave)
        sysexCommand[8] = currentOctaveShift;
        MidiUSB.sendSysEx(sizeof(sysexCommand), sysexCommand);
        MidiUSB.flush();
      }
      buttonUpState = true;
    }
  } else {
    buttonUpState = false;
  }

  // Octave DOWN button logic
  if (digitalRead(PIN_DOWN) == LOW) {
    if (!buttonDownState) {
      if (currentOctaveShift > -2) {
        currentOctaveShift--;
        // The value to send for negative transpose might be 0xFF, or a different range.
        // You must check your keyboard's manual for this!
        sysexCommand[8] = currentOctaveShift;
        MidiUSB.sendSysEx(sizeof(sysexCommand), sysexCommand);
        MidiUSB.flush();
      }
      buttonDownState = true;
    }
  } else {
    buttonDownState = false;
  }

  // A small debounce delay
  delay(10);
}
