#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myServo;
//keypad interfacing
const byte rows=4;
const byte cols=4;
byte rowpins[rows]={9,8,7,6};
byte colpins[cols]={5,4,3,2};
char keys[rows][cols]={
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
Keypad k= Keypad(makeKeymap(keys), rowpins, colpins, rows , cols);

//lcd interfacing
LiquidCrystal_I2C lcd(0x27, 16,2);


String password= "12345";
String enteredPassword= "";
char key = NO_KEY;

unsigned long stateStartTime=0;
const unsigned long BOOT_TIME= 2000;
const unsigned long UNLOCK_TIME=5000;
const unsigned long LOCKED_TIME=30000;
const unsigned long BUZZER_TIME=5000;
const unsigned long GREEN_LED_TIME = 2000;

const byte SERVO=10;
const byte red=13;
const byte green =12;
const byte buzzer =11;
const byte MAX_ATTEMPTS=3;
const byte PASSWORD_LENGTH=5;
byte failedAttempts =0;

//FSM states 
enum State {
    STATE_BOOT,
    STATE_LOCKED,
    STATE_PASSWORD_ENTRY,
    STATE_VERIFY,
    STATE_ACCESS_GRANTED,
    STATE_ACCESS_DENIED,
    STATE_UNLOCKING,
    STATE_DOOR_OPEN,
    STATE_LOCKING,
    STATE_SYSTEM_LOCKED,
};
State currentState= STATE_BOOT;

void displayPassword(){

    lcd.setCursor(0,1);
    for (int i =0;i<PASSWORD_LENGTH;i++){
        if (i<enteredPassword.length())
            lcd.print("* ");
        else
            lcd.print("_ ");
        }
}
void Servo_run()
{
    for (int pos = 0; pos <= 90; pos++)
    {
        myServo.write(pos);
        delay(15);
    }
}
void Servo_lock()
{
    for(int pos=90; pos>=0; pos--)
    {
        myServo.write(pos);
        delay(15);
    }
}

        
    

void displayState(){
    lcd.clear();
    switch (currentState){
        case STATE_BOOT:{
            lcd.setCursor(0,0);
            lcd.print("Door Lock");

            lcd.setCursor(0,1);
            lcd.print("Initializing");
        }
            break;
        case STATE_LOCKED:{
            lcd.setCursor(0,0);
            lcd.print("Door locked");
            lcd.setCursor(0,1);
            lcd.print("press #");
        }
            break;
        case STATE_PASSWORD_ENTRY:{
            lcd.setCursor(0,0);
            lcd.print("Enter Password");
            displayPassword();
        }
            break;
        case STATE_VERIFY:
            break;
        case STATE_ACCESS_GRANTED:{
            
            lcd.setCursor(0,0);
            lcd.print("ACCESS GRANTED");
            lcd.setCursor(0,1);
            lcd.print("WELCOME");
        }
            break;
        case STATE_ACCESS_DENIED:{
            lcd.setCursor(0,0);
            lcd.print("Wrong Password");

            lcd.setCursor(0,1);
            lcd.print(MAX_ATTEMPTS-failedAttempts);
            lcd.print("Left");
        }
            break;
        case STATE_UNLOCKING:{
            lcd.setCursor(0,0);
            lcd.print("Unlocking");
        }
            break;
        case STATE_DOOR_OPEN:{
            lcd.setCursor(0,0);
            lcd.print("door open");
            lcd.setCursor(0,1);
            lcd.print("please enter");
        }
            break;
        case STATE_LOCKING:{
            lcd.setCursor(0,0);
            lcd.print("door locking");
            
        }
            break;
        case STATE_SYSTEM_LOCKED:{
            lcd.setCursor(0,0);
            lcd.print("DOOR LOCKED FOR");
            lcd.setCursor(0,1);
            lcd.print("WAIT 30 SEC");
        }

    }
}


void changeState(State newState){
    currentState=newState;
    stateStartTime=millis();

    displayState();
}

void updateFSM(){
    
    switch (currentState){
        case STATE_BOOT:{
            if ((millis()- stateStartTime) >= BOOT_TIME){
                changeState(STATE_LOCKED);
            }}
            break;
        case STATE_LOCKED:{
            
            if (key=='#'){
                enteredPassword="";
                changeState(STATE_PASSWORD_ENTRY);
                }
            }
        break;
        case STATE_PASSWORD_ENTRY:{
            
            if (key){
                if (isDigit(key) && (enteredPassword.length()<PASSWORD_LENGTH)){
                    enteredPassword +=key;
                    displayPassword();
                    }
                else if (key=='#'&& enteredPassword.length()==5){
                    changeState(STATE_VERIFY);

                    }
                else if (key =='*'){
                    enteredPassword="";
                    displayPassword();
                }
                }

            }
            break;

        
            
        case STATE_VERIFY:{
            if (enteredPassword==password){
                failedAttempts=0;
                changeState(STATE_ACCESS_GRANTED);
                }
            else{
                failedAttempts+= 1;
                changeState(STATE_ACCESS_DENIED);
            }}
            break;
        case STATE_ACCESS_GRANTED:{
            digitalWrite(green,HIGH);
            Servo_run();

            if (millis()-stateStartTime >= GREEN_LED_TIME)
            {   digitalWrite(green,LOW);
                changeState(STATE_UNLOCKING);}
            
        }
            break;
        case STATE_ACCESS_DENIED:{
            
            if (failedAttempts >= MAX_ATTEMPTS){
                
                changeState(STATE_SYSTEM_LOCKED);
                
            }
            else{
                digitalWrite(red,HIGH);
                tone(buzzer, 1000);
                if(millis()-stateStartTime>=BOOT_TIME)
                {   digitalWrite(red,LOW);
                    noTone(buzzer);
                    changeState(STATE_LOCKED);}
            }
            
            
        }
            break;
        case STATE_UNLOCKING:{
            
            changeState( STATE_DOOR_OPEN);
        }
            break;
        case STATE_DOOR_OPEN:{
            if (millis()-stateStartTime >= UNLOCK_TIME){
                changeState(STATE_LOCKING);
            }
        }
            break;
        case STATE_LOCKING:{
            Servo_lock();
            enteredPassword="";
            changeState(STATE_LOCKED);
            }
        
            break;

        case STATE_SYSTEM_LOCKED:{
            digitalWrite(red,HIGH);
            tone(buzzer, 1000);
            if (millis()-stateStartTime>= BUZZER_TIME){
                digitalWrite(red,LOW);
                noTone(buzzer);
            }
            
            if (millis()-stateStartTime>=LOCKED_TIME){
                enteredPassword="";
                failedAttempts=0;
                changeState(STATE_LOCKED);
            }
        }


    }
}

void setup(){

    myServo.attach(SERVO);

    myServo.write(0);
    pinMode (green, OUTPUT);
    pinMode (red, OUTPUT);
    pinMode (buzzer, OUTPUT);
    myServo.attach(SERVO);
    lcd.init();
    lcd.backlight();
    
    changeState( STATE_BOOT) ;  
}

void loop(){
    key= k.getKey();
    updateFSM();
}
