#include <TM1637.h>
#include <iarduino_RTC.h>
iarduino_RTC watch(RTC_DS3231);
#define CLK 13
#define DIO 12
#define S0 16
#define S1 0
#define S2 2
#define S3 15
#define SIGMULTIPLEXOR 14
#define SIG A0
TM1637 tm1637(CLK, DIO);
int dayy = 0;
int monthh = 0;
int yearr = 0;
int year1 = 0;
int hour1, hour2;
int minute1;
int day1;
int day2;
int weekday1, weekday2;
int month1;
int month2;
int prevsec = -999;
int nowsec, nowsec1, nowsec2;
int t, sw, plus, minus;
int x;
int m;
int y, d, h, minn, dn, trash;
int8_t DispMSG[4] {8, 2, 3, 4};
int para[7] = {480, 570, 660, 750, 810, 900, 1020};
//8, 9.30, 11, 12.30, 14, 15.30,17
int del = 300;
int count;
int jj;
int dennedeli[6][4] = {
  {0, 1, 1, 0}, //channel 6
  {1, 1, 1, 0}, //channel 7
  {0, 0, 0, 1}, //channel 8
  {1, 0, 0, 1}, //channel 9
  {0, 1, 0, 1}, //channel 10
  {1, 1, 0, 1}, //channel 11
};
int parasvet[6][4] = {
  {0, 0, 0, 0}, //channel 0
  {1, 0, 0, 0}, //channel 1
  {0, 1, 0, 0}, //channel 2
  {1, 1, 0, 0}, //channel 3
  {0, 0, 1, 0}, //channel 4
  {1, 0, 1, 0}, //channel 5
};
void setup() {
  sw = 1;
  tm1637.init();
  tm1637.set(BRIGHTEST);
  watch.begin();
  watch.settime(0, 10, 15, 10, 1, 24, 6);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(SIGMULTIPLEXOR, OUTPUT);
  pinMode(SIG, INPUT);


}
void loop() {
  switch (sw) {
    case 1:
      watch.gettime();
      nowsec = watch.seconds;
      minute1 = watch.minutes;
      hour1 = watch.Hours;
      day1 = watch.day;
      day2 = watch.day;
      month1 = watch.month;
      month2 = watch.month;
      yearr = watch.year;
      weekday1 = watch.weekday;
      day1 = day1 / 10;
      day2 = day2 % 10;
      nowsec1 = nowsec / 10;
      nowsec2 = nowsec % 10;
      month1 = month1 / 10;
      month2 = month2 % 10;
      if (nowsec == 10 or nowsec == 20 or nowsec == 30 or nowsec == 40 or nowsec == 50 or nowsec == 0) {
        DispMSG[0] = day1;
        DispMSG[1] = day2;
        DispMSG[2] = month1;
        DispMSG[3] = month2;
      }
      if (nowsec == 5 or nowsec == 15 or nowsec == 25 or nowsec == 35 or nowsec == 45 or nowsec == 55) {
        year1 = 2;
        DispMSG[0] = year1;
        year1 = 0;
        DispMSG[1] = year1;
        year1 = yearr / 10;
        DispMSG[2] = year1;
        year1 = yearr % 10;
        DispMSG[3] = year1;
      }
      tm1637.display(DispMSG);
      t = analogRead(SIG);
      for (int j; j < 6; j++) {
        hour2 = hour1 * 60 + minute1;
        int p = j + 1;
        if (para[j] <= (hour2) and para[p] > (hour2)) {
          jj = j;
          weekday2 = weekday1 - 1;
          digitalWrite(S0, parasvet[jj][0]);
          digitalWrite(S1, parasvet[jj][1]);
          digitalWrite(S2, parasvet[jj][2]);
          digitalWrite(S3, parasvet[jj][3]);
          digitalWrite(SIGMULTIPLEXOR, HIGH);
          delay(10);
          digitalWrite(S0, dennedeli[weekday2][0]);
          digitalWrite(S1, dennedeli[weekday2][1]);
          digitalWrite(S2, dennedeli[weekday2][2]);
          digitalWrite(S3, dennedeli[weekday2][3]);
          digitalWrite(SIGMULTIPLEXOR, HIGH);
        }
      }


      if (t > 600 and t < 800) {
        delay(del);
        sw = 2;
      }
      break;


    case 2:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;

      }
      if (x >= 3 || x <= 0) {
        x = 1;
      }
      DispMSG[0] = x;
      DispMSG[1] = 0;
      DispMSG[2] =  0;
      DispMSG[3] =  0;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        if (x == 1) {
          count = 0;
          delay(del);
          sw = 55;
        } else {
          delay(del);
          sw = 3;
        }
      }
      break;

    //год 20х3
    case 3:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      DispMSG[0] = 2;
      DispMSG[1] =  0;
      DispMSG[2] =  x;
      year1 = yearr % 10;
      DispMSG[3] = year1;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 4;
        y = x * 10;
      }
      break;

    //год 202х
    case 4:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      DispMSG[0] = 2;
      DispMSG[1] =  0;
      trash =  y / 10;
      DispMSG[2] =  trash;
      year1 = x;
      DispMSG[3] = year1;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 6;
        y += x;
      }
      break;

    //день х2

    case 6:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 4 || x <= -1) {
        x = 0;
      }
      DispMSG[0] = x;
      DispMSG[1] = day2;
      DispMSG[2] = month1;
      DispMSG[3] = month2;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 7;
        d = x * 10;
      }
      break;
    //день 2х

    case 7:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 9 || x <= -1) {
        x = 0;
      }
      trash = d / 10;
      DispMSG[0] = trash;
      DispMSG[1] = x;
      DispMSG[2] = month1;
      DispMSG[3] = month2;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 8;
        d += x;
      }
      break;

    //месяц х2
    case 8:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 2 || x <= -1) {
        x = 0;
      }
      trash = d / 10;
      DispMSG[0] = trash;
      trash = d % 10;
      DispMSG[1] = trash;
      DispMSG[2] = x;
      DispMSG[3] = month2;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 9;
        m = x * 10;
      }
      break;
    //месяц 2х
    case 9:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      trash = d / 10;
      DispMSG[0] = trash;
      trash = d % 10;
      DispMSG[1] = trash;
      trash = m % 10;
      DispMSG[2] = trash;
      DispMSG[3] = x;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 10;
        m += x;
      }
      break;



    //час х2
    case 10:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 3 || x <= -1) {
        x = 0;
      }
      DispMSG[0] = x;
      trash = hour1 % 10;
      DispMSG[1] = trash;
      trash = minute1 / 10;
      DispMSG[2] = trash;
      trash = minute1 % 10;
      DispMSG[3] = trash;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 11;
        h = x * 10;
      }
      break;
    //час 2x
    case 11:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      trash = h / 10;
      DispMSG[0] = trash;
      DispMSG[1] = x;
      trash = minute1 / 10;
      DispMSG[2] = trash;
      trash = minute1 % 10;
      DispMSG[3] = trash;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 12;
        h += x;
      }
      break;
    //минута х2
    case 12:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 6 || x <= -1) {
        x = 0;
      }
      trash = h / 10;
      DispMSG[0] = trash;
      trash = h % 10;
      DispMSG[1] = trash;
      trash = x;
      DispMSG[2] = trash;
      trash = minute1 % 10;
      DispMSG[3] = trash;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 13;
        minn = x * 10;
      }
      break;
    //минута 2x
    case 13:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      trash = h / 10;
      DispMSG[0] = trash;
      trash = h % 10;
      DispMSG[1] = trash;
      trash = minn / 10;
      DispMSG[2] = trash;
      DispMSG[3] = x;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 14;
        minn += x;
      }
      break;
    //день недели
    //(0-воскресенье, 6-суббота)
    case 14:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        x++;
        delay(del);
        DispMSG[0] =  x;
      } if (t > 900) {
        x--;
        delay(del);
      }
      if (x >= 8 || x <= 0) {
        x = 1;
      }
      DispMSG[0] = x;
      DispMSG[1] = 0;
      DispMSG[2] = 0;
      DispMSG[3] = 0;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        if (x == 7) {
          dn = 0;
        } else {
          dn = x;
        }
        delay(del);
        sw = 15;
      }

      break;

    case 15:
      watch.settime(0, minn, h, d, m, y, dn);
      sw = 1;
      break;

    //час х2
    case 55:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 3 || x <= -1) {
        x = 0;
      }
      DispMSG[0] = x;
      trash = hour1 % 10;
      DispMSG[1] = trash;
      trash = minute1 / 10;
      DispMSG[2] = trash;
      trash = minute1 % 10;
      DispMSG[3] = trash;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 56;
        h = x * 10;
      }
      break;
    //час 2x
    case 56:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      trash = h / 10;
      DispMSG[0] = trash;
      DispMSG[1] = x;
      trash = minute1 / 10;
      DispMSG[2] = trash;
      trash = minute1 % 10;
      DispMSG[3] = trash;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 57;
        h += x;
      }
      break;
    //минута х2
    case 57:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 6 || x <= -1) {
        x = 0;
      }
      trash = h / 10;
      DispMSG[0] = trash;
      trash = h % 10;
      DispMSG[1] = trash;
      trash = x;
      DispMSG[2] = trash;
      trash = minute1 % 10;
      DispMSG[3] = trash;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 58;
        minn = x * 10;
      }
      break;
    //минута 2x
    case 58:
      t = analogRead(SIG);
      if ( t < 400 and t >100) {
        delay(del);
        x++;
        DispMSG[0] =  x;
      } if (t > 900) {
        delay(del);
        x--;
      }
      if (x >= 10 || x <= -1) {
        x = 0;
      }
      trash = h / 10;
      DispMSG[0] = trash;
      trash = h % 10;
      DispMSG[1] = trash;
      trash = minn / 10;
      DispMSG[2] = trash;
      DispMSG[3] = x;
      tm1637.display(DispMSG);
      if (t > 600 and t < 800) {
        delay(del);
        sw = 59;
        minn += x;
      }
      break;
    case 59:
      Serial.println(count);
      para[count] = h * 60 + minn;
      if (count == 6) {
        count = 0;
        sw = 1;
      }
      else {
        count++;
        sw = 55;
      }
      break;
  }
}
