#include <RotaryEncoder.h>
#include <FlexiTimer2.h>

#define BUF_SIZE 200
volatile int dat_buffer[BUF_SIZE][2];
int buf_first_pos = 0;
int buf_last_pos = 0;

#define UPDATE_TIME 0.000001 // ms
int current_time = 0;

RotaryEncoder encoder(A2, A3);

#define ENCODER_START_THRESHOLD 10
int value = 0;
int enc_new_pos = 0;

void setup() {
  Serial.begin(115200);
  
  FlexiTimer2::set(UPDATE_TIME, capture_data);
  FlexiTimer2::start();
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
  //Serial.println(encoder_position);
  delay(10);
}

void capture_data() {
  current_time++;

  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  //Se a posicao foi alterada, mostra o valor
  //no Serial Monitor
  if (pos != newPos) {
    buf_last_pos++;
    if (buf_last_pos == BUF_SIZE)
      buf_last_pos = 0;
    
    dat_buffer[buf_last_pos][0] = pos;
    dat_buffer[buf_last_pos][1] = current_time;
    
    pos = newPos;
    current_time = 0;
  }
}
