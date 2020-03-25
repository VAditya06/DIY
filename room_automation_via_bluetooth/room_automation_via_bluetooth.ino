/* This piece of code is written by                                                         ROOM AUTOMATION 
 *  Aditya.v
 *  Aditya.github06@gmail.com
 *  
 *  This code is for room automation.
 *  Ideal for room containing onr tube light, one fan and one night lamp.
 *  Any changes to the code can be done by the DEVELOPER only. 
 *  
 *  
 *  This code is subjected to copyright.
 *  
 *  For further information contact the DEVELOPER with the above given details.
 *  DOWNLOAD THE APK FOR THE APP FROM THIS REPOSITORY
 */



#include<LiquidCrystal_I2C.h>
#include<Wire.h>

//Definition of the relay 
#define relay1 2
#define relay2 3
#define relay3 4
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);//Declaring the i2c chip for the lcd screen

char val; //variable to accept the Serial.read() data
boolean value; //power consumption variable
boolean power; //power consumption variable
boolean night_light = true; //Backlight variable (to enable backlight for the lcd without overloading)

void switch_off(){      //This function switches off all the lights connected to the relay
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TURNING OFF .......");
  lcd.setCursor(0,1);
  lcd.print("Wait for 2 sec");
  lcd.noCursor();
  delay(2000);
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  backlight_off();
  night_light = true;
  }

void switch_on(){      //This function switches on all the lights connected to the relay
  backlight_on();
  lcd.clear();
  lcd.print("Everything is on");
  
  digitalWrite(relay1, LOW);
  digitalWrite(relay3, LOW);
}

void backlight_off(){  //Function to turn the lcd off
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  lcd.noDisplay();
 }

 void backlight_on(){  //Function to turn the lcd on
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  lcd.display();
 }


void setup() {
  pinMode(relay1, OUTPUT);//setting the pin mode for all the relay pins as output 
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  lcd.begin(16,2);//Initializing the lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INITIALIZING...");
  lcd.setCursor(0,1);
  lcd.print("Wait for 2 sec");
  delay(2000);
  lcd.clear();
  
  digitalWrite(relay1, HIGH);//Turning off all the relay's when the arduino is reset
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  Serial.begin(9600);//Beginning the serial monitor at 9600 baud rate
  
}
//This loop executes forever 
void loop() {
  if(Serial.available()>0)//Checking if any serial data is available
    val = Serial.read();//if it is available then reading the data and assigning to val
    
    
    //The particular light is turned on
  if(val == '1'){
    if(power == true){ //this is to prevent from turning on the tube light when the night lamp is on
      backlight_off();  //turning off the backlight for the led
      digitalWrite(relay1, HIGH);  //turning off the tubelight
    }
    else{     
      digitalWrite(relay1, LOW);  //Turning on the tubelight
      backlight_on();
      lcd.clear();
      lcd.display();
      lcd.setCursor(0,0);
      lcd.print("TUBELIGHT ON");
      lcd.noCursor();
      value = true;  //setting the boolean value to true for power consuption
    }  
       
  }
  else if(val == '2'){ //Serial Read to check if the night lamp must be turned on
    if(value == true){ //power consumption(to turn off the tube light before turning on the night lamp)
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("TURN OFF THE");
      lcd.setCursor(0,1);
      lcd.print("LIGHT FIRST");
      delay(5000); // delay 5 seconds
    }
    while(night_light == true){  //Loop to print the night light status before powering off the lcd without overloading
      backlight_on();
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("NIGHT LIGHT ON");
    lcd.noCursor();
    delay(2000);
    night_light = false;
    }
    digitalWrite(relay2, LOW); //Turnning on the night lamp on
    power = true;  //Setting the boolean value to true (for switching on and off the fan even when the night lamp is on with the output on the lcd display)
    delay(3000); // delay for 3 seconds
    backlight_off();
    
  }
  else if(val == '3'){   //Turn on the fan if the condition is met
    if(power == true){  //checks if the night light is on ; And allows us to turn on the fan even if the night lamp is on with the lcd display output
      digitalWrite(relay3, LOW); //Turning the fan on
      backlight_on();
      lcd.clear();
      lcd.display();
      lcd.setCursor(0,0);
      lcd.print("FAN ON");  //setting the output as "fan on"
      lcd.noCursor(); 
      delay(2000);   //delay for 2 seconds
      while(power == true){
      backlight_off();   //Turns off the backlight after 2 seconds
      val = Serial.read();   //Serial read to check if the input
      if(val == 'B'){    //if the condition is met the night lamp is turned off
        digitalWrite(relay2, HIGH);    //Turning off the night lamp off
        power = false;  //Boolean value set to false so as to break out of the while loop
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("NIGHT LIGHT OFF");
        lcd.noCursor();
        backlight_on();
        lcd.setCursor(0,1);
        lcd.print("GOOD MORNING");
      }
      else if(val == 'C'){  //Serial read to check if the fan has to be turned off
        digitalWrite(relay3, HIGH); //Turning the fan off
        backlight_on();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("FAN OFF");
        lcd.noCursor();
        delay(1000);  //Delay for 1 second
  }
  else if(val == '3'){  //Serial read to check if the fan must be turned on
        digitalWrite(relay3, LOW); //Turning the fan on
        backlight_on();
        lcd.clear();
        lcd.display();
        lcd.setCursor(0,0);
        lcd.print("FAN ON");
        lcd.noCursor();
        delay(1000);  //Delay for 1 second
      }
     }  
    }
    else{  //Initially if the power = false then this else loop is executed
      digitalWrite(relay3, LOW); //turning the fan on
      backlight_on();
      lcd.clear();
      lcd.display();
      lcd.setCursor(0,0);
      lcd.print("FAN ON");
      lcd.noCursor();
  }
  }
  
  
  //the particular lights are turned off
  else if(val == 'A'){
    digitalWrite(relay1, HIGH);  //Turning the tube light off
    value = false; //setting the boolean "value" to false to break out of the if condition
    lcd.clear();
    lcd.display();
    lcd.setCursor(0,0);
    lcd.print("TUBELIGHT OFF");
    lcd.noCursor();
    if(power == true)
      backlight_off(); 
  }
  else if(val == 'B'){
    if(power == true){  //To check if the night lamp has already been turned off or not to save computing power
      digitalWrite(relay2, HIGH); //Turning off the night lamp
      power = false;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("NIGHT LIGHT OFF");
      lcd.noCursor();
      backlight_on();
      lcd.setCursor(0,1);
      lcd.print("GOOD MORNING");
    }
  }
  else if(val == 'C'){
    digitalWrite(relay3, HIGH); //Turning off the fan
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FAN OFF");
    lcd.noCursor();
  }
  
  
  //All the light are switched off
  else if(val == '9')
    switch_on();
  //All the lights are switched on
  else if(val == 'I')
    switch_off();
  delay(100);
  }
