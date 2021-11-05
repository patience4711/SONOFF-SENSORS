
/* deze software is generalistisch voor 4 types:
 voor de sonoff met sensors: define SENSORS
 voor de ESP01 #define ESP01
 voor de ESP8285 #define ESP8285
 voor de Sonoff S20 #define BUILDIN
 voor de Sonoff mini #define MINI
 Geen Wifimanager maar eigen wifi configportal
*/

#define SENSORS
//#define ESP01_FET
//#define ESP01_TRIAC
//#define ESP8285
//#define S20  // dit is de S20 met tweekleurige led
//#define MIN
//#define SONOFF // normal sonoff w/o sensors
//#define WEMOS  // for testing

#include <time.h>
#include <sunMoon.h>

#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal

#include <ArduinoJson.h>
#include <Arduino.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);


#ifdef SENSORS
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include <ESP8266HTTPClient.h>
#include "MAX44009.H"
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#endif

#include <PubSubClient.h>
#include "HTML.H"
#include "AAA_PORTAL_HTML.H"
WiFiClient My_WifiClient ;
PubSubClient MQTT_Client ( My_WifiClient ) ;
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org");
const byte DNS_PORT = 53;
DNSServer dnsServer;
#include<EEPROM.h>
#define TIMERCOUNT  4

//#define DEBUG
#ifdef  DEBUG
#define DebugPrint(x)     Serial.print (x)
#define DebugPrintDec(x)  Serial.print (x, DEC)
#define DebugPrintln(x)   Serial.println (x)
#else
#define DebugPrint(x)
#define DebugPrintDec(x)
#define DebugPrintln(x)
#endif


// variabelen die met de klok en timers te maken hebben
//  WiFiUDP Udp; 
  time_t dagbegintijd = 0;
  time_t uitschakeltijd[4] = {0};
  time_t inschakeltijd[4] = {0};
  time_t inschakeltijdstip = 0;
  time_t repeateruitschakeltijd = 0;  
//  time_t zetAanschakeltijd = 0;
  byte mDay = 0;
  String  maan = "";
  int dst;

 // variabelen wificonfig
  char pswd[11] = "0000";
//  char lengte[8] = "5.123";
//  char breedte[8] = "51.345";
  float longi = 5.123;
  float lati = 51.123;
  char timezone[5] = "+120";  //+5.30 GMT
  char ssid[33] = ""; // was 33 
  char pass[40] = ""; // was 40
  bool tryConnectFlag = false;

  bool zomerTijd = false;
  //bool checkBool = false;
//  bool mqttEnable = false;
//  bool optelBool = false;
  char static_ip[16] = "000.000.000.000";
 
 //variabelen timerconfig 
  char timerActive[5] = "0000";
  char relToSunOn[5] = "0000"; 
  char relToSunOff[5] = "0000"; 
  char switchOn[21] = "00:0000:0000:0000:00";
  char switchOff[21] = "00:0000:0000:0000:00";
  char weekDag[29] = "nnnnnnnnnnnnnnnnnnnnnnnnnnnn";
  char switchcdwn[6] = "00:00";
//  char zetAan[3] = "00";
  char switchHL[5] = "0000"; // bool voor hoger of lager therm hyg licht dig

#ifdef SENSORS
  char switchTemp[6] = "+0.1"; // was 4
  char switchMoist[3] = "11"; // de vochtigheidsgraad
  char switchLicht[6] = "20"; // omgevingslicht in lux 
  char BS[1] = "";
  char tempCal[5] = "+0.0";
  char sensor[1] = "";
  int meetRes = 180; // hebben we nodig om de mqtt verbinding te bewaken
  char timer[2] = "1";
#endif


  //uiverselevariabelen basis config 
  char swName[21] = "";
  char userPasswd[11] = "1111";
  char aso[6] = "06:00";

