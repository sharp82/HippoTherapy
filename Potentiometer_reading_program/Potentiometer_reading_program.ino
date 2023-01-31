
//MILO NEW STUFF
int pot_out1 = A1;      // Analog,IN, Potentiometer For Motor 1 read pin
int pot_out2 = A0;      // Analog,IN, Potentiometer For Motor 2 read pin
int pot_out3 = A3;      // Analog,IN, Potentiometer For Motor 3 read pin
float rangeOfLinearMotor = 50.0; //50 mm
int MotorLocation1 = 0;
int MotorLocation2 = 0;
int MotorLocation3 = 0;
int GoTo1 = 25;
int GoTo2 = 25;
int GoTo3 = 0;
float GoToTest = 0.0;






//OLD HIPPOTHERAPY CODE
int en_1 = 9;          //Duty Cycle Out Pin for Motor 1[PWM signal] -> roll
int en_2 = 10;         //Duty Cycle Out Pin for Motor 2[PWM signal] -> pitch
int en_3 = 11;         //Duty Cycle Out Pin for Motor 3[PWM Signal] -> yaw
int in_1_1 = 2;        // Direction Control Out 1 for Motor1 -> roll
int in_1_2 = 3;        // Direction Control Out 2 for Motor1
int in_2_1 = 4;        // Direction Control Out 1 for Motor2
int in_2_2 = 5;        // Direction Control Out 2 for Motor2
int in_3_1 = 6;        // Direction Control Out 1 for Motor3
int in_3_2 = 7;        // Direction Control Out 2 for Motor3
int dutyCycle = 255;    // Since voltage is 12V, considering 100% duty cycle
int g_positionValue1 = 0;  // Initialization of Motor 1 to 0(position tracker)
int g_positionValue2 = 0;  // Initialization of Motor 2 to 0(position tracker)
int g_positionValue3 = 0;  // Initialization of Motor 3 to 0(position tracker)
//int pot2_startReading = 0; // Motor2 Potentiometer Start Reading
//int pot2_endReading = 0;   // Motor2 Potentiometer End Reading
float maxSpeedOfLinearMotor = 150.0; //150mm/sec
float rangeScaleAsPerGraph = 12.0; //12 degrees
int   potTolerance = 2;

float pitchMotions[25] = { 9.025546063, 14.04062945, 15.84375291, 15.74362591, 15.04257653, 12.0354366, 8.021994481, 4.00855236, 1.802930987, 0.200236054, 2.003359513, 8.021994481, 12.0354366, 14.44146755, 15.04257653, 15.64349303, 12.43659253, 8.021994481, 2.003359513, 1.502323911, 1.001412434,0,0.400495935,4.00855236,8.222707243 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pot_out2, INPUT); //setting the analog pin mode for the potentiometer as input
  pinMode(pot_out1, INPUT); //setting the analog pin mode for the potentiometer as input
  pinMode(pot_out3, INPUT); //setting the analog pin mode for the potentiometer as input

  pinMode(in_1_1, OUTPUT); //setting the pin of 1_1 as output for motor 1[input pin for motor 1]
  pinMode(in_1_2, OUTPUT); //setting the pin of 1_2 as output for motor 1[input pin for motor 1]
  pinMode(en_1, OUTPUT);   //setting the pin of the enable pin as output for motor 1

  pinMode(in_2_1, OUTPUT); //setting the pin of 2_1 as output for motor 2[input pin for motor 2]
  pinMode(in_2_2, OUTPUT); //setting the pin of 2_2 as output for motor 2[2nd input pin for motor 2]
  pinMode(en_2, OUTPUT);   //setting the pin mode of the enable pin as output for motor 2

  pinMode(in_3_1, OUTPUT); //setting the pin of 3_1 as output for motor 3[input pin for motor 3]
  pinMode(in_3_2, OUTPUT); //setting the pin of 3_2 as output for motor 3[2nd input pin for motor 3]
  pinMode(en_3, OUTPUT);   // setting the pin mode of the enable pin as output for motor 3


}

void loop() {
  // put your main code here, to run repeatedly:
  int pot1_startReading = analogRead(pot_out1); //reads the potentiometer start value for motor 1 as Analog
  int pot2_startReading = analogRead(pot_out2); //reads the potentiometer start value for motor 2 as Analog
  int pot3_startReading = analogRead(pot_out3); //reads the potentiometer start value for motor 3 as Analog
  MotorLocation1 = rangeOfLinearMotor / 1023 * pot1_startReading;
  MotorLocation2 = rangeOfLinearMotor / 1023 * pot2_startReading;
  MotorLocation3 = rangeOfLinearMotor / 1023 * pot3_startReading;
for (int i = 0; i < 25 ; i++) {
  GoToTest = pitchMotions[i];

  while (GoToTest < MotorLocation1) {
    analogWrite(en_2, dutyCycle);
    digitalWrite(in_2_1, LOW); // sets the 1st input of the motor 1 to LOW
    digitalWrite(in_2_2, HIGH); //sets the 2nd input of the motor 1 to HIGH
    delay(3);
  }

  while (GoToTest > MotorLocation1) {
    analogWrite(en_2, dutyCycle);
    digitalWrite(in_2_1, HIGH); // sets the 1st input of the motor 1 to HIGH
    digitalWrite(in_2_2, LOW); //sets the 2nd input of the motor 1 to LOW

    delay(3);
  }
  while (GoToTest = MotorLocation1) {
    delay(5);
  }
}

  /*
    Serial.println("pot 1");
      Serial.println(pot1_startReading);
      Serial.println("pot 2");
      Serial.println(pot2_startReading);
      Serial.println("pot 3");
      Serial.println(pot3_startReading);
      Serial.println("");

      MotorLocation1 = rangeOfLinearMotor/1023*pot1_startReading;
      MotorLocation2 = rangeOfLinearMotor/1023*pot2_startReading;
      MotorLocation3 = rangeOfLinearMotor/1023*pot3_startReading;
      Serial.println("Motor 1 location in MM");
      Serial.println(MotorLocation1);
        Serial.println("Motor 2 location in MM");
      Serial.println(MotorLocation2);
        Serial.println("Motor 3 location in MM");
      Serial.println(MotorLocation3);
        Serial.println("");

       delay(10000);
  */


  /*Serial.flush();
    Serial.println("\n\nPlease choose one of the following"); //Prints this to the user
    Serial.println("1 : Execute Cam on all Motors"); //First option to run a motion profile
    Serial.println("2 : Move Selected Motor to Position"); //2nd option to pick a particular motor
    Serial.println("3 : Reset all Motors to Min Positions\n"); // 3rd option to reset all motors to complete retracted stroke
    while (Serial.available() == 0) {}
    int CamOrPosChoice = Serial.parseInt(); //reads the user's input from the Serial and puts it into the variable
  */


}
