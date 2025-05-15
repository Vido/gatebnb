#pragma once
#define LED_BUILTIN 2

bool builtin_state = LOW;

inline void set_led(bool state) {
  builtin_state = state;
  digitalWrite(LED_BUILTIN, builtin_state);
}

inline void blink() {
  builtin_state = ! builtin_state;
  digitalWrite(LED_BUILTIN, builtin_state);
}

inline void deathblink() {
  while(1){
    blink();
    delay(250);
  }
}