String  mqttBroker=    "192.168.0.100";
String  mqttIntopic =   "esp/in";
String  mqttOuttopic =  "domoticz/in";
//String  Mqtt_Username = "";
//String  Mqtt_Password = "";
String  mqttPort =     "1883";
bool    mqttEnabled =  false;
int     idxSwitch = 0;
int     idxSensor = 0;  
String requestUrl = ""; // to remember where we came from  
//  String mqttMess;
//  int mqttfoutTeller = 0;
//  bool logAdmin = false; 
  bool mustSwitch[4] = {false, false, false, false }; // = 0;
  bool hasSwitched[4] = {false, false, false, false }; // = 0;  
  bool mustCalc[4] = {false, false, false, false }; // = 0;
  int actionFlag = 0;
//  int verklikker = 0; // heeft domoticz iets verstuurd
  int cdwnuur = 0;
  int cdwnminuut = 0;
  int asouur = 0; // beveiliging
  int asominuut = 0;
  time_t sunset = 0;
  time_t sunrise = 0;
  bool timeRetrieved = false;
  static unsigned long laatsteMeting = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  static unsigned long lastCheck = 0;
  int tKeuze=100;
  int networksFound =0;

#ifdef SENSORS
  int weerteller = 1;
//  String domReaction;
  int verzonden = 0;
  //String domString = "geen";

  Adafruit_BME280 bme; // I2C
  //float h, t, p, pin, dp;
  float p, pin, dp;
  int bar_for = 0;
  float luchtdruktabel[11];
  String ldString="←→";

  MAX44009 licht;

#define ONE_WIRE_BUS  3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define BME280_ADDRESS                (0x4A)
#define DHTPIN  3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE, 22); // 11 works fine for ESP8266
float humidity, temp_f, temp_c;  // Values read from sensor
float switch_temp = 0;
#endif
//unsigned long previousMillis = 0;        // will store last temp was read
//const long interval = 2000; 

int datum = 0; //
#define knop              0  //

#if defined SENSORS || defined S20  || defined MINI   || defined SONOFF // alle sonoffs
   #define RELAY_PIN        12  // esp-12 en sonoff
   #define led_onb           13  // onboard led was 2  sinc!
   #define LED_AAN  LOW     // 
   #define LED_UIT  HIGH
   #define SWITCH_AAN  HIGH
   #define SWITCH_UIT  LOW
#endif

#ifdef MINI
#define sw_pin             4
#endif

#ifdef ESP01_FET 
   #define RELAY_PIN        3  // stuurt een transistor met fet aan, laag is aan !!
   #define led_onb           2  // moet sourcen
   #define LED_AAN  LOW
   #define LED_UIT  HIGH
   #define SWITCH_AAN  LOW
   #define SWITCH_UIT  HIGH
#endif   

#ifdef WEMOS
   #define RELAY_PIN        3  // stuurt een transistor met fet aan, laag is aan !!
   #define led_onb           2  // moet sourcen
   #define LED_AAN  LOW
   #define LED_UIT  HIGH
   #define SWITCH_AAN  LOW
   #define SWITCH_UIT  HIGH
#endif

#ifdef ESP01_TRIAC
// deze heeft een opto-coupler op pin2 dus sinc
   #define RELAY_PIN        2  // bij de socket sinc !!
   #define led_onb           3  // mag sourcen
   #define LED_AAN  HIGH  //normale socket waarbij gesourced wordt, bij stekkerdoos LOW
   #define LED_UIT  LOW
   #define SWITCH_AAN  LOW  //
   #define SWITCH_UIT  HIGH //
#endif

#ifdef ESP8285
   #define RELAY_PIN     12  // stuurt een transistor met FET dus hoog  == uit 
   #define led_onb        2   //  onboard led  sinc ! cannot be low @ boot
   #define touch_pin      4   // kunnen we gewoon aan pin 0 hangen
   #define LED_AAN  LOW
   #define LED_UIT  HIGH
   #define SWITCH_AAN  LOW
   #define SWITCH_UIT  HIGH
#endif

String toSend = ""; 
int value = 0;
int event = 0;
int aantal = 0;

////////////////////////////////////////////////////////
// variables To record and display last events on the home page.
struct logEvent {
  String    Log_date ;
  String    Log_kind ;
  String    Log_message;
};

