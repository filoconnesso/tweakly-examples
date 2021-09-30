//Tweakly Example for Arduino Oplà IoT Kit

#include "Tweakly.h"

#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

 //Birthday melody
int birthday[] = {
  NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,
  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2, 0, 0, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,
  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_END
};

TickTimer clockTimer;
TickTimer greenBlinker;
TickTimer redBlinker;
TickTimer blueBlinker;
TickTimer pwmLed1Timer;
TickTimer pwmLed2Timer;

Pad buzzer(7, MELODY_OUTPUT);

doList changeGreenLedState;
doList changeRedLedState;
doList changeBlueLedState;

doList printTimeText;

Clock myClock;

Player myMelody;

Pong pwmValue1;
Pong pwmValue2;

String timeString;
volatile bool clearTextBool { false };

int greenledstate = 0;
int redledstate = 0;
int blueledstate = 0;

void setup() {

  CARRIER_CASE = false;

  carrier.begin();

  carrier.Buttons.updateConfig(98);
 
  myClock.autoSync();

  pwmValue1.setRange(0,255);
  pwmValue2.setRange(0,255);

  myMelody.buzzerAttach(buzzer.pinNumber());
  myMelody.melodyAttach(birthday);
  myMelody.setNoteDelay(120);
  myMelody.setNoteDuration(40);
  myMelody.repeat(false);
  
  carrier.display.setRotation(4);
  carrier.display.fillScreen(ST77XX_WHITE);

  carrier.display.fillScreen(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(40, 60);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.print("Easy, Tweakly!");

  carrier.display.setCursor(34,105);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.print("Beta 2 is here!");

  carrier.display.setTextSize(3);

  changeGreenLedState.addTask([] { greenledstate = 255;});
  changeGreenLedState.addTask([] { greenledstate = 0;});
  
  changeRedLedState.addTask([] { redledstate = 255;});
  changeRedLedState.addTask([] { redledstate = 0;});
  
  changeBlueLedState.addTask([] { blueledstate = 255;});
  changeBlueLedState.addTask([] { blueledstate = 0;});

  printTimeText.addTask([] { 
     timeString = stringAssembler("%d:%d:%d", myClock.getHours(), myClock.getMinutes(), myClock.getSeconds());
     carrier.display.setTextColor(ST77XX_BLACK);
     carrier.display.print(timeString);
  });
  printTimeText.addTask([] { 
     carrier.display.setTextColor(ST77XX_WHITE);
     carrier.display.print(timeString);
  });

  pwmLed1Timer.attach(10,[] {
    carrier.leds.setPixelColor(3, 0,pwmValue1.value(),0);
    carrier.leds.show();
  });

  pwmLed2Timer.attach(1,[] {
    carrier.leds.setPixelColor(4, 0,pwmValue2.value(),0);
    carrier.leds.show();
  });

  clockTimer.attach(500, [] {
    carrier.display.setCursor(55, 80);
    printTimeText++;
  });

  greenBlinker.attach(250, [] {
    carrier.leds.setPixelColor(0, greenledstate,0,0);
    carrier.leds.show();
    changeGreenLedState++;
  });
  redBlinker.attach(1000, [] {
    carrier.leds.setPixelColor(1, 0,redledstate,0);
    carrier.leds.show();
    changeRedLedState++;
  });
  blueBlinker.attach(1500, [] {
    carrier.leds.setPixelColor(2, 0,0,blueledstate);
    carrier.leds.show();
    changeBlueLedState++;
  });

}

void loop() {
  TweaklyRun();
  
  carrier.Buttons.update();
  
  if(carrier.Buttons.getTouch(TOUCH3)) {
    myMelody.play();
    Serial.println("play pressed");
  }
  if(carrier.Buttons.getTouch(TOUCH1)) {
    myMelody.pause();
    Serial.println("pause pressed");
  }
  if(carrier.Buttons.getTouch(TOUCH2)) {
    myMelody.stop();
    Serial.println("stop pressed");
  }
  
}
