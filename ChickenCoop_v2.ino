const int btn_down = 5 ;
const int btn_up = 6 ;
const int btn_automan = 7 ;     
const int led =  8;      
const int motorIN1 = 10;  //sainsmart dual H bridge
const int motorIN2 = 9;
const int photopin = A0;  //100mW, 150VPK, 3Kohm with a 10K pulldown resistor

boolean isdooropen = false;

int BS_down = 0;  //button state for "down"
int BS_up = 0;  //button state for "up"    
int BS_automan = 0;  //button state for "auto/manual" 
int photovalue = 0; //storage for photocell value 
int photovalueMap = 0;
int blueknobval = 0;
int goldknobval = 0;

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
  blueknobval = analogRead(A2);  //blue knob controls delay
  goldknobval = analogRead(A1);  //gold knob controls sensitivity
  int bluemap = map(blueknobval, 0, 1024, 45, 0);
  int goldmap = map(goldknobval, 0, 1024, 1024, 0);

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
    photovalueMap = map(photovalue,400,1024,0,1024);
    Serial.print("Photo sensor value is ");
    Serial.print(photovalue);
    Serial.print(", Mapped value is ");
    Serial.print(photovalueMap);
    Serial.print(", sensitivity is ");
    Serial.print(goldmap);
    Serial.print(", delay is ");
    Serial.println(bluemap);
    if (photovalueMap < goldmap && isdooropen == true) {
      Serial.println("Beginning closing process!");
      for (int timer = 1; timer <= bluemap; timer++){
        for (int countdown = 0; countdown < 58; countdown++){
          delay(1000);
          Serial.print(".");
        }
        Serial.print(timer);
        Serial.println(" minutes have passed");
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
      }
      digitalWrite(led, HIGH);
      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
      for (int timer = 1; timer <= 45; timer++){
        delay(1000);
        Serial.print(".");
      }
      digitalWrite(motorIN1, LOW);
      digitalWrite(led, LOW);
      isdooropen = false;
    }
    else if (photovalueMap > goldmap && isdooropen == false){
      Serial.println("Opening the door!");
      digitalWrite(led, HIGH);
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, HIGH);
      for (int timer = 1; timer <= 45; timer++){
        delay(1000);
        Serial.print(".");
      }
      digitalWrite(motorIN2, LOW);
      digitalWrite(led, LOW);
      isdooropen = true;
    }
  }
  delay(1000);
}
