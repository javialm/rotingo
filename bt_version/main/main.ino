/*###################### DEFINICIONES #################################*/
#include <LEANTEC_ControlMotor.h>//Incluimos la librería control de motores 

// Configuramos los pines que vamos a usar

  ControlMotor control(2,3,7,4,5,6); // MotorDer1,MotorDer2,MotorIzq1,MotorIzq2,PWM_Derecho,PWM_Izquierdo

/*######################## SETUP ###############################*/

void setup() {

  Serial.begin(9600); // Default communication rate of the Bluetooth module
}

/*######################### LOOP ##############################*/

void loop() { /*main*/
  control.Motor(255,1);
  delay(3000);
  control.Motor(0,1);
  delay(3000);
}//end loop
