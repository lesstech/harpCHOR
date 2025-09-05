#include <MIDI.h>
#include <Servo.h>

//Motor 1
const int motor1ForwardPin = 13;
const int motor1BackwardPin = 12;
//Motor 2
const int motor2ForwardPin = 11;
const int motor2BackwardPin = 10;
//Motor 3
const int motor3ForwardPin = 9;
const int motor3BackwardPin = 8;
//Motor 4
const int motor4ForwardPin = 7;
const int motor4BackwardPin = 6;
//Motor 5
const int motor5ForwardPin = 5;
const int motor5BackwardPin = 4;
//Motor 6
const int motor6ForwardPin = 3;
const int motor6BackwardPin = 2;

const int vibro1Pin = 30;
const int vibro2Pin = 31;
const int vibro3Pin = 32;
const int vibro4Pin = 33;
const int vibro5Pin = 34;

// Servo pins
const int servo1Pin = 45;
const int servo2Pin = 44;

unsigned long time;


Servo servo1;
Servo servo2;

const int vibratePin1 = 5;
int speed = 100;

const int num_voices = 11;
int playing[num_voices] {0};

int playing_pins[num_voices] {
  motor5ForwardPin,
  motor3ForwardPin,
  motor4ForwardPin,
  motor6ForwardPin,
  motor2ForwardPin,
  motor1ForwardPin,
  vibro1Pin,
  vibro2Pin,
  vibro3Pin,
  vibro4Pin,
  vibro5Pin,
};

struct motor_speed_range {
  int min_speed;
  int max_speed;
};

motor_speed_range ranges[6] {
  {20, 180},
  {100, 180},
  {100, 180},
  {100, 180},
  {100, 180},
  {100, 180},
};

struct CustomBaudRate : public MIDI_NAMESPACE::DefaultSettings {
  static const long BaudRate = 38400;
};

void handleStop() {
  for (int i = 0; i<num_voices; ++i) {
    // Handle the vibrators
    if (i >= 6) {
      digitalWrite(playing_pins[i], LOW);
    } else {
      analogWrite(playing_pins[i], 0);
    }
  }
  servo1.write(135);
  servo2.write(30);
  delay(15);
}

void handleControlChange(byte channel, byte cc_number, byte value) {
    //servo1.write(135);
    //servo2.write(30);
    if (cc_number == 16) {
      servo1.write(map(value,0,127,135,115));
    } else if (cc_number == 17) {
      servo2.write(map(value,0,127,30,55));
    }

}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    switch (pitch) {
      case 60:
        playing[0] = velocity;
        break;
      case 62:
        playing[1] = velocity;
        break;
      case 64:
        playing[2] = velocity;
        break;
      case 65:
        playing[3] = velocity;
        break;
      case 67:
        playing[4] = velocity;
        break;
      case 69:
        playing[5] = velocity;
        break;
      case 71:
        playing[6] = velocity;
        break;
      case 72:
        playing[7] = velocity;
        break;
      case 74:
        playing[8] = velocity;
        break;
      default:
        break;
    }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
      switch (pitch) {
      case 60:
        playing[0] = -1;
        break;
      case 62:
        playing[1] = -1;
        break;
      case 64:
        playing[2] = -1;
        break;
      case 65:
        playing[3] = -1;
        break;
      case 67:
        playing[4] = -1;
        break;
      case 69:
        playing[5] = -1;
        break;
      case 71:
        playing[6] = -1;
        break;
      case 72:
        playing[7] = -1;
        break;
      case 74:
        playing[8] = -1;
        break;
    }

}

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, CustomBaudRate);
void setup()
{
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleStop(handleStop);
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages

    //Set pins as outputs
    pinMode(motor1ForwardPin, OUTPUT);
    pinMode(motor1BackwardPin, OUTPUT);
    pinMode(motor2ForwardPin, OUTPUT);
    pinMode(motor2BackwardPin, OUTPUT);
    pinMode(motor3ForwardPin, OUTPUT);
    pinMode(motor3BackwardPin, OUTPUT);
    pinMode(motor4ForwardPin, OUTPUT);
    pinMode(motor4BackwardPin, OUTPUT);
    pinMode(motor5ForwardPin, OUTPUT);
    pinMode(motor5BackwardPin, OUTPUT);
    pinMode(motor6ForwardPin, OUTPUT);
    pinMode(motor6BackwardPin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(vibro1Pin,OUTPUT);
    pinMode(vibro2Pin,OUTPUT);
    pinMode(vibro3Pin,OUTPUT);

    for (int i = 0; i<num_voices; ++i)
    {
      playing[i] = -1;
    }

    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    servo1.write(135);
    servo2.write(30);
    delay(15);
} 

void loop()
{
    // Read incoming messages
    MIDI.read();

    time = millis();

    for (int i = 0; i<num_voices; ++i) {
      // Handle the vibrators
      if (i >= 6) {
        if (playing[i] >= 0) {
          digitalWrite(playing_pins[i], HIGH);
        } else {
          digitalWrite(playing_pins[i], LOW);
        }
      } else {
        if (playing[i] >= 0) {
          analogWrite(playing_pins[i], map(playing[i],0,127,ranges[i].min_speed,ranges[i].max_speed));
        } else {
          analogWrite(playing_pins[i], 0);
        }
      }
    }


}