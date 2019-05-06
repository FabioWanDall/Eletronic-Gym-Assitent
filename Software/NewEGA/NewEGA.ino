//Carrega a biblioteca do encoder
#include <RotaryEncoder.h>
//Pinos de ligacao do encoder
RotaryEncoder encoder(A2, A3);

int initialPosition = 100;
int position = 0;
int newPosition = 0;
int positionWhenChangeDirection = 0; 

//Variaveis de controle
boolean newDirection = true;//true quando incrementa
boolean direction = true;
int repetitionsCounter = 0;//conta as repeticoes executadas
unsigned long lastMovementTime = 0;//guarda o tempo do ultimo movimento para determinar time out na execucao
unsigned int executionTimeout = 3000;//tempo maximo aceito sem movimento(MEDIDO EM MILISEGUNDOS)
unsigned long lastChangeDirectionTime = 0;

int minimumMovement = 5; //define qual o movimento minimo no sentido anti-horario para validar uma execucao  (PODERA SER ALTERADA PELO USUARIO)
//int numero_repeticoes_programado //talvez não se usa = 5; //define a quantidade de repeticoes que deverao ser executadas          (PODERA SER ALTERADA PELO USUARIO)

void setup() {
  Serial.begin(38400);
  //Serial.println("The System is Alive");
  Serial.println("CLEARDATA");            // Reset da comunicação serial
  Serial.println("LABEL,Repeticao,Movimento,Tempo(ms)");   // Nomeia as colunas

  encoder.setPosition(initialPosition); //Aquele velho probleminha de convesão
  //Interrupção
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.
}

// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A2 and A3: exactly where we need to check.
ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
}

boolean position_changed(){
  return (position != newPosition);
}

int repetitions_counter(){   
  //Le as informacoes do encoder
  newPosition = encoder.getPosition();

  if (position_changed()){
    lastMovementTime = millis();
    newDirection = position < newPosition;//TRUE SE esta girando no sentido horario
  
    if (newDirection != direction)//SE mudar o sentido
    {
      direction = newDirection;
      increment_repetitions_counter();//incrementa se cumprir condicoes de posicionamento
      lastChangeDirectionTime = millis();
    }

    ////tela_cont_repeticoes();
  
    position = newPosition;
  }
  return repetitionsCounter;
}

void increment_repetitions_counter(){
  int movement = abs(positionWhenChangeDirection - position);
  if (movement >= minimumMovement)//SE atingir a movimentacao minima para considerar uma repeticao
  {
    if (direction)//só incrementa execucao se for no sentido horario para nao duplicar
      repetitionsCounter++;
    positionWhenChangeDirection = position;//depois de voltar define um novo ponto de inicio do movimento 
    
    Serial.print("DATA,");
    Serial.print(repetitionsCounter);
    Serial.print(",");
    Serial.print(movement);
    Serial.print(",");
    Serial.println(millis()-lastChangeDirectionTime);
  }  
}

void modo_conta_repeticao(){
  /*if(repetitions_counter() >= numero_repeticoes_programado and !avisou){
    lcd.setCursor(8, 1);
    BT.print("R#_FinishedRepetition");
    ////toca_alerta_repeticoes_programado();
    avisou = true;
  }*/
  if(timeout()){
    if(position == initialPosition){
      repetitionsCounter = 0;
      ////change_state(CONTA_SERIE);
    }
    ////BT.print("TO_TimeOut");
  }
}

boolean timeout(){
  return (millis() - lastMovementTime > executionTimeout);
}
void loop() {
  // put your main code here, to run repeatedly:
  repetitions_counter();
  //Serial.println(encoder.getPosition());
}
