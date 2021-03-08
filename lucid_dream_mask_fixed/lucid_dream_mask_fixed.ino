/*
Created 2016
by AlexGyver
AlexGyver Home Labs Inc.
*/

#include <avr/sleep.h>    //подключаем библиотеку режимов энергопотребления
extern volatile unsigned long timer0_millis;;   //переменная для хранения времени таймера (миллисекунды!)

int LED[]={3,5,6,9,10,11};   //массив пинов, к которым подключены светодиоды
/*Режимы мигания (взято по аналогии с http://www.sleepwithremee.com/config.php?)
* 0 - Sweep Left
* 1 - Sweep Right
* 2 - Sweep Bounce
* 3 - Glow Left
* 4 - Glow Right
* 5 - Sparkle (random)
* 6 - Alt Blink
* 7 - Outside In
* 8 - Split Bounce
* 9 - Glow Alt
*/
int mode=1;  //выбор режима мигания (0-9)
float del_start=0.000166;    //задержка до включения (В ЧАСАХ! в виде 2.5 , 4.5 - десятичная дробь, разделитель - точка)
unsigned long del_blink=1;    //время между показами мерцания режима (В МИНУТАХ)
unsigned long del_show=5;    //время работы режима (В МИНУТАХ)
int n=20;            //столько раз повторить мерцание в режиме
int brightness=10;  //яркость от 0 до 10
byte blink_speed=1;  //скорость мерцания, 0 быстро, 1 медленно

unsigned long mode_time, start_timer; //хранение времени работы режима и стартового таймера
byte startflag, flag;   //флажочки :3
int pin;   //переменная pin для режима "случайный"

void setup() {
  start_timer=floor((float)del_start*60*60*1000);  //перевод часов в миллисекунды  

    for (int i=0;i<=5;i++) { //делаем все светодиодные пины выходами
    pinMode(LED[i],1);
  }
  /*delay(5000);   //задержка перед снижением частоты процессора
  
  noInterrupts();    //снижаем частоту
  CLKPR = 1<<CLKPCE;
  CLKPR = 8;
  interrupts();          //частота снижена до минимальной
  */
}
void fade_up(int x) {        //функция для плавного зажигания 
  if (x < 0 || x > 5) return;
  for (int i=0;i<=50;i++) {
    analogWrite(LED[x],i*brightness/50);
    delay(1+blink_speed);
  }
}
void fade_down(int x) {        //функция для плавного затухания
  if (x < 0 || x > 5) return; 
  for (int i=40;i>=0;i--) {
    analogWrite(LED[x],i*brightness/40);
    delay(1+blink_speed);
  } 
}

