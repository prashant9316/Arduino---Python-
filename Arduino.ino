const int knockSensor = A0;
const int threshold = 100;
const int relayPin = 8;
int sensorReading;


char mode;
char operation;
int pin_number;
int digital_value;
int analog_write;
int value_to_write;
int wait_for_transmission = 5;


void set_pin_mode(int pin_number, char mode){

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

  sensorReading = analogRead(knockSensor);
}


void open_the_door(int t) {
  digitalWrite(relayPin,HIGH);
  delay(t*1000);
  digitalWrite(relayPin,LOW);
  delay(t*1000);
  return 0;
}

void knock() {
  unsigned long t,t2;
  t = millis();
  while((unsigned long)(millis() - t) < 5000) {
      sensorReading = analogRead(knockSensor);
      if(sensorReading >= threshold) {
          t2 = millis();
          while((unsigned long)(millis() - t2) < 2000){
              sensorReading = analogRead(knockSensor);
              if(sensorReading >= threshold)
                  open_the_door(5);
          }
       }
   }
   return 0;
}


