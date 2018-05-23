/*###################### DEFINICIONES #################################*/

/*###### HC-SR04 ######*/
#include <NewPing.h>

#define MAX_SENSORES    1 // Numero de sensores
#define PING_INTERVAL 33
#define PING_CORRIENTE 10
unsigned long pingTimer[MAX_SENSORES];
unsigned int cm[MAX_SENSORES];
uint8_t currentSensor = 0;

NewPing sonar[MAX_SENSORES] = {   //Array
  NewPing(11, 12, 100) //Trigger, Echo, Distancia maxima.
};

/*###### Bluetooth ######*/
char aux = 0;
String state = "";
boolean newline = false;
int fast = 150;
int mode = 0;
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

  /*###### HC-SR04 ######*/
  pinMode(PING_CORRIENTE, OUTPUT);
  digitalWrite(PING_CORRIENTE, HIGH);
  pingTimer[0] = millis() + 75;
  for (uint8_t i = 1; i < MAX_SENSORES; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  forward();
}

/*######################### LOOP ##############################*/

void loop() { /*main*/

  /*=== BLUETOOTH RECIEVE SERIAL DATA ===*/
  state = "";
  while (Serial.available() > 0) { // Checks whether data is comming from the serial port
    aux = Serial.read(); // Reads the data from the serial port

    if (aux == '\r') {
      continue;
    }
    else if (aux == '\n') {
      newline = true;
      break;
    }
    else {
      state = state + aux;
    }

  }

  Serial.print(state);
  if (newline) {
    Serial.print(state);
  }

  /*=== BLUETOOTH MODE CONTROL ===*/
  if (state == '0') {
    mode = 0;
  }
  else if (state == '1') {
    mode = 1;
  }

  if (mode == 1) {
    /*###### HC-SR04 ######*/
    for (uint8_t i = 0; i < MAX_SENSORES; i++)
    {
      if (millis() >= pingTimer[i])
      {
        pingTimer[i] += PING_INTERVAL * MAX_SENSORES;
        if (i == 0 && currentSensor == MAX_SENSORES - 1) oneSensorCycle();
        sonar[currentSensor].timer_stop();
        currentSensor = i;
        cm[currentSensor] = 0;
        sonar[currentSensor].ping_timer(echoCheck);
      }
    }
  }

  if (mode == 0) { //bluetooth mode
    /*=== BLUETOOTH SWITCH CASE ===*/
    if (state == "A") {
      Serial.println("ALANTE");
      forward();
    } else if (state == "B") {
      Serial.println("IZQUIERDA");
      turnLeft();
    } else if (state == "C") {
      Serial.println("DERECHA");
      turnRight();
    } else if (state == "D") {
      Serial.println("ATRAS");
      goback();
    } else if (state == "AB" || state == "BA") {
      Serial.println("ALANTE-IZQUIERDA");
      forwardLeft();
    } else if (state == "AC" || state == "CA") {
      Serial.println("ALANTE-DERECHA");
      forwardRight();
    } else if (state == "DB" || state == "BD") {
      Serial.println("ATRAS-IZQUIERDA");
      gobackLeft();
    } else if (state == "DC" || state == "CD") {
      Serial.println("ATRAS-DERECHA");
      gobackRight();
    }
  }

}//end loop

/*######################## FUNCTIONS ###############################*/

/*###### HC-SR04 ######*/
void echoCheck() 
{
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}
//Resultado sensores
void oneSensorCycle() { 
  for (uint8_t i = 0; i < MAX_SENSORES; i++) 
  {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
    if(cm[i] <= 10 && cm[i] > 0) 
    {
      goback();
      Serial.println("TECLADOOOOO!!");
    }
    if(cm[i] == 0) forward();
  }
  Serial.println();
}

/*##### orders #######*/
void forward() {
  goA();
  goB();
}

void forwardLeft() {
  goA();
  goHalfB();
}

void forwardRight() {
  goHalfA();
  goB();
}

void goback() {
  backA();
  backB();
}

void gobackLeft() {
  backA();
  backHalfB();
}

void gobackRight() {
  backHalfA();
  backB();
}

void turnLeft() {
  goB();
}

void turnRight() {
  goA();
}

void staphPlz() {
  stopA();
  stopB();
}


/*##### basic functions #####*/

void goA() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, fast);
}

void goHalfA() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, fast / 2);
}

void goB() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, fast);
}

void goHalfB() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, fast / 2);
}

void backA() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, fast);
}

void backHalfA() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, fast / 2);
}

void backB() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, fast);
}

void backHalfB() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, fast / 2);
}

void stopA() {
  analogWrite(ENA, LOW);// Motor A desactivado
}

void stopB() {
  analogWrite(ENB, LOW);// Motor B desactivado
}