static const int Log_MaxEvents = 20;    // Please avoid to make a too big value here, the past events will be stored in memory and displayed in the html home
bool Log_MaxReached = false;
byte Log_CurrentEvent = 0;
logEvent Log_EventList[Log_MaxEvents];  // To stock in memory the last x events (20 by default)


// *****************************************************************************
// *                              SETUP
// *****************************************************************************

void setup() {

#ifdef  DEBUG and SENSORS //alleen bij een sonoff met sensors
// als we willen debuggen dan moet tx de gewone functie hebben:
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  delay(10);
  #define TXpin      14
# else
 #define TXpin       1
 pinMode(TXpin, FUNCTION_3);  // schakel over naar gpio
 Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
 delay(10);
 #endif

#if defined ESP01_FET || defined ESP01_TRIAC || defined SENSORS
pinMode(3, FUNCTION_3); //rx als gpio pin
#endif

#ifdef MINI
  pinMode(sw_pin, INPUT_PULLUP); // de s-in terminal
#endif
  
  pinMode(knop, INPUT_PULLUP); // de knop
  pinMode(RELAY_PIN, OUTPUT); // de lamp
  pinMode(led_onb, OUTPUT); // onboard led

  ledblink(1, 800);
  
  attachInterrupt(digitalPinToInterrupt(knop), isr, FALLING);

  digitalWrite(RELAY_PIN, SWITCH_UIT); // initieel uit

  SPIFFS_read();
// Serial.println("\n \n na spiffs read zomerTijd = " + String(zomerTijd) );
//Serial.println("checkBool na read basisconfig = " + String(checkBool) );
  read_eeprom(); // put the value of requestUrl back
  if (requestUrl == "") requestUrl="/"; // vangnet
  
  start_wifi(); // met de gelezen gegevens een verbinding maken
// Serial.println("na start_wifi zomerTijd = " + String(zomerTijd) );  

  getTijd(); // get the time, when successful the timers are armed for calculation
//  switchCalc(); // calculate the timers although maybe not needed


#ifdef SENSORS
if (sensor[0] == '4' || sensor[0]=='5' || sensor[0]=='7' ){  // voor de body, touch, digital sensor
    pinMode(3, INPUT);
    digitalWrite(3, LOW);
  }
  if (sensor[0] == '3' || sensor[0] == '6'){  // voor de bme of de lichtsensor
    //Wire.begin(3, TXpin);  // sda 3  scl 1  rx = pin3 tx=pin1
    Wire.begin(TXpin, 3);  // sda 3  scl 1  rx = pin3 tx=pin1
   }
   if(sensor[0] == '3') {
      DebugPrint("het BME 12c adres = ");  
      DebugPrint(BME280_ADDRESS); 
      DebugPrintln("");
      DebugPrintln(F("BME280 verbinding testen ....."));
          if (!bme.begin()) {
           //Serial.println("Could not find a valid BME280 sensor, check wiring!");
           //while (1);
          }
     }
    if(sensor[0] == '6') { 
      DebugPrint("het max44009 12c adres = ");  //gp4 = sda gp5 = scl
      DebugPrint(MAX_ADDR); 
      DebugPrintln("");
      DebugPrintln(F("MAX44009 verbinding testen ....."));
          if (!licht.begin()) {
           DebugPrintln("geen max44009 sensor aangesloten, check verbindingen!");
          }
     }
   sensors.begin();
  delay(1000);  //anders krijgen we een foutmeting 
#endif

//  ****************** mqtt init *********************
   MQTT_Client.setServer(mqttBroker.c_str(), mqttPort.toInt());
   MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;
  

  Update_Log("system","boot up");
  healthCheck(); //starts mqtt and checks time
  
  #ifdef SENSORS 
  meetENschakel(); //
  #else
  mqttSwitchupdate(); //mqtt dat de schakelaar uit is
  #endif

  ledblink(3, 500);
  
  //DebugPrint("laatsteMeting = "); DebugPrintln(String(laatsteMeting));
  laatsteMeting = 0; //nodig?? jaaaa, hier komt een waarde in te staan waarvan ????
  Serial.println("setup done");
  Serial.println(WiFi.localIP());
  //Serial.println("einde setup zomerTijd = " + String(zomerTijd) );
}

