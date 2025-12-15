#include <Wire.h>
#include <LPS331.h>
#include <dht11.h>
#include <LiquidCrystal.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;
RTCDateTime dt1;
dht11 DHT;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, pin8 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LPS331 ps;

int s_A0 = A0;
int button_1, button_2, button_3;
int state_0, state_1, state_2, state_3;
int skip = 0;
int h = 0, mi = 0, y = 0, mo = 1, d = 1;
int hour_2;
int w_A0;
int z = 5, i = 0, k = 0;
String rain, hour, hour_1, ret;


void setup(){
  
  lcd.begin(16, 2);
  Wire.begin();
  clock.begin();
  pinMode(8, INPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  ps.enableDefault();
  clock.setDateTime(2018, 5, 15, 12, 0, 0);
}

void loop(){
  
  button_1 = digitalRead(7);
  button_2 = digitalRead(9);
  button_3 = digitalRead(6);
  dt1 = clock.getDateTime();
  hour_1 = (String)(clock.dateFormat("s", dt1));
  hour_2= atoi(hour_1.c_str());
  if(button_1 == LOW){state_3 = 1;}
  if(button_1 == HIGH && state_3 == 1){state_3=0;setTime();}
  if(hour_2%9==0){
    show();
  }
  if(button_2 == LOW){state_1 = 1;}
  if(button_2 == HIGH && state_1 == 1){state_1=0;z+=1;show();}
  if(button_3 == LOW){state_2 = 1;}
  if(button_3 == HIGH && state_2 == 1){state_2=0;z-=1;show();}
}

String getHour(){

  dt = clock.getDateTime();
  hour = ((String)clock.dateFormat("H:i   d.m.y", dt));
  return hour;
  
}

String getData(){
  
  if(z> 5){z=1;}
  if(z<1){z=5;}
    int temp = ps.readTemperatureC()-2;
    String temperature = "Temp: " + (String)temp + "^C";
    ret = temperature;
  
  if(z==1){
    DHT.read(pin8);
    int wil = (int)DHT.humidity;
    String humidity = "Humidity: " + (String)wil + "%";
    ret = humidity;
  }
  if(z==2){
    int cis = ps.readPressureMillibars();
    String pressure = "Pressure:" + (String)cis + "hPa";
    ret = pressure;
  }
  if(z==3){
    int cis = ps.readPressureMillibars();
    int wys = ps.pressureToAltitudeMeters(cis);
    String high = "High: " + (String)wys + "m";
    ret = high;
  }
  if(z==4){
    w_A0 = analogRead(s_A0);
    if(w_A0<=500){rain = "Rain";}
    else{rain = "No Rain";}
    ret = rain;
  }
  return ret;
}

void setTime(){
  
  state_0 = 1;
  state_1 = 1;
  lcd.clear();
  lcd.print("SET for set");
  lcd.setCursor(0, 1);
  lcd.print("data & time.");
  while(state_0 == 1){
    button_1 = digitalRead(7);
    button_2 = digitalRead(9);
    button_3 = digitalRead(6);
    if(button_1 == HIGH&&state_0==1){state_1 = 1;}
    if(button_1 == LOW && state_1 == 1){
      int tab[] = {0,3,14,9,6};
      state_1=0;
      i = i%6+1;
      k++;
      if(i<6){
        lcd.clear();
        lcd.print(view());
        lcd.setCursor(tab[i-1], 1);
        lcd.print(view_d());
      }
      if(k>5){state_0=0;}
      delay(600);
    }
    if(button_2 == LOW && i == 1){
      h++;
      if(h>23){h=0;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(0, 1);
      lcd.print("^H");
      delay(600);
    }
    if(button_2 == LOW && i == 2){
      mi++;
      if(mi>59){mi=0;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(3, 1);
      lcd.print("^M");
      delay(600);
    }
    if(button_2 == LOW && i == 3){
      y = y%99+1;
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(14, 1);
      lcd.print("^Y");
      delay(600);
    }
    if(button_2 == LOW && i == 4){
      mo = mo%12+1;
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(9, 1);
      lcd.print("^M");
      delay(600);
    }
    if(mo==1 || mo==3 || mo==5 || mo==7 || mo==8 || mo==10 || mo==12){
      if(button_2 == LOW && i == 5){
      d = d%31+1;
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==4 || mo==6 || mo==9 || mo==11){
      if(button_2 == LOW && i == 5){
      d = d%30+1;
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==2){
      if(y%4==0){skip=29;}
      else{skip=28;}
      if(button_2 == LOW && i == 5){
      d = d%skip+1;
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(button_3 == LOW && i == 1){
      h--;
      if(h<0){h=23;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(0, 1);
      lcd.print("^H");
      delay(600);
    }
    if(button_3 == LOW && i == 2){
      mi--;
      if(mi<0){mi=59;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(3, 1);
      lcd.print("^M");
      delay(600);
    }
    if(button_3 == LOW && i == 3){
      y --;
      if(y<0){y=99;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(14, 1);
      lcd.print("^Y");
      delay(600);
    }
    if(button_3 == LOW && i == 4){
      mo--;
      if(mo<1){mo=12;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(9, 1);
      lcd.print("^M");
      delay(600);
    }
    if(mo==1 || mo==3 || mo==5 || mo==7 || mo==8 || mo==10 || mo==12){
      if(button_3 == LOW && i == 5){
      d--;
      if(d<1){d=31;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==4 || mo==6 || mo==9 || mo==11){
      if(button_3 == LOW && i == 5){
      d--;
      if(d<1){d=30;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==2){
      if(y%4==0){skip=29;}
      else{skip=28;}
      if(button_3 == LOW && i == 5){
      d--;
      if(d<1 && y%4 == 0){d=29;}
      if(d<1 && y%4 != 0){d=28;}
      lcd.clear();
      lcd.print(view());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
  }
  clock.setDateTime(2000+y, mo, d, h, mi, 0);
}

String view(){
  
  String hh = (String)h;
  String mii = (String)mi;
  String dd = (String)d;
  String moo = (String)mo;
  String hhh = "0" + (String)h;
  String miii = "0" + (String)mi;
  String mooo = "0" + (String)mo;
  String ddd = "0" + (String)d;
  if(h>= 0 && h<10){hh=hhh;}
  if(mi>= 0 && mi<10){mii=miii;}
  if(d>= 0 && d<10){dd=ddd;}
  if(mo>= 0 && mo<10){moo=mooo;}
  String yy = (String)(2000+y);
  String w = hh + ":" + mii + " " + dd + "." + moo + "." + yy;
  return w;
}

 String view_d(){
  
  String w2 = "^H";
  if(i==2){return w2 = "^M";}
  if(i==3){return w2 = "^Y";}
  if(i==4){return w2 = "^M";}
  if(i==5){return w2 = "^D";}
  return w2;
 }

 void show(){
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(getHour());
  lcd.setCursor(0, 1);
  lcd.print(getData());
  delay(600);
 }
