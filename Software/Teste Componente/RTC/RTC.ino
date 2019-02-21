//Programa : Relogio com modulo RTC DS1307
//Autor : FILIPEFLOP
 
//Carrega a biblioteca do RTC DS1307
#include <DS1307.h>
 
//Modulo RTC DS1307 ligado as portas A4 e A5 do Arduino 
DS1307 rtc(A4, A5);
 
void setup()
{
  //Aciona o relogio
  rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  //rtc.setDOW(THURSDAY);      //Define o dia da semana
  //rtc.setTime(22, 20, 0);     //Define o horario
  //rtc.setDate(14, 2, 2018);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
   
  Serial.begin(9600);
  Serial.print("Oieee");
}
 
void loop()
{
  //Mostra as informações no Serial Monitor
  Serial.print("Hora : ");
  Serial.print(rtc.getTimeStr());
  Serial.print(" ");
  Serial.print("Data : ");
  Serial.print(rtc.getDateStr());
  Serial.print(" ");
  Serial.println(rtc.getDOWStr());
   
  //Aguarda 1 segundo e repete o processo
  delay (1000);
}