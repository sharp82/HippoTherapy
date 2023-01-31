int en_1 = 9;          //Duty Cycle Out Pin for Motor 1[PWM signal] -> roll
int en_2 = 10;         //Duty Cycle Out Pin for Motor 2[PWM signal] -> pitch
int en_3 = 11;         //Duty Cycle Out Pin for Motor 3[PWM Signal] -> yaw
int in_1_1 = 2;        // Direction Control Out 1 for Motor1 -> roll
int in_1_2 = 3;        // Direction Control Out 2 for Motor1 
int in_2_1 = 4;        // Direction Control Out 1 for Motor2
int in_2_2 = 5;        // Direction Control Out 2 for Motor2
int in_3_1 = 6;        // Direction Control Out 1 for Motor3
int in_3_2 = 7;        // Direction Control Out 2 for Motor3
int pot_out2 = A0;      // Analog,IN, Potentiometer For Motor 2 read pin
int dutyCycle = 255;    // Since voltage is 12V, considering 100% duty cycle
int g_positionValue1 = 0;  // Initialization of Motor 1 to 0(position tracker)
int g_positionValue2 = 0;  // Initialization of Motor 2 to 0(position tracker)
int g_positionValue3 = 0;  // Initialization of Motor 3 to 0(position tracker)
int pot2_startReading = 0; // Motor2 Potentiometer Start Reading
int pot2_endReading = 0;   // Motor2 Potentiometer End Reading
float rangeOfLinearMotor = 50.0; //50 mm
float maxSpeedOfLinearMotor = 150.0; //150mm/sec
float rangeScaleAsPerGraph = 12.0; //12 degrees
int   potTolerance = 2;

//Motor 1 time Cam (Red in Graph) in degree
float Motor1Cam[21] = {6.375, 7.375, 8.375, 8.875, 9.375, 9.750, 9.875, 9.843, 9.812, 9.750, 9.500, 9.312, 8.625, 8.125, 7.250, 6.562, 5.500, 4.312, 3.687, 3.187, 2.750};
//Motor 2 Time Cam (Blue in Graph) in degree
float Motor2Cam[21] = {9.437, 9.312, 9.250, 9.125, 9.000, 8.625, 8.375, 8.125, 8.000, 7.000, 6.000, 4.875, 4.750, 4.875, 5.124, 5.500, 5.750, 5.937, 5.843, 5.562, 5.500};
//Motor 3 Time Cam (Yellow in Graph) in degree
float Motor3Cam[21] = {4.312, 4.343, 4.468, 4.500, 4.625, 4.875, 4.968, 5.000, 5.093, 5.125, 5.218, 5.375, 5.500, 5.750, 5.937, 6.437, 6.625, 7.187, 7.375, 7.625, 7.687};

