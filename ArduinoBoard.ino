const int pinRelay = 2;
const int pinButton = 4;
int incomingByte = 0;
const int pinTrigger = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinRelay, OUTPUT);
  pinMode(pinButton, OUTPUT);
  digitalWrite(pinRelay, LOW);
  digitalWrite(pinButton, LOW);
  pinMode(pinTrigger, OUTPUT);
  digitalWrite(pinTrigger, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'F') { // didn't scanned
      digitalWrite(pinButton, LOW);
      delay(1);
    }
    else if (incomingByte == 'p'){ //scanned but worng part
      digitalWrite(pinButton, LOW);
      digitalWrite(pinRelay, HIGH);
      delay(1);
    }
    else if (incomingByte == 'P'){ // right part
      digitalWrite(pinButton, HIGH);
      digitalWrite(pinRelay, LOW);
      delay(1);
    }
    else if (incomingByte == 'T'){ // trigger camera
      digitalWrite(pinTrigger, HIGH);
      delay(1);
    }
    else if (incomingByte == 'S'){ // stop camera
      digitalWrite(pinTrigger, LOW);
      delay(1);
    }
  }
}
