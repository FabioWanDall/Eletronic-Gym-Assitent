#include <RotaryEncoder.h>
RotaryEncoder encoder(A2, A3);

#define BUF_SIZE 200
volatile int dat_buffer[BUF_SIZE][2];
int buf_first_pos = 0;
int buf_last_pos = 0;

unsigned long long last_update_time = 0;
int position = 0;
int newPos = 0;

String data;

void setup() {
  Serial.begin(115200);
  
  PCICR |= (1 << PCIE1); //Habilita Interrupção 
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);
}

ISR(PCINT1_vect) {
  encoder.tick();
  update_position();
}

void loop() {
  if (buf_first_pos != buf_last_pos) {
    data = "";
    data.concat(dat_buffer[buf_first_pos][0]);
    data.concat(","); 
    data.concat(dat_buffer[buf_first_pos][1]);
    data.concat(",");
    Serial.println(data);
    buf_first_pos++;
    if (buf_first_pos == BUF_SIZE) {
      buf_first_pos = 0;
    }
  }
  delay(10);
}

void update_position() {
  newPos = encoder.getPosition();
  if (position != newPos) {
    position = newPos;
    
    dat_buffer[buf_last_pos][0] = position;
    dat_buffer[buf_last_pos][1] = millis() - last_update_time;
    
    buf_last_pos++;
    if (buf_last_pos == BUF_SIZE)
      buf_last_pos = 0;
    
    last_update_time = millis();
  }
}