////////////////////////////////////////////////////////////
// Setup
///////////////////////////////////////////////////////////
void setup() {

  pinMode(in_1_1, OUTPUT); //setting the pin of 1_1 as output for motor 1[input pin for motor 1]
  pinMode(in_1_2, OUTPUT); //setting the pin of 1_2 as output for motor 1[input pin for motor 1]
  pinMode(en_1, OUTPUT);   //setting the pin of the enable pin as output for motor 1

  pinMode(in_2_1, OUTPUT); //setting the pin of 2_1 as output for motor 2[input pin for motor 2]
  pinMode(in_2_2, OUTPUT); //setting the pin of 2_2 as output for motor 2[2nd input pin for motor 2]
  pinMode(en_2, OUTPUT);   //setting the pin mode of the enable pin as output for motor 2

  pinMode(in_3_1, OUTPUT); //setting the pin of 3_1 as output for motor 3[input pin for motor 3]
  pinMode(in_3_2, OUTPUT); //setting the pin of 3_2 as output for motor 3[2nd input pin for motor 3]
  pinMode(en_3, OUTPUT);   // setting the pin mode of the enable pin as output for motor 3

  pinMode(pot_out2, INPUT); //setting the analog pin mode for the potentiometer as input 
  MoveMotorToMaxOrMin(3);//Move to Max range of Motor 2
  pot2_startReading = analogRead(pot_out2); //reads the potentiometer start value for motor 2 as Analog 
  MoveMotorToMaxOrMin(4);//Move to Min Range of Motor 2
  pot2_endReading = analogRead(pot_out2); //reads the potentiometer end value for motor 2 as Analog
  MoveMotorToMaxOrMin(2);//Move to Min to Motor1
  MoveMotorToMaxOrMin(6);//Move to Min to Motor3
  g_positionValue1 = 0;  // Initialize Motor1 position
  g_positionValue2 = 0;  // Initialize Motor2 position
  g_positionValue3 = 0;  // Initialize Motor3 position
}
////////////////////////////////////////////////////////////
// Loop
///////////////////////////////////////////////////////////
void loop() {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("\n\nPlease choose one of the following"); //Prints this to the user
  Serial.println("1 : Execute Cam on all Motors"); //First option to run a motion profile
  Serial.println("2 : Move Selected Motor to Position"); //2nd option to pick a particular motor
  Serial.println("3 : Reset all Motors to Min Positions\n"); // 3rd option to reset all motors to complete retracted stroke

  Serial.println(pot2_startReading);
  while (Serial.available() == 0) {}
  int CamOrPosChoice = Serial.parseInt(); //reads the user's input from the Serial and puts it into the variable

  // clear serial buffer
  delay(5);
  while (Serial.available() > 0){ Serial.read(); }
  
  if(CamOrPosChoice   == 3) //Reset all Motors to Min Pos
  {
     MoveMotorToMaxOrMin(4);//Move to Min Motor2 
     MoveMotorToMaxOrMin(2);//Move to Min Motor1
     MoveMotorToMaxOrMin(6);//Move to Min Motor3
     g_positionValue1 = 0;  // Initialize Motor1 position
     g_positionValue2 = 0;  // Initialize Motor2 position
     g_positionValue3 = 0;  // Initialize Motor3 position
     Serial.println ("Done Moving all Motors to Min Extension"); 
  }
  else if(CamOrPosChoice   == 2) //Selected when user wants to move a particular motor to a certain position
  {  
     Serial.println("1 : Motor 1"); //Prints out the motor 1 option
     Serial.println("2 : Motor 2"); //Prints out the motor 2 option
     Serial.println("3 : Motor 3"); //Prints out the motor 3 option
     Serial.print("Which Motor do you want to move?"); //Prints out the question to the user

     // Get Motor Choice
     while (Serial.available() == 0) {}
     int motorChoice = Serial.parseInt(); //Saves the user's input for which motor they would like to move
     Serial.println(motorChoice); //Prints the user's motor inputted

     // clear serial buffer
     delay(5);
     while (Serial.available() > 0) 
     {
        Serial.read();
     }
     //Gets the position the user would like to move the particular motor to from the min to the max length of the stroke length
     Serial.print("What position to move(Range : 0-50mm) ?");
     while (Serial.available() == 0) {}
     int end_PositionValue = Serial.parseInt();

     //Makes sure it is within threshold limits of actuator
     if(end_PositionValue > rangeOfLinearMotor)
     {
        end_PositionValue = rangeOfLinearMotor; 
     }
     if(end_PositionValue <= 0)
     {
        end_PositionValue = 0;
     }
     Serial.println(end_PositionValue);

     switch (motorChoice)
     {
        case 1:  //Move motor 1 to position
            MoveMotor1ToPos(end_PositionValue);
            break;
        case 2:// move motor 2 to position
        {
            MoveMotor2ToPos(end_PositionValue);
            // Move using the Potentiometer feedback value
            float expectedPotValue = ((float(pot2_endReading-pot2_startReading))*((float)end_PositionValue))/rangeOfLinearMotor;
            expectedPotValue = (float)pot2_endReading - expectedPotValue;
            int readPotValue = analogRead(pot_out2); //Current potentiometer value for motor 2
            int diffPotReading = readPotValue - expectedPotValue; 
            int absPotDiff = abs(diffPotReading); 
            /*if((absPotDiff-potJitter) > 0) //Only if value greater than 0 after taking care of Jitter
            {
              if(diffPotReading > 0) //move UP
              {
                do
                {
                    analogWrite(en_2, dutyCycle);
                    digitalWrite(in_2_1, HIGH); //sets the digital pin of an input for motor 2 to HIGH
                    digitalWrite(in_2_2, LOW);  //sets the digital pin of an input for motor 2 to LOW
                    delay(1); //lets it run for 1msec
                    readPotValue = analogRead(pot_out2); 
                    Serial.println(readPotValue);
                    diffPotReading = readPotValue - expectedPotValue;                
                } while((diffPotReading-potJitter > 0)&&(readPotValue > pot2_startReading));
                analogWrite(en_2, 0);
                digitalWrite(in_2_1, LOW); //stops it from moving any further once it has run and moved
                digitalWrite(in_2_2, LOW); //stops second input from recieving any instruction and set it to Low
              }
              else // Move Down
              {
                do
                {
                    analogWrite(en_2, dutyCycle); //sends the enable(PWM pin) the duty cycle value specified
                    digitalWrite(in_2_1, LOW); // sets the 1st input of the motor 2 to LOW
                    digitalWrite(in_2_2, HIGH); //sets the 2nd input of the motor 2 to HIGH
                    delay(1);
                    readPotValue = analogRead(pot_out2); //reads the current potentiometer value
                    Serial.println(readPotValue);
                    diffPotReading = readPotValue - expectedPotValue; //calculates the difference between read and expected value               
                } while((diffPotReading-potJitter <0) && (readPotValue < pot2_endReading));
                analogWrite(en_2, 0);
                digitalWrite(in_2_1, LOW);
                digitalWrite(in_2_2, LOW);
              }
            }*/
        }
            break;
        case 3:  // move motor 3 
            MoveMotor3ToPos(end_PositionValue);
            break;
     }//End of Switch

     Serial.print ("Done Moving Motor "); 
     Serial.print(motorChoice); 
     Serial.print(" To Position: ");
     Serial.println(end_PositionValue);
  }
  else if(CamOrPosChoice   == 1) //execution if the user has chosen to run the Cam motion profile 
  //this part is correct we want to keep this
  {
     Serial.println("Executing Time CAM on all Motors");
     int end_PositionValue1 = 0; //sets the initial position value to 0 -Motor1
     int end_PositionValue2 = 0; //sets the initial position value to 0 -Motor2
     int end_PositionValue3 = 0; //sets the initial position value to 0 -Motor3
     int numOfElements = sizeof(Motor1Cam)/sizeof(Motor1Cam[0]); //calculates the number of elements for the motion profile
     for (int i = 0; i < numOfElements ; i++) //sets the # of times the for loop has to execute
     {
        end_PositionValue1 = (Motor1Cam[i]*rangeOfLinearMotor)/rangeScaleAsPerGraph; //Computes the end position of the motor1
        end_PositionValue2 = (Motor2Cam[i]*rangeOfLinearMotor)/rangeScaleAsPerGraph; //Computes the end position of an element of motor2 
        end_PositionValue3 = (Motor3Cam[i]*rangeOfLinearMotor)/rangeScaleAsPerGraph; //Computes the end position of an element of motor3
        MoveMotor1ToPos(end_PositionValue1); //Sends the computed end position for motor 1 to the particular function
        MoveMotor2ToPos(end_PositionValue2); //Sends the computed end position for motor 2 to the particular function
        MoveMotor3ToPos(end_PositionValue3); //Sends the computed end position for motor 3 to the particular function
        delay(40);// Delay for 20 msec
     }
     Serial.println ("Done Executing CAM for all Motors:"); 
  }
  Serial.flush();
  Serial.end();
}
////////////////////////////////////////////////////////////
// Function to move motor 1 to a particular position
// Sends in the input of where the end position value is
// Does a correction based on the difference calculated
///////////////////////////////////////////////////////////
void MoveMotor1ToPos(int end_PositionValue)
{ 
  int diff_position = end_PositionValue - g_positionValue1;
  if (diff_position != 0)
  {
     g_positionValue1 = end_PositionValue;
     if (diff_position < 0)
     {
       diff_position = diff_position * -1;
       analogWrite(en_1, dutyCycle);
       digitalWrite(in_1_1, LOW);
       digitalWrite(in_1_2, HIGH);
     } 
     else 
     {
       analogWrite(en_1, dutyCycle);
       digitalWrite(in_1_1, HIGH);
       digitalWrite(in_1_2, LOW);
     }
     float time_sec = ((float)diff_position/maxSpeedOfLinearMotor) * 1.6; //adjustment 60% longer based on testing
     int timeToRun = (time_sec * 1000);// Calculate time in msec
     delay(timeToRun);
     //Stop Motion
     analogWrite(en_1, 0);
     digitalWrite(in_1_1, LOW);
     digitalWrite(in_1_2, LOW);
  }
}
////////////////////////////////////////////////////////////
//Moves Motor 2 to a particular position 
//Sends the end position value as an input
//Computes the particular time to run the motor to reach
// the desired position
///////////////////////////////////////////////////////////
void MoveMotor2ToPos(int end_PositionValue)
{ 
  int diff_position = end_PositionValue - g_positionValue2;
  if(diff_position != 0)
  {
      g_positionValue2 = end_PositionValue;
      if (diff_position < 0)
      {
         diff_position = diff_position * -1;
         analogWrite(en_2, dutyCycle);
         digitalWrite(in_2_1, LOW);
         digitalWrite(in_2_2, HIGH);
      } 
      else
      {
          analogWrite(en_2, dutyCycle);
          digitalWrite(in_2_1, HIGH);
          digitalWrite(in_2_2, LOW);
      }
      float time_sec = ((float)diff_position/maxSpeedOfLinearMotor) * 1.6; //adjustment 60% longer based on testing
      int timeToRun = (time_sec * 1000);
      delay(timeToRun);
      //Stop Motion
      analogWrite(en_2, 0);
      digitalWrite(in_2_1, LOW);
      digitalWrite(in_2_2, LOW);
  }
}
////////////////////////////////////////////////////////////
// Moves Motor 3 to a particular position 
// Sends the desired end position value as an input
///////////////////////////////////////////////////////////
void MoveMotor3ToPos(int end_PositionValue)
{ 
  int diff_position = end_PositionValue - g_positionValue3;
  if (diff_position != 0)
  {
    g_positionValue3 = end_PositionValue;
    if (diff_position < 0)
    {
       diff_position = diff_position * -1;
       analogWrite(en_3, dutyCycle);
       digitalWrite(in_3_1, LOW);
       digitalWrite(in_3_2, HIGH);
    } 
    else
    {
       analogWrite(en_3, dutyCycle);
       digitalWrite(in_3_1, HIGH);
       digitalWrite(in_3_2, LOW);
    }

    float time_sec = ((float)diff_position/maxSpeedOfLinearMotor) * 1.6; //adjustment 60% longer based on testing
    int timeToRun = (time_sec * 1000); 
    delay(timeToRun);
    //Stop Motion
    analogWrite(en_3, 0);
    digitalWrite(in_3_1, LOW);
    digitalWrite(in_3_2, LOW);
  }
}