void loop() {
  while (timer0_millis < start_timer) {  //задержка перед включением режима (несколько часов)
    set_sleep_mode(SLEEP_MODE_IDLE); // выбор режима энергопотребления
    sleep_mode();                    // уходим в спячку
  }
  /*if (startflag==0) {
    noInterrupts();    //вышли из спячки, возвращаем частоту процессора
    CLKPR = 1<<CLKPCE;
    CLKPR = 0;
    interrupts();      //закончили
    startflag=1;       //и флажок подняли, чтобы больше проц не трогать
  }*/
  brightness=map(brightness,0,10,0,254);  //перевод яркости с 0-10 до 0-254 (для analogWrite)
  del_blink=del_blink*60*1000;  //перевод минут в миллисекунды
  del_show=del_show*60*1000;  //перевод минут в миллисекунды

  /* 
*  дальше следуют все режимы, включается один из них 
*  согласно выбору в самом начале скетча
*  каждый имеет структуру
*    if (НОМЕР РЕЖИМА И ФЛАЖОК ОПУЩЕН) {
mode_time=millis();                     //запомнить время старта режима (lzk контроля длительности)
while (millis()-mode_time<del_show) {   //контроль длительности режима (как долько дольше del_show заканчиваем
for (int i=1;i<=n;i++) {              //выполнить режим n раз
КОД РЕЖИМА
}
delay(del_blink);                     //задержка между одним циклом режима
}
flag=1;                                 //поднять паруса! Ой, флаг. Очки отработали и больше не запустятся
}
*/

  if (mode==0 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=0;i<=6;i++) {
          fade_up(i);
          fade_down(i-1);
          delay(150+blink_speed*120); 
        }
      }
      delay(del_blink);
    }
    flag=1;
  }

  if (mode==1 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=5;i>=-1;i--) {
          fade_up(i);
          fade_down(i+1);
          delay(150+blink_speed*120); 
        }
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==2 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=5;i>=1;i--) {
          fade_up(i);
          fade_down(i+1);
          delay(100+blink_speed*150); 
        }
        for (int i=0;i<=4;i++) {
          fade_up(i);
          fade_down(i-1);
          delay(100+blink_speed*150); 
        }
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==3 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=0;i<=50;i++) {
          analogWrite(LED[5],i*brightness/50);
          analogWrite(LED[4],i*brightness/50);
          analogWrite(LED[3],i*brightness/50);
          delay(1+blink_speed);
        }
        delay(100+blink_speed*250);
        for (int i=50;i>=0;i--) {
          analogWrite(LED[5],i*brightness/50);
          analogWrite(LED[4],i*brightness/50);
          analogWrite(LED[3],i*brightness/50);
          delay(1+blink_speed);
        }
        delay(100+blink_speed*250);
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==4 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=0;i<=50;i++) {
          analogWrite(LED[0],i*brightness/50);
          analogWrite(LED[1],i*brightness/50);
          analogWrite(LED[2],i*brightness/50);
          delay(1+blink_speed);
        }
        delay(100+blink_speed*250);
        for (int i=50;i>=0;i--) {
          analogWrite(LED[0],i*brightness/50);
          analogWrite(LED[1],i*brightness/50);
          analogWrite(LED[2],i*brightness/50);
          delay(1+blink_speed);
        }
        delay(100+blink_speed*250);
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==5 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        pin=random(0,6);
        fade_up(pin);
        delay(100+blink_speed*200);
        fade_down(pin);        
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==6 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=0;i<=1;i++) {
          for (int i=0;i<=50;i++) {
            analogWrite(LED[0],i*brightness/50);
            analogWrite(LED[2],i*brightness/50);
            analogWrite(LED[3],i*brightness/50);
            analogWrite(LED[5],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(50+blink_speed*100);
          for (int i=50;i>=0;i--) {
            analogWrite(LED[0],i*brightness/50);
            analogWrite(LED[2],i*brightness/50);
            analogWrite(LED[3],i*brightness/50);
            analogWrite(LED[5],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(50+blink_speed*100);
        }
        for (int i=0;i<=1;i++) {
          for (int i=0;i<=50;i++) {
            analogWrite(LED[1],i*brightness/50);
            analogWrite(LED[4],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(50+blink_speed*100);
          for (int i=50;i>=0;i--) {
            analogWrite(LED[1],i*brightness/50);
            analogWrite(LED[4],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(50+blink_speed*100);
        }
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==7 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=0;i<=1;i++) {
          analogWrite(LED[0],brightness);
          analogWrite(LED[5],brightness);
          delay(100+blink_speed*100);
          analogWrite(LED[0],0);
          analogWrite(LED[5],0);
          delay(100+blink_speed*100);
        }
        for (int i=0;i<=1;i++) {
          analogWrite(LED[1],brightness);
          analogWrite(LED[4],brightness);
          delay(100+blink_speed*100);
          analogWrite(LED[1],0);
          analogWrite(LED[4],0);
          delay(100+blink_speed*100);
        }
        for (int i=0;i<=1;i++) {
          analogWrite(LED[2],brightness);
          analogWrite(LED[3],brightness);
          delay(100+blink_speed*100);
          analogWrite(LED[2],0);
          analogWrite(LED[3],0);
          delay(100+blink_speed*100);
        }
        for (int i=0;i<=1;i++) {
          analogWrite(LED[1],brightness);
          analogWrite(LED[4],brightness);
          delay(100+blink_speed*100);
          analogWrite(LED[1],0);
          analogWrite(LED[4],0);
          delay(100+blink_speed*100);
        }
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==8 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=1;i<=2;i++) {
          analogWrite(LED[i],brightness);
          analogWrite(LED[5-i],brightness);
          analogWrite(LED[i-1],0);
          analogWrite(LED[6-i],0);
          delay(150+blink_speed*100); 
        }
        for (int i=1;i>=0;i--) {
          analogWrite(LED[i],brightness);
          analogWrite(LED[5-i],brightness);
          analogWrite(LED[1+i],0);
          analogWrite(LED[4-i],0);
          delay(150+blink_speed*100); 
        }
      }
      analogWrite(LED[0],0);
      analogWrite(LED[5],0);
      delay(del_blink);
    }
    flag=1;
  }
  if (mode==9 && flag==0) {
    mode_time=millis();
    while (millis()-mode_time<del_show) {
      for (int i=1;i<=n;i++) {
        for (int i=0;i<=1;i++) {
          for (int i=0;i<=50;i++) {
            analogWrite(LED[5],i*brightness/50);
            analogWrite(LED[4],i*brightness/50);
            analogWrite(LED[3],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(150+blink_speed*100);
          for (int i=50;i>=0;i--) {
            analogWrite(LED[5],i*brightness/50);
            analogWrite(LED[4],i*brightness/50);
            analogWrite(LED[3],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(150+blink_speed*100);
        }
        for (int i=0;i<=1;i++) {
          for (int i=0;i<=50;i++) {
            analogWrite(LED[0],i*brightness/50);
            analogWrite(LED[1],i*brightness/50);
            analogWrite(LED[2],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(150+blink_speed*100);
          for (int i=50;i>=0;i--) {
            analogWrite(LED[0],i*brightness/50);
            analogWrite(LED[1],i*brightness/50);
            analogWrite(LED[2],i*brightness/50);
            delay(1+blink_speed);
          }
          delay(150+blink_speed*100);
        }        
      }
      delay(del_blink);
    }
    flag=1;
  }
  if (flag==1) {
    set_sleep_mode(SLEEP_MODE_IDLE); // выбор режима энергопотребления
    sleep_mode();                    // уходим в спячку
  }
}
