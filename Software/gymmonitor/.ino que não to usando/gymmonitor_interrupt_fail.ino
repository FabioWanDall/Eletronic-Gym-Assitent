#include <RotaryEncoder.h>
#include <FlexiTimer2.h>

#define BUF_SIZE 200
volatile int dat_buffer[BUF_SIZE][2];
int buf_first_pos = 0;
int buf_last_pos = 0;

const float UPDATE_TIME = 1.0/1000; // 1 ms
unsigned long long current_time = 0;

RotaryEncoder encoder(2, 3);

int enc_position = 0;
int enc_new_pos = 0;

void setup() {
  Serial.begin(115200);
  
  attachInterrupt(digitalPinToInterrupt(2), encoder_tick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoder_tick, CHANGE);
  
  FlexiTimer2::set(1, UPDATE_TIME, upd_count);
  FlexiTimer2::start();
}

void encoder_tick() {
  encoder.tick();
}

void upd_count() {
  update_position();
  current_time++;
}

String data;

void loop() {
  if (buf_first_pos != buf_last_pos) {
    Serial.print(dat_buffer[buf_first_pos][0]);
    Serial.print(",");
    Serial.print(dat_buffer[buf_first_pos][1]);
    Serial.println(",");
    buf_first_pos++;
    if (buf_first_pos == BUF_SIZE) {
      buf_first_pos = 0;
    }
  }
  delay(10);
}

void update_position() {
  int newPos = encoder.getPosition();
  if (enc_position != newPos) {
    enc_position = newPos;
    
    dat_buffer[buf_last_pos][0] = enc_position;
    dat_buffer[buf_last_pos][1] = current_time;
    
    buf_last_pos++;
    if (buf_last_pos == BUF_SIZE)
      buf_last_pos = 0;
    
    current_time = 0;
  }
}
