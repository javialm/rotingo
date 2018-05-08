/*###################### DEFINICIONES #################################*/
/*###### Bluetooth ######*/

int state = 0;
int fast = 150;
/*###### Motores ######*/

// Motor A
const int ENA = 6; // Pin PWM
const int IN1 = 7;
const int IN2 = 4;
 
// Motor B
const int ENB = 5; // Pin PWM
const int IN3 = 2;
const int IN4 = 3;

/*######################## SETUP ###############################*/

void setup() {
    pinMode(13, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  //digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}

/*######################### LOOP ##############################*/

void loop() { /*main*/
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }

 /*if (state == '0') {
  digitalWrite(ledPin, LOW); // Turn LED OFF
  Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
  state = 0;
 }
 else if (state == '1') {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED: ON");;
  state = 0;
 } */
 
 if (state == 'X'){
  Serial.println("AAAAA A");
 }
 
 switch (state){
  case 'A':
    digitalWrite(13, LOW);
    Serial.println("A");
    staphPlz();
    state = 0;
    break;
  case 'B':
    forward();
   // state = 0;
    break;
  case 'C':
    goback();
    state = 0;
    break;
  case 'D':
    turnLeft();
    state = 0;
    break;
  case 'E':
    turnRight();
    state = 0;
    break;
 }
}

/*######################## FUNCTIONS ###############################*/

/*##### orders #######*/
void forward(){
  goA();
  goB();
}

void goback(){
  backA();
  backB();
}

void turnLeft(){
  goB();
}

void turnRight(){
  goA();
}

void staphPlz(){
  stopA();
  stopB();
}


/*##### basic functions #####*/

void goA(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, fast);
}

void goB(){
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, fast);
}

void backA(){
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, fast);
}

void backB(){
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, fast);
}

void stopA() {
    analogWrite(ENA, LOW);// Motor A desactivado
}
 
void stopB() {
    analogWrite(ENB, LOW);// Motor B desactivado
}
