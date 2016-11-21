#include <DueTimer.h>

void
interrupt ()
{
  static int8_t toggle = 0;
  toggle = 1 - toggle;
  digitalWrite (72, toggle);
  digitalWrite (38, toggle);
  //Serial.println("IT [" + String(toggle) + "] " + String(millis()));
}

void
interrupt2 ()
{
  static int8_t toggle = 0;
  toggle = 1 - toggle;
  digitalWrite (73, toggle);
  digitalWrite (39, toggle);
  //Serial.println("IT [" + String(toggle) + "] " + String(millis()));
}

void
setup ()
{  
  Serial.begin (/*baurate = */250000);
  Serial.println("Init ...");
  pinMode (72, OUTPUT);
  pinMode (73, OUTPUT);
  pinMode (38, OUTPUT);
  pinMode (39, OUTPUT);
  DueTimer::getAvailable ().attachInterrupt (interrupt).setPeriod (50).start ();
  DueTimer::getAvailable ().attachInterrupt (interrupt2).setPeriod (1000).start ();
  Serial.println("...done");
}

void
loop ()
{
}