//****************************************************************************
//*                         LOOP
//*****************************************************************************
void loop() {

  // ******************* foutcontroles ***************************

  MQTT_Client.loop();

  if ( !timeRetrieved && aantal < 3) {
    getTijd(); // // if successfull the mustCalc's are set
    aantal += 1;
  } //

// healtCheck
unsigned long nu = millis();  // de tijd dat het programma al loopt
 if ( nu - lastCheck >= 1000UL * 600 ) {
    lastCheck += 1000UL * 30 ; // wordt iedere keer (meetRes * miliseconden) groter;
    healthCheck();
} 

#ifdef SENSORS
 nu = millis();  // de tijd dat het programma al loopt
 if ( nu - laatsteMeting >= 1000UL * meetRes ) {
    laatsteMeting += 1000UL * meetRes ; // wordt iedere keer (meetRes * miliseconden) groter;
    meetENschakel();
}
#endif      
  
// ****************** check the actionflags *************************
 if(actionFlag != 0) 
 {   
        if (actionFlag == 11 ) 
        { // de knop is langdurig ingedrukt geweest, opstarten in AP
           toSend="";
           delay(1000); //time to release button
           DebugPrintln("erasing the wifi credentials, value = " + String(value) + "  actionFlag = " + String(actionFlag));
           eraseFlash(); // wipe the sector where the cerdentials are saved
           ESP.restart();
       }
       if (actionFlag == 10)
       { // the button has been pressed utill the led goes on. resetting
           delay(2000); // give the server the time to send the confirm
           DebugPrintln("rebooting on actionFlag==10");
           write_eeprom();
           ESP.restart();
      }
      if (actionFlag == 20)
      {
          actionFlag = 0;
          value = 1;
          switch_on_now(true, true, "button");
      }  
      if (actionFlag == 21) 
      {
          actionFlag=0;
          //value = 0; not needed
          switch_off_now(true, true, "button");
      }
    
    #ifdef SENSORS
      if (actionFlag == 15) // the buttom
      {
          actionFlag=0;
          meetENschakel();
      }
    #endif 
 }  

  if(Serial.available() > 7) {
    Serial.println(F("Serial incoming bytes"));
       handle_Serial();
   }  

// *********************** dayly synchronize and arm the timers for recalculation  
  // if there is a new date we synchronize at 4 o'clock
   if (day() != datum && hour() > 4) { // if date overflew and later than 4
   getTijd(); // synchronize and arm the timers for recalculation
   }
#ifdef SENSORS
  // ****************************************************************************
  //                schakelen door de timers
  // ****************************************************************************  
  if (timer[0] == '1') // overrules the individual timer active settings 
  { 
       // if the switch is off and armed, we can recalculate it
       for(int z=0; z<TIMERCOUNT; z++)
       {
          //mustCalc is only true if the timer is active
          if(mustCalc[z] == true && hasSwitched[z] == false) //mustCalc only timers that have timerActive true
          { // only when off
             timerCalc(z);  // when calculated mustCalc becomes false
          }
       }    
       timerSwitch(); 
}
#else
// not SENSORS  
      // if the switch is off and armed, we can recalculate it
       for(int z=0; z<TIMERCOUNT; z++)
       {
          //mustCalc is only true if the timer is active
          if(mustCalc[z] == true && hasSwitched[z] == false) //mustCalc only timers that have timerActive true
          { // only when off
             timerCalc(z);  // when calculated mustCalc becomes false
          }
       }    
       timerSwitch(); 
#endif
#ifdef SENSORS   
   //  ************** automatisch uitschakelen door de cdwn ********************************
   // is active when a motion sensor is triggerd
   // test if triggered by the motionsensor
    if (event == 18 ) 
    { // motion sensor  
           if (now() > inschakeltijdstip + ((cdwnuur * 3600 + cdwnminuut) * 60)) 
           { // now = current time in seconds
               switch_off_now(true, false, "countdown"); //meteen de lamp uit en mqtt, geen timercheck
               event = 0;
           }
     }

#endif

// ****************** forgotten lamp security *********************************************
// if the lamp is switched manual it would be on forever

if ( value == 1 || value == 2 || value == 17) { // switched on by button or web or mosquitto
// als de lamp brandt ( dat doet ie als event > 0 )
     if ( now() > inschakeltijdstip + ((asouur * 60 + asominuut) * 60)) { // 6 uur == 21600 voor test 3 min. == 180sec
             switch_off_now(true, false, "security"); //meteen de lamp uit, mqtt, geen timercheck
             //DebugPrintln("uitgeschakeld door beveiliging"); 
             //value = 0;
             //Update_Log("security","switched off"); 
     }
}


#ifdef SENSORS 
// ***************************** motion sensor ****************************************
// behaviour can be set in timers
// als er een bewegingssensor ingesteld is en de pin is hoog
// en de lamp brandt niet door handmatig (domoticz) of timers ingeschakeld

// is de bewegingssensor gekozen dan moeten we kijken of we moeten inschakelen, dit hoeft alleen als de lamp
// niet aan is, dus niet ingeschakeld door web, knop domoticz of timers of bewegingssensor
// boodschap moet wel worden vezonden, ook als de lamp brandt
// de lamp wordt door countdwn uitgeschakeld
if ( sensor[0] == '4' ) { // works also when timers are active
   if ( digitalRead(3) == 1 ) // seen motion?
   { 
      //must the lamp be switched on? only when off
      //bool switched = false;
      if ( bwswitch() && value == 0 ) // we have to switch and the lamp = off
      { 
         value = 18;
         switch_on_now(true, false, "motion sensor"); // lamp on at once
         Update_Log("system", "motion sensor triggerd");
         event = 18;
         //DebugPrintln("bodysensor getriggerd");
      }
      if ( boodschap() ) 
      {
          // first check if a message is sent, if not do this and make verzonden true  
          if ( verzonden == 0 ) 
          {
             verzonden = 1;
             sendMqttsensor(); //zend de boodschap naar de sensor idx2
             inschakeltijdstip = now(); // we weten nu het tijdstip dat de sensor iets heeft gezien
          }
      }      
   } 
   else 
   {
   //  sensor is low
       if ( verzonden == 1 ) // an on message is sent
       { 
        //we send an off message after 30 sec
            if (now() > inschakeltijdstip + 30 ) // now = current time in seconds thus after 30 sec
            { 
                verzonden = 2;
                sendMqttsensor(); //
                verzonden = 0; // 
           }
       }    
   }
}

 // delay will cause a crash in the async webserver so actions that require delay
 // are performed in the loop bij a value of actionflag

// ***************************** externe drukknop sensors ****************************************
if( sensor[0] == '5' ) { // deze werkt naast de timers dus timer[0] kan op 1 staan
  if (digitalRead(3) == 1) {
       DebugPrint("drukknop gedetecteerd, de waarde van VALUE = ");
      DebugPrintln(value);
      unsigned long starttijd = millis(); // sla de tijd op in currentMillis
      unsigned long eindtijd = millis();
        while (eindtijd - starttijd  <= 50) { //2000 millis = 2 sec // was 100
        eindtijd = millis();
        }
 // na even gewacht te hebben kijken we of de knop nog is ingedrukt
int val = digitalRead(3);
     if (val == 0) { // dwz de knop is weer losgelaten
       DebugPrintln("knop was te kort ingedrukt dus een toevallige storing");
      } else { //de knop is nog ingedrukt
              digitalWrite(led_onb, LED_AAN);
               delay(200); // even wachten
               digitalWrite(led_onb, LED_UIT); // We zetten de led uit
               value = value+1; //aanuitsignaal = altijd 1,   of groter ...
     
                if (value > 1) { // value was 1 dus er was ingeschakeld
                    //DebugPrintln("we moeten uitschakelen");
                    actionFlag = 21; // make it switch off
                    delay(1000); //om repetities te voorkomen
                } else {
                     //DebugPrintln("we moeten inschakelen"); 
                     actionFlag = 20;
                     delay(1000); //om repetities te voorkomen
                }
            }
        }
    }
#endif
#ifdef MINI
// ***************************** externe drukknop mini ****************************************

  if (digitalRead(sw_pin) == LOW) {
       //DebugPrint("drukknop gedetecteerd, de waarde van VALUE = ");
      //DebugPrintln(value);
      unsigned long starttijd = millis(); // sla de tijd op in currentMillis
      unsigned long eindtijd = millis();
        while (eindtijd - starttijd  <= 50) { //2000 millis = 2 sec // was 100
        eindtijd = millis();
        }
 // na even gewacht te hebben kijken we of de knop nog is ingedrukt
int val = digitalRead(sw_pin);
     if (val == 1) { // dwz de knop is weer losgelaten
       //DebugPrintln("knop pressed too short, was a disturbance");
      } else { //de knop is nog ingedrukt
              digitalWrite(led_onb, LED_AAN);
               delay(200); // even wachten
               digitalWrite(led_onb, LED_UIT); // We zetten de led uit
               value = value+1; //aanuitsignaal = altijd 1,   of groter ...
     
                if (value > 1) { // value was not 1 
                    //DebugPrintln("we moeten uitschakelen");
                    //value=0; //
                    actionFlag = 21;
                    delay(1000); //om repetities te voorkomen
                } else {
                     //DebugPrintln("we moeten inschakelen"); 
                     actionFlag = 20;
                     delay(1000); //om repetities te voorkomen
                }
            }
        }
 
#endif

}
//****************Einde Loop   *****************************


