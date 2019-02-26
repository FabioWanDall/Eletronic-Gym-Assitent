//Programa :
//Autor :

//Carrega a biblioteca para utilizar o TIMER1
#include "TimerOne.h"

//Carrega a biblioteca do encoder
#include <RotaryEncoder.h>
//Pinos de ligacao do encoder
RotaryEncoder encoder(A2, A3);

//Carrega biblioteca para ter uma nova porta serial
#include <SoftwareSerial.h>
//Pinos da porta serial
SoftwareSerial BT(10, 11); //RX, TX

/**Morrera em breve**/
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

enum e_modo{
  CONTA_REPETICAO,
  CONTA_DESCANSO,
  CONTA_SERIE,
  CONFIGURA_REPETICAO,
  FIM
};
e_modo modo_atual = CONTA_REPETICAO;

/**Morrera em breve**/
int pin_buzzer = 10;

/**Morrera em breve**/
int pin_sw_enter = 8;
int pin_sw_esc = 9;
int pin_sw_inc = 11; //increment
int pin_sw_dec = 12; //decrement

//Variavel para o encoder
int pin_sw_encoder = 13;

/**Morrera em breve**/
int valor_sw = 0;

int newposition = 0;
int position = 0;
int initialPosition = 100;
int positionWhenChangeSentido = 0; //nao sei como é sentido em ingles

//Variaveis de controle
boolean new_sentido_horario = true;//true quando incrementa
boolean sentido_horario = true;
int cont_repeticoes = 0;//conta as repeticoes executadas
int cont_time = 0;//utilizado como o proprio relogio
int cont_series = 0;//conta a quantidade de series ja feitas
int time_of_last_move = 0;//guarda o tempo do ultimo movimento para determinar time out na execucao
int tempo_timeout_execucao = 3;//tempo maximo aceito sem movimento(MEDIDO EM SEGUNDOS)

int minimo_movimento = 5; //define qual o movimento minimo no sentido anti-horario para validar uma execucao  (PODERA SER ALTERADA PELO USUARIO)
int numero_repeticoes_programado = 5; //define a quantidade de repeticoes que deverao ser executadas          (PODERA SER ALTERADA PELO USUARIO)
int tempo_descanso_programado = 10; //define quantos SEGUNDOS de descanso                                     (PODERA SER ALTERADA PELO USUARIO)
int numero_series_programado = 2; //define quantas series deverao ser feitas                                  (PODERA SER ALTERADA PELO USUARIO)

boolean avisou = false;//aviso que o numero de repetiçoes foi alcançado

void setup(){
  pinMode(pin_sw_encoder, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("The System is Alive");

  //HC-05 usually default baud-rate
  BT.begin(9600); 
  
  /**Morrera em breve**/
  pinMode(pin_buzzer,OUTPUT); //Pino do buzzer

  /**Morrera em breve**/
  pinMode(pin_sw_enter, INPUT_PULLUP);
  //pinMode(pin_sw_esc, INPUT_PULLUP);
  //pinMode(pin_sw_inc, INPUT_PULLUP);
  //pinMode(pin_sw_dec, INPUT_PULLUP);
  
  /**Morrera em breve**/
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);

  //define uma posição alta para nao haver troca de sinais(causa estouro)
  encoder.setPosition(initialPosition);

  //Inicializa e define funçao
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(callback);
  
  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.

  ini();
}

void ini(){
  BT.println("EGA BETA Z");
  
  /**Morrera em breve**/
  lcd.setCursor(0, 0);
  lcd.print(" POLLIN MONITOR");
  lcd.setCursor(3, 1);
  lcd.print("      V0.8");
  delay(1000);

  position = initialPosition;
  newposition = initialPosition;
  
  change_state(CONTA_REPETICAO);  
}

// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A2 and A3: exactly where we need to check.
ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
}

void callback() {
  cont_time++;
}

/**Morrera em breve**/
void toca_alerta_repeticoes_programado(){
  int frequencia = 0;
  int tempo = 1;
  
  for (frequencia = 1300; frequencia < 1800; frequencia += 1) 
  {
    tone(pin_buzzer, frequencia, tempo); 
    delay(1);
  }
}

void tela_cont_repeticoes(){
    BT.print("REPETICOES:");
    BT.print(cont_repeticoes);
    BT.print("/");
    BT.println(numero_repeticoes_programado);
    
    /**Morrera em breve**/
    //APRESENTA A QUANTIDADE DE REPETICOES REALIZADAS
    //"REPETICOES:XX/XX"
    lcd.setCursor(0, 0);
    lcd.print("REPETICOES:");
    lcd.setCursor(11, 0);
    lcd.print(cont_repeticoes);  
    lcd.setCursor(13, 0);
    lcd.print("/");  
    lcd.setCursor(14, 0);
    lcd.print(numero_repeticoes_programado);  

    /**Morrera em breve**/
    //---teste---
    //APRESENTA POSICAO DO ENCODER
    //"POS:XXX"
    //lcd.setCursor(0, 1);
    //lcd.print("POS:");
    lcd.setCursor(4, 1);
    lcd.print(newposition);
}

void tela_timer(){
    BT.print("TIME:");
    BT.println(cont_time); 
  
    /**Morrera em breve**/
    //APRESENTACAO DO TIMER
    lcd.setCursor(8, 1);
    lcd.print("TIME:");
    lcd.setCursor(13, 1);
    lcd.print(cont_time);  
}

