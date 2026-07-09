#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
/******************************************************************************
*******************************************************************************/

//keypad interfacing
const byte rows=4;
const byte cols=4;

byte rowpins[rows]={9,8,7,6};
byte colpins[cols]={5,4,3,2};
char keys[rows][cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad k = Keypad(makeKeymap(keys), rowpins, colpins, rows, cols);
/*************************************************************************/

//lcd interfacing
LiquidCrystal_I2C lcd(0x27, 16, 2);
/*******************************************************************************/

String password ="1234546";
String enteredPassword = "";
const byte relay =10;
const unsigned long BOOT_TIME=2000;


enum State {
    BOOT,
    LOCKED,
    PASSWORD_ENTRY,
    VERIFY_PASSWORD,
    ACCESS_GRANTED,
    ACCESS_DENIED,
    UNLOCKING,
    DOOR_OPEN,
    LOCKING
    };
State currentState= BOOT;
unsigned long stateStartTime=0;

void changeState(SystemState newState)
{
    currentState = newState;
    stateStartTime = millis();

    displayState();
}

void updateFSM()
{
    switch(currentState)
    {
        case BOOT:

            if(millis() - stateStartTime >= BOOT_TIME)
            {
                changeState(LOCKED);
            }

            break;

        case LOCKED:

            // Wait for keypad input
            break;

        case PASSWORD_ENTRY:
            break;

        case VERIFY_PASSWORD:
            break;

        case ACCESS_GRANTED:
            break;

        case.ACCESS_DENIED:
            break;

        case UNLOCKING:
            break;

        case DOOR_OPEN:
            break;

        case LOCKING:
            break;
    }
}
void displayState(){
    lcd.clear();
    switch (currentState)
        {case BOOT:
            lcd.setCursor(0,0);
            lcd.print("Door Lock");

            lcd.setCursor(0,1);
            lcd.print("Initializing");
            break;

        case LOCKED:
            lcd.setCursor(0,0);
            lcd.print("Door Locked");

            lcd.setCursor(0,1);
            lcd.print("Enter Password");
            break;

        default:
            lcd.setCursor(0,0);
            lcd.print("State:");

            lcd.setCursor(0,1);
            lcd.print(currentState);
            break;}

}

void setup() {
    Serial.begin(9600);
    
    lcd.init();
    lcd.backlight();

    pinMode(relay,OUTPUT);
    digitalWrite(relay, LOW);
    changeState(BOOT);
}

void loop() {
    updateFSM();

 

}