void ledblink(int i, int wait) {
  for (int x = 0; x < i; x++) {
    digitalWrite(led_onb, LED_AAN);
    delay(wait);
    digitalWrite(led_onb, LED_UIT);
    delay(wait);
  }
}
#ifdef SENSORS  

// schakelen door de sensors
// we checken altijd de status eerst (value) anders worden er telkens mqtt boodschappen verzonden  
// als door de sensor wordt geschakeld dan wordt dit in meten() in de mqtt boodschap verwerkt
// dus er hoeft niets te qworden verzonden  
// *********** automatisch inschakelen door thermostaat *************************************************

// in de mqtt message zit ook de status van de schakelaar als deze bool true is
void thermostaat() {
  if (temp_c == 1000 || temp_c == -127) {
    return;
  }
  float temperatuur = atof(switchTemp);
  //DebugPrint("switchTemp = ");
  //DebugPrintln(switchTemp);
  char b = switchHL[0]; 
      if (temp_c > switch_temp) { // als de polled temperatuur hoger is

           if ( b == '0' && value == 0) { // inschakelen als 'hoger dan' is geselecteerd en niet aan is
                  value = 12;
                  switch_on_now(true, false, "thermostate"); //meteen de lamp aan
                  //DebugPrintln("ingeschakeld door thermostaat");
                  //event = 12;
                  
                  return;
           }
           if ( b == '1' && value > 0 ) { //uitschakelen als 'lager dan' is geselecteerd
                  //DebugPrintln("uitgeschakeld door thermostaat");
                  switch_off_now(true, false, "thermostate"); //meteen de lamp uit en mqtt message
                  //event = 32;
                  return;
                 }
           
      } else { // als de polled temperatuur lager is
           if ( b == '1' && value == 0) { // inschakelen als 'lager dan' is geselecteerd en niet aan is
                  switch_on_now(true, false, "thermostate"); //meteen de lamp aan
                  //DebugPrintln("ingeschakeld door thermostaat");
                  //event = 12;
                  value = 12;
                  return;
           }
           if ( b == '0' && value > 0 ) { //uitschakelen als 'hoger dan' is geselecteerd en aan is
                  //DebugPrintln("uitgeschakeld door thermostaat");
                  switch_off_now(true, false, "thermostate"); //meteen de lamp uit en mqtt message
                  //event = 32;
                  return;
                 }
           }  
//return false; // als er niet is geschakeld
} 
  // *********** automatisch inschakelen door hygrostaat *************************************************
