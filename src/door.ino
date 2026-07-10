#include <keypad.h>
#include <LiquidCrystal_I2C>

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
}
Keypad k= Keypad(makekeymap(keys), rowpins, colpins, rows , cols);

//lcd interfacing
LiquidCrystal_I2C lcd(0x27, 16,2);


String password= "1234";
String enteredPassword= "",

const unsigned long stateStartTime=0;
const unsigned long BOOT_TIME= 2000;

const byte relay=10;

const byte PASSWORD_LENGTH=5;

//FSM states 
enum State {
    STATE_BOOT,
    STATE_LOCKED,
    STATE_PASSWORD_ENTRY,
    STATE_VERIFY,
    STATE_GRANTED,
    STATE_DENIED,
    STATE_UNLOCKING,
    STATE_OPEN,
    STATE_LOCKING
}
State currentState= STATE_BOOT;


void displayState(){
    lcd.clear();
    switch (currentState){
        case STATE_BOOT:
            break;
        case STATE_LOCKED:
            break;
        case STATE_PASSWORD_ENTRY:
            break;
        case STATE_VERIFY:
            break;
        case STATE_GRANTED:
            break;
        case STATE_DENIED:
            break;
        case STATE_UNLOCKING:
            break;
        case STATE_OPEN:
            break;

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
                changeState(STATE_LOCKED)
            }}
            break;
        case STATE_LOCKED:{
            
            if (key=='#'){
                
                changeState(STATE_PASSWORD_ENTRY);
                }
            }
        }break;
        case STATE_PASSWORD_ENTRY:{
            if (key){
                if (isDigit(key) && (enteredPassword.length()<PASSWORD_LENGTH)){
                    enteredPassword +=key;
                    // lcd.clear();
                    // lcd.setCursor(0,1);
                    // lcd.print(enteredPassword);
                    }
                if (key=='#'){
                    changeState(STATE_VERIFY)

                    }
                if (key =='*'){
                    enteredPassword="";
                    lcd.setCursor(0,1);
                    lcd.print("                   ");
                }
                }

            }

        }
            
        case STATE_VERIFY:{
            if (enteredPassword==password){
                changeState(STATE_GRANTED)
                }
            else{
                changeState(STATE_DENIED)
            }
            break;
        case STATE_GRANTED:
            break;
        case STATE_DENIED:
            break;
        case STATE_UNLOCKING:
            break;
        case STATE_OPEN:
            break;


    }
}

void setup(){
    pinMode (relay, OUTPUT);
    lcd.init();
    lcd.backlight();
    digitalWrite(relay, LOW);
    changeState(BOOT)   
}

void(){
    char key= k.getKey();
    updateFSM();
}
