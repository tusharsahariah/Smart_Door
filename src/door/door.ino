#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <ESP32Servo.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BOT_TOKEN "8929379040:AAFUItIbom8wDeK-k1o15S6bd7yGTAkPa7w"
#define CHAT_ID "6088702825"
const char* ssid = "Tushar";
const char* wifipassword="1234567890";

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);



// Servo myServo;
//keypad interfacing
const byte rows=4;
const byte cols=4;
byte rowpins[rows]={12,14,27,26};
byte colpins[cols]={25,33,32,13};
char keys[rows][cols]={
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
Keypad k= Keypad(makeKeymap(keys), rowpins, colpins, rows , cols);

//lcd interfacing
Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

String password = "12345";
String enteredPassword = "";
char key = NO_KEY;

unsigned long stateStartTime=0;
const unsigned long BOOT_TIME= 2000;
const unsigned long UNLOCK_TIME=5000;
const unsigned long LOCKED_TIME=30000;
const unsigned long BUZZER_TIME=5000;
const unsigned long GREEN_LED_TIME = 2000;
const unsigned long TELEGRAM_CHECK_INTERVAL = 1000;
unsigned long lastTelegramCheck=0;

// const byte SERVO=5;
const byte red=19;
const byte green =23;
const byte buzzer =18;
const byte MAX_ATTEMPTS=3;
const byte PASSWORD_LENGTH=5;
byte failedAttempts =0;
int numMessage=0;

bool telegramPending = false;
String telegramMessage = "";

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

void handleTelegramMessage(){
    numMessage = bot.getUpdates(bot.last_message_received + 1);
    while(numMessage){
        for (int i = 0; i < numMessage; i++){
            String chat_id = bot.messages[i].chat_id;
            String text = bot.messages[i].text;
            if (chat_id != CHAT_ID)
            {
                bot.sendMessage(chat_id, "❌ Unauthorized user.", "");
                continue;
            }

            if (text == "/start")
            {
                String message =
                    "🔐 Smart Lock\n\n"
                    "Welcome!\n\n"
                    "Available Commands:\n"
                    "/start - Start bot\n"
                    "/help - Show commands";

                bot.sendMessage(chat_id, message, "");
            }

            else if (text == "/help")
            {
                String message =
                    "🔐 Smart Lock Commands\n\n"
                    "/start - Start bot\n"
                    "/help - Show commands";

                bot.sendMessage(chat_id, message, "");
            }
            else if (text=="/status"){
                String connection;
                String doorStatus;
                if (currentState == STATE_ACCESS_GRANTED ||
                    currentState == STATE_UNLOCKING ||
                    currentState == STATE_DOOR_OPEN){
                    doorStatus="🔓Lock OPEN";
                }
                else{
                    doorStatus="🔒Lock CLOSED";
                }
                if (WiFi.status() == WL_CONNECTED){
                    connection = "CONNECTED";
                }
                else {
                    connection ="NOT CONNECTED";
                }
                String message =
                    "🔐 SMART LOCK STATUS\n\n"
                    "Door: " + doorStatus + "\n"
                    "Failed Attempts: " + String(failedAttempts) + "\n"
                    "WiFi:" + connection;
                bot.sendMessage(CHAT_ID,message,"");

            }
            else if (text=="/unlock"){
                if (currentState==STATE_SYSTEM_LOCKED){
                    bot.sendMessage(CHAT_ID, "Can't unlock, system is locked","");
                }
                else if (currentState == STATE_ACCESS_GRANTED ||
                    currentState == STATE_UNLOCKING ||
                    currentState == STATE_DOOR_OPEN){
                        bot.sendMessage(CHAT_ID, "Door already Unlocked","")
                    }
                else{
                    changeState(STATE_ACCESS_GRANTED);
                    telegramPending=false;
                    telegramMessage="";
                    bot.sendMessage(CHAT_ID, "Door unlocked", "")
                }
            }
            else if(text=="/lock"){
                if (currentState==STATE_SYSTEM_LOCKED){
                    bot.sendMessage(CHAT_ID, "Can't unlock, system is locked","");
                }
                else if (currentState == STATE_LOCKING ||
                    currentState == STATE_LOCKED){
                        bot.sendMessage(CHAT_ID, "Door already locked","");
                    }
                else{
                    changeState(STATE_LOCKING);
                    telegramPending=false;
                    telegramMessage="";
                    bot.sendMessage(CHAT_ID, "Door locked", "")

                }

            }

            else
            {
                bot.sendMessage(
                    chat_id,
                    "❓ Unknown command.\n\nSend /help for available commands.",
                    ""
                );
            }


        }
        numMessage = bot.getUpdates(bot.last_message_received + 1);
    }
}

void checkTelegram(){
    if(millis()-lastTelegramCheck>=TELEGRAM_CHECK_INTERVAL){
        lastTelegramCheck= millis();
        if (WiFi.status() == WL_CONNECTED){
            handleTelegramMessage();
        }
    }
}
void connectWifi(){
    WiFi.begin(ssid, wifipassword);
    while(WiFi.status()!= WL_CONNECTED){
        delay(500);
    }
}

void sendTelegram(String message){
    bot.sendMessage(CHAT_ID,message,"");
}

void displayPassword(){

    display.setCursor(0,16);
    for (int i =0;i<PASSWORD_LENGTH;i++){
        if (i<enteredPassword.length())
            display.print("* ");
            
        else
            display.print("_ ");
            
        }
        display.display();

}
// void Servo_run()
// {   
    // for (int pos = 0; pos <= 90; pos++)
    // {
    //     myServo.write(pos);
    //     delay(15);
    // }
// }
// void Servo_lock()
// {
//     for(int pos=90; pos>=0; pos--)
//     {
//         myServo.write(pos);
//         delay(15);
//     }
// }

        
    

void displayState(){
    display.clearDisplay();
    switch (currentState){
        case STATE_BOOT:{
            display.clearDisplay();
            display.setCursor(0,0);
            display.print("Door Lock");
            
            display.setCursor(0,16);
            display.print("Initializing");
            display.display();
        }
            break;
        case STATE_LOCKED:{
            display.setCursor(0,0);
            display.print("Door locked");
           
            display.setCursor(0,16);
            display.print("press #");
            display.display();
        }
            break;
        case STATE_PASSWORD_ENTRY:{
            display.setCursor(0,0);
            display.print("Enter Password");
            display.display();
            displayPassword();
        }
            break;
        case STATE_VERIFY:
            break;
        case STATE_ACCESS_GRANTED:{
            
            display.setCursor(0,0);
            display.print("ACCESS GRANTED");
            display.display();
            display.setCursor(0,16);
            display.print("WELCOME");
            display.display();
        }
            break;
        case STATE_ACCESS_DENIED:{
            display.setCursor(0,0);
            display.print("Wrong Password");
            display.display();

            display.setCursor(0,16);
            display.print(MAX_ATTEMPTS-failedAttempts);
            display.display();
            display.print("Left");
            display.display();
        }
            break;
        case STATE_UNLOCKING:{
            display.setCursor(0,0);
            display.print("Unlocking");
            display.display();
        }
            break;
        case STATE_DOOR_OPEN:{
            display.setCursor(0,0);
            display.print("door open");
            display.display();
            display.setCursor(0,16);
            display.print("please enter");
            display.display();
        }
            break;
        case STATE_LOCKING:{
            display.setCursor(0,0);
            display.print("door locking");
            display.display();
            
        }
            break;
        case STATE_SYSTEM_LOCKED:{
            display.setCursor(0,0);
            display.print("DOOR LOCKED FOR");
            display.display();
            display.setCursor(0,16);
            display.print("WAIT 30 SEC");
            display.display();
        }

    }
}

void prepareTelegramMessage()
{
    switch (currentState)
    {
        case STATE_ACCESS_GRANTED:
            telegramMessage = "🔓 Door Unlocked";
            telegramPending = true;
            break;

        case STATE_ACCESS_DENIED:
            telegramMessage = "❌ Wrong Password";
            telegramPending = true;
            break;

        case STATE_SYSTEM_LOCKED:
            telegramMessage = "🚨 System Locked";
            telegramPending = true;
            break;

        default:
            break;
    }
}
void processTelegram()
{
    if (telegramPending && WiFi.status() == WL_CONNECTED)
    {
        bot.sendMessage(CHAT_ID, telegramMessage, "");

        telegramPending = false;
        telegramMessage = "";
    }
}
void changeState(State newState){
    currentState=newState;
    stateStartTime=millis();

    displayState();
    prepareTelegramMessage();
    
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
            // Servo_run();

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
                digitalWrite(buzzer,HIGH);
                // tone(buzzer, 1000);
                if(millis()-stateStartTime>=BOOT_TIME)
                {   digitalWrite(red,LOW);
                    digitalWrite(buzzer,LOW);
                
                    // noTone(buzzer);
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
            // Servo_lock();
            digitalWrite(green, LOW);
            enteredPassword="";
            changeState(STATE_LOCKED);
            }
        
            break;

        case STATE_SYSTEM_LOCKED:{
            digitalWrite(red,HIGH);
            digitalWrite(buzzer,HIGH);
            // tone(buzzer, 1000);
            if (millis()-stateStartTime>= BUZZER_TIME){
                digitalWrite(red,LOW);
                // noTone(buzzer);
                digitalWrite(buzzer,LOW);
            }
            
            if (millis()-stateStartTime>=LOCKED_TIME){
                enteredPassword="";
                failedAttempts=0;
                changeState(STATE_LOCKED);
            }
        }


    }
}

void setup() {
    // myServo.attach(SERVO);
    // myServo.write(0); 
    // pinMode(SERVO,OUTPUT);

    pinMode(green, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(buzzer, OUTPUT);

    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    // digitalWrite(SERVO, LOW);
    // noTone(buzzer);
    digitalWrite(buzzer,LOW);
    
    Wire.begin(21,22);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        while(true);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
    

    client.setInsecure();
    connectWifi();
    sendTelegram("🔐 Smart Lock Started");
    changeState(STATE_BOOT);
}

void loop(){
    key= k.getKey();
    updateFSM();
    processTelegram();
    checkTelegram();
}