void hygrostaat() {
  if (temp_c == 1000 || temp_c == -127) {
    return;
  }
  int hum = atof(switchMoist);
  DebugPrint("hum = ");
  DebugPrintln(hum);
  char b = switchHL[1]; 

      if (humidity > hum) { // als de polled vochtigheid hoger is
           if ( b == '0' && value == 0) { // inschakelen als 'hoger dan' is geselecteerd en niet aan is
                  value = 13;
                  switch_on_now(true, false, "humidity sensor"); //meteen de lamp aan
                  //DebugPrintln("ingeschakeld door hygrostaat");
                  //event = 13;
                  return;
           }
           if ( b == '1' && value > 0 ) { //uitschakelen als 'lager dan' is geselecteerd
                  //DebugPrintln("uitgeschakeld door hygrostaat");
                  switch_off_now(true, false, "humidity sensor"); //meteen de lamp uit en mqtt message
                  //event = 33;
                  return;
                 }
           
      } else { // als de polled vochtigheid lager is

           if ( b == '1' && value == 0) { // inschakelen als 'hoger dan' is geselecteerd en niet aan is
                  value = 13;
                  switch_on_now(true, false, "humidity sensor"); //meteen de lamp aan
                  //DebugPrintln("ingeschakeld door hygrostaat");
                  //event = 13;

                  return;
           }
           if ( b == '0' && value > 0 ) { //uitschakelen als 'lager dan' is geselecteerd
                  //DebugPrintln("uitgeschakeld door hygrostaat");
                  switch_off_now(true, false, "humidity sensor"); //meteen de lamp uit en mqtt message
                  //event = 33;
                  return;
                 }
        }
    
  }