void tela_timer_descanso(){
    BT.print("TEMPO DESCANSO:");
    BT.print(cont_time);
    BT.print("/");
    BT.println(tempo_descanso_programado); 
  
    /**Morrera em breve**/
    //APRESENTACAO DO TIMER REGRECIVO 
    lcd.setCursor(0, 0);
    lcd.print("TEMPO DESCANSO:");
    lcd.setCursor(1, 1);
    lcd.print(cont_time);  
    lcd.setCursor(4, 1);
    lcd.print("/");
    lcd.setCursor(5, 1);
    lcd.print(tempo_descanso_programado);  
}

void tela_cont_series(){
  lcd.setCursor(0, 0);
  lcd.print("SERIES:");
  lcd.setCursor(8, 0);
  lcd.print(cont_series);
  lcd.setCursor(10, 0);
  lcd.print("/");
  lcd.setCursor(11, 0);
  lcd.print(numero_series_programado);  
}

boolean change_position(){
  return (position != newposition);
}

void increment_cont_repeticoes(){
  if (abs(positionWhenChangeSentido - position) >= minimo_movimento)//SE atingir a movimentacao minima para considerar uma repeticao
  {
    if (sentido_horario)//só incrementa execucao se for no sentido horario para nao duplicar
      cont_repeticoes++;
    positionWhenChangeSentido = position;//depois de voltar define um novo ponto de inicio do movimento 
  }  
}

int contando_repeticoes(){   
  //Le as informacoes do encoder
  newposition = encoder.getPosition();

  if (change_position()){
    time_of_last_move = cont_time;
    new_sentido_horario = position < newposition;//TRUE SE esta girando no sentido horario
  
    if (new_sentido_horario != sentido_horario)//SE mudar o sentido
    {
      sentido_horario = new_sentido_horario;
      increment_cont_repeticoes();//incrementa se cumprir condicoes de posicionamento
    }
    
    tela_cont_repeticoes();
  
    position = newposition;
  }
  return cont_repeticoes;
}

boolean timeout_execucao(){
  return ((cont_time - time_of_last_move) >= tempo_timeout_execucao);
}

void modo_conta_repeticao(){
  if(contando_repeticoes() >= numero_repeticoes_programado and !avisou){
    lcd.setCursor(8, 1);
    //lcd.print("R#");
    toca_alerta_repeticoes_programado();
    avisou = true;
  }
  if(timeout_execucao()){
    
    if(position == initialPosition){
      cont_repeticoes = 0;
      change_state(CONTA_SERIE);
    }
    
    lcd.setCursor(12, 1);
    //lcd.print("TO");
  }
}

int contando_series(){
  cont_series++;
  return cont_series;
}

void modo_conta_serie(){
  if(contando_series() >= numero_series_programado){
    //---dar algum aviso de fim---
    lcd.setCursor(10, 1);
    
  //lcd.print("S#");
    //---deligar sistema---
    delay(1000);
    change_state(FIM);
  }
  else{
    change_state(CONTA_DESCANSO);
  }
  tela_cont_series();
  delay(2000);
}

void modo_conta_descanso(){
  tela_timer_descanso();
  if(cont_time > tempo_descanso_programado){
    change_state(CONTA_REPETICAO);
  toca_alerta_repeticoes_programado();
  }
}

void escreve_display(char Str1[15]){
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(Str1);
    delay(1000);
}

void escreve_display(int num){
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(num);
    delay(1000);
}

void change_state(e_modo novo_modo){
  switch(novo_modo){
    case  CONTA_REPETICAO:
      modo_atual = CONTA_REPETICAO;
    avisou = false; //aviso que o numero de repetiçoes foi alcançado
      time_of_last_move = 100;//posso zerar o cont_timer e usar esse 100 como um time out de maquina parada(nao sei bem)
      lcd.clear();
      tela_cont_repeticoes();
    break;
    case  CONTA_SERIE:
      modo_atual = CONTA_SERIE;//ALTERNA PARA MODO CONTA_SERIE
      cont_time = 0;
      lcd.clear();
    break;  
    case  CONTA_DESCANSO:
      modo_atual = CONTA_DESCANSO;//ALTERNA PARA MODO CONTA_DESCANSO
      cont_time = 0;
      lcd.clear();
    break;  
    case  FIM:
      modo_atual = FIM;//ALTERNA PARA MODO CONTA_DESCANSO
      cont_time = 0;
      lcd.clear();
    break;  
  }
  
}

void verifica_teclado(){
  int valor = digitalRead(8);
  if (valor != 1)
  {
    Serial.println("Botao 8 pressionado");
    while (digitalRead(8) == 0)
      delay(10);
  }
  valor = digitalRead(9);
  if (valor != 1)
  {
    Serial.println("Botao 9 pressionado");
    while (digitalRead(9) == 0)
      delay(10);
  }
  valor = digitalRead(11);
  if (valor != 1)
  {
    Serial.println("Botao 11 pressionado");
    while (digitalRead(11) == 0)
      delay(10);
  }
  valor = digitalRead(12);
  if (valor != 1)
  {
    Serial.println("Botao 12 pressionado");
    while (digitalRead(12) == 0)
      delay(10);
  }
  valor = digitalRead(13);
  if (valor != 1)
  {
    Serial.println("Encoder pressionado");
    while (digitalRead(7) == 0)
      delay(10);
  }
  
}

void loop(){
  switch(modo_atual){
    case  CONTA_REPETICAO:
      modo_conta_repeticao();
    break;  
    case  CONTA_SERIE:
      modo_conta_serie();
    break;
    case  CONTA_DESCANSO:
      modo_conta_descanso();
    break;  
    case  FIM:
      escreve_display("  !!!FESHOW!!!");
    toca_alerta_repeticoes_programado();
    toca_alerta_repeticoes_programado();
    toca_alerta_repeticoes_programado();
      delay(3000);
    break;  
  }
  //Serial.println(modo_atual);
  verifica_teclado();
}
