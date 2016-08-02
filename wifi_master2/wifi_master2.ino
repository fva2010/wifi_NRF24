#include <SPI.h>
#include "RF24.h"
RF24 radio(9, 10);

const uint64_t pipes[2] = {0xF0F0F0F000LL, 0xF0F0F0F0FFLL};
// адреса каналов приема и передачи
char code()
{
  char sym = '-';
  char symbols[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
  for(short i = 2; i < 6; i++)
  {
      digitalWrite(i, HIGH);
      sym = '-';
      for(short j = 14; j < 17; j++)
      {
        if(digitalRead(j) == HIGH)
        {
          delay(15);
          sym = symbols[(i-2)*3 + (j-14)]; 
          digitalWrite(7, LOW);   
          while(digitalRead(j) == HIGH);
          digitalWrite(7, HIGH);   
          delay(15);
          return sym;
        }
      }
      digitalWrite(i, LOW);
  }
  return '-';
}

void beep()
{
  int pause[30] = {657, 624, 592, 561, 530, 500, 470, 441, 
  412, 384, 357, 331, 305, 280, 256, 232, 209, 187, 166, 146,
  126, 108, 90, 74, 58, 44, 32, 21, 11, 4};
  int tone[30] = {86,87,90,93,96,99,103,107,112,116,121,
                126,131,137,143,149,155,161,167,174,181,188,
                195,202,210,217,225,233,241,249};

  for(short n = 0; n < 31; n++)
  {
    digitalWrite(7, LOW);
    analogWrite(6, tone[n]);
    delay(pause[n]);
    digitalWrite(7, HIGH);
    analogWrite(6, 0);
  }
}
void setup(){
  //-----keyboard rows-----------
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //----------------------
  pinMode(7, OUTPUT);//signal diode
  pinMode(6, OUTPUT);//biper
  digitalWrite(6, HIGH);   
  //--------------------------
  radio.begin();  
  radio.setDataRate(RF24_250KBPS); // Скорость передачи
  radio.setChannel(100); // Номер канала от 0 до 127
  radio.setRetries(15,15); // Кол-во попыток и время между попытками
  radio.openWritingPipe(pipes[1]);  // Открываем канал передачи
  radio.openReadingPipe(1, pipes[0]); // Открываем один из 6-ти каналов приема
  radio.startListening(); // Начинаем слушать эфир
}

char mychar[10]= {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}, rightchar[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
int k = 0;
//-----------main loop------------------
void loop()
{
    bool flag = true;
    char temp = code();
    if(temp != '-' && temp != '*' && temp != '#' && k < 10)
    {
      mychar[k] = temp;
      k++;
      temp = '-';
    }
    if(k == 10 && temp == '#')
    {
      flag = true;
      for (short l = 0; l < 10; l++)
      {
        if(mychar[l] != rightchar[l])
        {
          flag = false;
          break;
        }
      }
      if(flag)
      {
        beep();
        int data[1] = {1};
        radio.stopListening();
        radio.write(&data, 1);
        radio.startListening();
      }
      else 
      {
        for(short t = 0; t < 3; t++)
        {
          digitalWrite(7, LOW);
          delay(300);
          digitalWrite(7, HIGH);
          delay(300);
        }
        flag = true;
        for (short l = 0; l < 10; l++)
        {
          mychar[l] = '-';
        }
      }
    }
 }