// *********** automatisch inschakelen door lichtsensor *************************************************
void lichtSensor() {
  float licht = atof(switchLicht);
  DebugPrint("switchLicht = ");
  DebugPrintln(licht);
  char b = switchHL[2]; 
      if (p > licht) { // if the polled light is higher
           if (b == '0' && value == 0) { // //switch on when 'higher than' is selected and off
                  value = 14;
                  switch_on_now(true, false, "lightsensor"); //meteen de lamp aan
                  //DebugPrintln("ingeschakeld door lichtsensor");
                  //event = 14;

                  return;
           }
          if (b == '1' && value >0) { //switch off when 'lower than' is selected and on
                  //DebugPrintln("uitgeschakeld door lichtsensor");
                  switch_off_now(true, false, "lightsensor"); //meteen de lamp uit en mqtt message
                  //event = 34;
                  return;
                 }
           
      } else { // de polled lichtsterkte is dus lager
           if (b == '1' && value == 0) { // //switch on when 'lower than' is selected and off
                  value = 14;
                  switch_on_now(true, false, "lightsensor"); //meteen de lamp aan
                  return;
                  }
          if (b == '0' && value > 0) { //switch off when 'higer than' is selected and on
                  switch_off_now(true, false, "lightsensor"); //meteen de lamp uit en mqtt message
                  return;
                 }
        }
}
// *********** automatic switch by generic digital sensor *************************************************
void digitalSensor() { //called by meten()

// if the sensor is high
  char b = switchHL[3];
      if (digitalRead(3) == HIGH) { // if the sensor is high
           if (b == '1' && value == 0) { // //switch on when 'high' is selected
                  value = 15;                  
                  switch_on_now(true, false, "digital sensor"); //meteen de lamp aan
                  //DebugPrintln("ingeschakeld door digital sensor");
                  //event = 15;

                  return;
           }
          if (b == '0' && value > 0) { //switch off when 'low' is selected
                   switch_off_now(true, false, "digital sensor"); //meteen de lamp uit en mqtt message
                  //event = 35;
                  return;
                 }
           
      } else { // sensor is low
           if (b == '0' && value == 0) { //switch on when 'high' is selected
                  value = 15;                 
                  switch_on_now(true, false, "digital sensor"); //meteen de lamp aan
                  return;
           }
          if (b == '1' && value > 0) { //switch off when 'low' is selected
                  switch_off_now(true, false, "digital sensor"); //meteen de lamp uit en mqtt message
                  return; 
                 }
       }
 }