////////////////////////////////////////////////////////////
// MoveMotorToMaxOrMin
// Moves a particular motor to the max position or minimum
///////////////////////////////////////////////////////////
void MoveMotorToMaxOrMin(int motorChoice)
{
  //Assumption is that in 2 sec all motion is finished
    switch (motorChoice)
    {
      case 1: //Moves motor one to full extension of stroke length
      {
        analogWrite(en_1, dutyCycle); // sets the enable pin of motor one to the set dutycycle
        digitalWrite(in_1_1, HIGH); //sets the input pin of motor 1 to pass the signal(movement forward)
        digitalWrite(in_1_2, LOW); //sets the 2nd input pin of motor 1
        delay(1000);//1sec
        //Stop Motion motor 1
        analogWrite(en_1, 0); 
        digitalWrite(in_1_1, LOW);
        digitalWrite(in_1_2, LOW);
      }
      break;
      case 2: //Move motor 1 down to full retraction of stroke length
      {
        analogWrite(en_1, dutyCycle);
        digitalWrite(in_1_1, LOW); //sets the first input pin for motor 1 to low(movement backward)
        digitalWrite(in_1_2, HIGH);// sets the 2nd input pin for motor  1
        delay(1000);//1sec
        //Stop Motion        
        analogWrite(en_1, 0); 
        digitalWrite(in_1_1, LOW);
        digitalWrite(in_1_2, LOW);
      }
      break; 
      case 3: //sets motor 2 to full extension of stroke 
      {
        analogWrite(en_2, dutyCycle); //sets the enable pin corresponding to motor 2 to designated duty cycle
        digitalWrite(in_2_1, HIGH);   //sets the first input pin of motor 2(designate move forward)
        digitalWrite(in_2_2, LOW);    //sets the 2nd input pin 
        delay(1000);//1sec
        //Stop Motion
        analogWrite(en_2, 0);
        digitalWrite(in_2_1, LOW);
        digitalWrite(in_2_2, LOW);
      }
      break;
      case 4: //Move motor 2 to full retraction of stroke
      {
        analogWrite(en_2, dutyCycle);
        digitalWrite(in_2_1, LOW);
        digitalWrite(in_2_2, HIGH);
        delay(2000);//2sec
        //Stops Motion of motor 2
        analogWrite(en_2, 0);
        digitalWrite(in_2_1, LOW);
        digitalWrite(in_2_2, LOW);
      }
      break;
      case 5: //Moves motor 3 to full extension of stroke length 
      {
        analogWrite(en_3, dutyCycle);
        digitalWrite(in_3_1, HIGH);
        digitalWrite(in_3_2, LOW);
        delay(2000);//2sec
        //Stops Motion of Motor 3
        analogWrite(en_3, 0);
        digitalWrite(in_3_1, LOW);
        digitalWrite(in_3_2, LOW);
      }
      break;
      case 6://Move motor 3 to full retraction of stroke length
      {
        analogWrite(en_3, dutyCycle);
        digitalWrite(in_3_1, LOW);
        digitalWrite(in_3_2, HIGH);
        delay(2000);//2sec
        //Stop motion of motor 3
        analogWrite(en_3, 0);
        digitalWrite(in_3_1, LOW);
        digitalWrite(in_3_2, LOW);
      }
      break;
    }
}
