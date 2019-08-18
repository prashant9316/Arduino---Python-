#include<Wire.h>
#include<Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
float cal,a;
char mode;
char operation;
int pin_number;
int digital_value;
int analog_write;
int value_to_write;
int wait_for_transmission = 5;
int DOOR = 0,flag = 0;
const int constant = 40;

void set_pin_mode(int pin_number, char mode){
    /*
     * Performs a pinMode() operation depending on the value of the parameter
     * mode :
     * - I: Sets the mode to INPUT
     * - O: Sets the mode to OUTPUT
     * - P: Sets the mode to INPUT_PULLUP
     */

    switch (mode){
        case 'I':
            pinMode(pin_number, INPUT);
            break;
        case 'O':
            pinMode(pin_number, OUTPUT);
            break;
        case 'P':
            pinMode(pin_number, INPUT_PULLUP);
            break;
    }
}

void digital_read(int pin_number){
    /*
     * Performs a digital read on pin_number and returns the value read to serial
     * in this format: D{pin_number}:{value}\n where value can be 0 or 1
     */

    digital_value = digitalRead(pin_number);
    Serial.print('D');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(digital_value); // Adds a trailing \n
}

void analog_read(int pin_number){
    /*
     * Performs an analog read on pin_number and returns the value read to serial
     * in this format: A{pin_number}:{value}\n where value ranges from 0 to 1023
     */
  
    int analog_value = analogRead(pin_number);
    Serial.print('A');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(analog_value); // Adds a trailing \n
}


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  pinMode(13,OUTPUT);
}

void loop()
{
  if(Serial.available() > 0) {
    operation = Serial.read();
    delay(wait_for_transmission);
    mode = Serial.read();
    pin_number = Serial.parseInt();
    if(Serial.read() == ':') value_to_write = Serial.parseInt();

  switch(operation){
      case 'R':
            if(mode == 'D') digital_read(pin_number);
            else if (mode == 'A') analog_read(pin_number);
            else if(mode == 'B') init_BMP();
            else if(mode == 'C') knock();
            else break;
      case 'W':
            if (mode == 'D') digitalWrite(pin_number, value_to_write);
            else if (mode == 'A') analogWrite(pin_number, value_to_write);
            else break;
     case 'M':
            set_pin_mode(pin_number, mode);
            break;
     default:
            break;
        }
  }
  /*a = bmp.readPressure() - cal - constant;
  float t; ;
  if(a>11)
   {  Serial.println("Single knock detected!!");
      if(flag == 0)
        {flag = 1;
         Serial.println(flag);
         t = millis();
         }
       if((flag == 1)&&((millis()-t)<2000)&&((millis()-t)>100))
         {flag = 0;
           Serial.println(flag);
           open_the_door();}
       if((millis() - t)>2000&&(flag==1))
          {flag = 0;
           Serial.println(flag);}
   }
 */ 
}

void callibrate()
{
  for(int i =0;i<100;i++)
   {
    if(i%2 == 0)
      digitalWrite(13,!digitalRead(13));
    cal += bmp.readPressure()/100;
   }
  }


void open_the_door() {
  digitalWrite(DOOR,HIGH);
  delay(5000);
  digitalWrite(DOOR,LOW);
}

void knock() {
  float t = millis();
  while((millis() - t)<5000) {
    if((bmp.readPressure() - cal - constant)>11)
      {
        open_the_door();
             Serial.print('A');
             Serial.print(7);
             Serial.print(':');
             Serial.println(14);
             return ;
        }
  }
             Serial.print('A');
             Serial.print(7);
             Serial.print(':');
             Serial.println(0);
}

void init_BMP() {
callibrate();
delay(1000);
  }