#endif


#ifdef SENSORS
bool donker() {
// als sensor[0] == '5' dan is het altijd donker want dan moet ie altijd werken
//if (sensor[0] == '5' ) {return 1;}
if ( sunrise < now() && now() < sunset ) {
return 0;
}else {
return 1;   
}
}
bool boodschap() { //boodschap bij 0(cond), 1(cond), 2, 3 en 4 altijd
  if ( BS[0] == '0' || BS[0] == '1' ) {
     if ( donker() ) { return 1; } else { return 0; }
    } else { // 2 3 4 altijd boodschap
       return 1;
    } // altijd boodschap
}
bool bwswitch() { // lamp moet aan bij 0(cond), 2(cond) en 3(altijd)
  if ( BS[0] == '0' || BS[0] == '2' ) { 
      if ( donker() ) { return 1; }  
  } else {
    if ( BS[0] == '3' ) {return 1;} // altijd lamp
  }
  // als we hier komen dan is niet aan de vorige condities voldaaan dus niet schakelen
  return 0; // opties 0 of 2 en licht,  en opties 1 en 4
}
#endif

void checkTimers() {
      for (int i = 0; i < TIMERCOUNT; i++) {
        if ( hasSwitched[i] ) { // er was door een timer ingeschakeld
        mustSwitch[i] = false; // timer nul of 1 2 3 4 5 6 7  uitgeschakeld zodat deze niet opnieuw inschakelt. 
        hasSwitched[i] = false;
      }
   }   
}
// ********************************************************************************************
//   T H E   A C T U A L  S W I T C H I N G
//*********************************************************************************************
// this function switches on and sends the mqtt messages an updates the log
void switch_on_now(bool zend, bool check, String logID) {
  inschakeltijdstip = now();
    if ( digitalRead(RELAY_PIN) != SWITCH_AAN ) {
        digitalWrite(RELAY_PIN, SWITCH_AAN); // schakel de lamp in als die uit was
        //value = 1; determined by the interface that cals this function 
        // 1=button 2=webinterface 3=external 4 = mqtt 5=timer0 6=timer1
        if( zend ) { mqttSwitchupdate(); }// mqtt message niet polled wel geschakeld 
        if( check ) {checkTimers();} // een eventueel ingeschakelde timer disarmen
        if(logID != "") Update_Log(logID, "on"); 
    }

}
// this function switches off and sends the mqtt messages an updates the log
void switch_off_now(bool zend, bool check, String logID) {
    if ( digitalRead(RELAY_PIN) != SWITCH_UIT ) {
        digitalWrite(RELAY_PIN, SWITCH_UIT); // schakel de lamp uit
        value = 0;
        if( zend ) { mqttSwitchupdate(); }// mqtt message niet polled wel geschakeld
        if( check ) {checkTimers();} // een eventueel ingeschakelde timer disarmen
        if(logID != "") Update_Log(logID, "off");  
    }
}

void write_eeprom() {
    EEPROM.begin(24);
  //struct data
  struct { 
    char str[16] = "";
    int haha = 0;
  } data;

 strcpy( data.str, requestUrl.c_str() ); 
 data.haha = tKeuze;
 //Serial.println("data.str na strcpy = " + String(data.str));
 //Serial.println("data.haha = " + String(data.haha));
    EEPROM.put(0, data);
    EEPROM.commit();

}
void read_eeprom() {
    EEPROM.begin(24);

  struct { 
    char str[16] = "";
    int haha = 0;
  } data;

EEPROM.get(0, data);
//Serial.println("read value from  is " + String(data.str));
requestUrl=String(data.str);
tKeuze = data.haha;
EEPROM.commit();

}  
