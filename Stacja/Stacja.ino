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

int s_A0 = A0; // Inicjalizacja sensora

int button1;
int button2;
int button3;
int stan0;
int stan1;
int stan2;
int stan3;
int prze = 0;
int i = 0;
int k = 0;
int h = 0;
int mi = 0;
int y = 0;
int mo = 1;
int d = 1;
String godz1;
int godz2;

int w_A0;     // Inicjalizacja opadĂłw
int z = 5;    // Inicjalizacja licznika
String opa;   // String opady
String godz;  // Inicjalizacja godzina()
String ret;   // Inicjalizacja dane()


void setup()
{
  lcd.begin(16, 2);     // Inicjalizacja lcd
  Wire.begin();         // Inicjalizacja zczytywania
  clock.begin();        // Inicjalizacja zegara
  pinMode(8, INPUT);    // Pin 8 jako wejĹ›cie
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  ps.enableDefault();
  clock.setDateTime(2018, 5, 15, 12, 0, 0);
}

void loop()
{
  button1 = digitalRead(7);
  button2 = digitalRead(9);
  button3 = digitalRead(6);
  dt1 = clock.getDateTime();
  godz1 = (String)(clock.dateFormat("s", dt1));
  godz2= atoi(godz1.c_str());
  if(button1 == LOW){stan3 = 1;}
  if(button1 == HIGH && stan3 == 1){stan3=0;setTime();}
  if(godz2%9==0){
    poka();
  }
  if(button2 == LOW){stan1 = 1;}
  if(button2 == HIGH && stan1 == 1){stan1=0;z+=1;poka();}
  if(button3 == LOW){stan2 = 1;}
  if(button3 == HIGH && stan2 == 1){stan2=0;z-=1;poka();}
}

String god(){
  // String Godzina

  dt = clock.getDateTime();
  godz = ((String)clock.dateFormat("H:i   d.m.y", dt));
  return godz;
  
}

String dane(){
  //String Dane
  if(z> 5){z=1;}
  if(z<1){z=5;}
  if(z==5){ // Zczytuje temperaturÄ™
    int temp = ps.readTemperatureC()-2;
    String temperatura = "Temp: " + (String)temp + "^C";
    ret = temperatura;
  }
  if(z==1){ // Zczytuje wilgotnoĹ›Ä‡
    DHT.read(pin8);
    int wil = (int)DHT.humidity;
    String wilgotnosc = "Wilgotnosc: " + (String)wil + "%";
    ret = wilgotnosc;
  }
  if(z==2){ // Zczytuje ciĹ›nienie
    int cis = ps.readPressureMillibars();
    String cisnienie = "Cisnienie:" + (String)cis + "hPa";
    ret = cisnienie;
  }
  if(z==3){ // Zczytuje wysokoĹ›Ä‡
    int cis = ps.readPressureMillibars();
    int wys = ps.pressureToAltitudeMeters(cis);
    String wysokosc = "Wysokosc: " + (String)wys + "m";
    ret = wysokosc;
  }
  if(z==4){ // Sprawdza czy pada
    w_A0 = analogRead(s_A0);
    if(w_A0<=500){opa = "Pada";}
    else{opa = "Nie Pada";}
    ret = opa;
  }
  // Zwraca Stringa z danymi
  return ret;
}

void setTime(){
  stan0 = 1;
  stan1 = 1;
  lcd.clear();
  lcd.print("SET aby ustawic");
  lcd.setCursor(0, 1);
  lcd.print("date i godzine.");
  while(stan0 == 1){
    button1 = digitalRead(7);
    button2 = digitalRead(9);
    button3 = digitalRead(6);
    
    if(button1 == HIGH&&stan0==1){stan1 = 1;}
    
    if(button1 == LOW && stan1 == 1){
      int tab[] = {0,3,14,9,6};
    stan1=0;
    i = i%6+1;
    k++;
    if(i<6){
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(tab[i-1], 1);
      lcd.print(widok_d());
    }
    if(k>5){stan0=0;}
    delay(600);
    }
    if(button2 == LOW && i == 1){
      
      h++;
      if(h>23){h=0;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(0, 1);
      lcd.print("^G");
      delay(600);
    }
    if(button2 == LOW && i == 2){
      
      mi++;
      if(mi>59){mi=0;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(3, 1);
      lcd.print("^M");
      delay(600);
    }
    if(button2 == LOW && i == 3){
      
      y = y%99+1;
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(14, 1);
      lcd.print("^R");
      delay(600);
    }
    if(button2 == LOW && i == 4){
      
      mo = mo%12+1;
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(9, 1);
      lcd.print("^M");
      delay(600);
    }
    if(mo==1 || mo==3 || mo==5 || mo==7 || mo==8 || mo==10 || mo==12){
      if(button2 == LOW && i == 5){
      
      d = d%31+1;
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==4 || mo==6 || mo==9 || mo==11){
      if(button2 == LOW && i == 5){
      
      d = d%30+1;
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==2){
      if(y%4==0){prze=29;}
      else{prze=28;}
      if(button2 == LOW && i == 5){
      
      d = d%prze+1;
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(button3 == LOW && i == 1){
      
      h--;
      if(h<0){h=23;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(0, 1);
      lcd.print("^G");
      delay(600);
    }
    if(button3 == LOW && i == 2){
      
      mi--;
      if(mi<0){mi=59;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(3, 1);
      lcd.print("^M");
      delay(600);
    }
    if(button3 == LOW && i == 3){
      
      y --;
      if(y<0){y=99;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(14, 1);
      lcd.print("^R");
      delay(600);
    }
    if(button3 == LOW && i == 4){
      
      mo--;
      if(mo<1){mo=12;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(9, 1);
      lcd.print("^M");
      delay(600);
    }
    if(mo==1 || mo==3 || mo==5 || mo==7 || mo==8 || mo==10 || mo==12){
      if(button3 == LOW && i == 5){
      
      d--;
      if(d<1){d=31;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==4 || mo==6 || mo==9 || mo==11){
      if(button3 == LOW && i == 5){
      
      d--;
      if(d<1){d=30;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
    if(mo==2){
      if(y%4==0){prze=29;}
      else{prze=28;}
      if(button3 == LOW && i == 5){
      
      d--;
      if(d<1 && y%4 == 0){d=29;}
      if(d<1 && y%4 != 0){d=28;}
      lcd.clear();
      lcd.print(widok());
      lcd.setCursor(7, 1);
      lcd.print("^D");
      delay(600);
    }
    }
  }
  clock.setDateTime(2000+y, mo, d, h, mi, 0);
}

String widok(){
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

 String widok_d(){
  String w2 = "^G";
  if(i==2){return w2 = "^M";}
  if(i==3){return w2 = "^R";}
  if(i==4){return w2 = "^M";}
  if(i==5){return w2 = "^D";}
  return w2;
 }

 void poka(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(god());
  lcd.setCursor(0, 1);
  lcd.print(dane());
  delay(600);
 }


