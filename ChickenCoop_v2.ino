const int btn_down = 5 ;
const int btn_up = 6 ;
const int btn_automan = 7 ;     
const int led =  8;      
const int motorIN1 = 10;  //sainsmart dual H bridge
const int motorIN2 = 9;
const int photopin = A0;  //100mW, 150VPK, 3Kohm with a 10K pulldown resistor

int BS_down = 0;  //button state for "down"
int BS_up = 0;  //button state for "up"    
int BS_automan = 0;  //button state for "auto/manual" 
int photovalue = 0; //storage for photocell value 
int photovalueMap = 0;

void setup() {
  Serial.begin(9600);  //for debugging, comment out before install
  pinMode(motorIN1,OUTPUT);
  pinMode(motorIN2,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(btn_down,INPUT);
  pinMode(btn_up,INPUT);
  pinMode(btn_automan,INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  BS_automan = digitalRead(btn_automan);

  if (BS_automan == HIGH) {
    // run in MANUAL mode:
    BS_down = digitalRead(btn_down);
    BS_up = digitalRead(btn_up);
    if (BS_down == HIGH) {
      digitalWrite(led, HIGH);
      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
    }
    else if (BS_up == HIGH) {
      digitalWrite(led, HIGH);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, HIGH);
    }
    else {
      digitalWrite(led, LOW);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, LOW);
    }
    
  } else {
    // run in AUTO mode:
    digitalWrite(led, LOW);
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, LOW);
    photovalue = analogRead(photopin);
    photovalueMap = map(photovalue,600,1024,0,1024);
    Serial.println(photovalue);
    Serial.print(",");
    Serial.print(photovalueMap);
    if (photovalueMap < 500) {
      digitalWrite(led, HIGH);
      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
      delay(45*1000);
      digitalWrite(motorIN1, LOW);
      digitalWrite(led, LOW);
    }
    else {
      digitalWrite(led, HIGH);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, HIGH);
      delay(45*1000);
      digitalWrite(motorIN2, LOW);
      digitalWrite(led, LOW);
    }
  }
}
