// **************************************************************************
//                           MQTT
// **************************************************************************

bool mqttConnect() {   // MQTT connection (documented way from AutoConnect : https://github.com/Hieromon/AutoConnect/tree/master/examples/mqttRSSI_NA)
    if ( mqttBroker[0] == '\0' || mqttBroker[0] == '0' ) {
      mqttEnabled = false; // we proberen het niet opnieuw
      DebugPrint("no broker, cancelling");
      return false;
    }
    DebugPrint("going to connect to mqtt broker "); DebugPrint(String(mqttBroker));
    DebugPrintln(" connecting");
    if (mqttPort == "" ) mqttPort = "1883";   // just in case ....
    uint8_t retry = 3;
    DebugPrintln(String("\nAttempting connect to MQTT broker: ") + String(mqttBroker));
    //We generate a unique name for this device to avoid mqtt problems in case if you have multiple RFlink-ESP devices
    String clientId = String(ESP.getChipId());
    while (!MQTT_Client.connected()) 
    {
     if (MQTT_Client.connect(clientId.c_str() )) 
     {
          DebugPrint(F("MQTT connection Established with ID : ")); //+ String(clientId));
          DebugPrintln(clientId);
          // ... and subscribe, send = MQTTtoRF
          //String topic = Mqtt_Topic;
          MQTT_Client.subscribe ( mqttIntopic.c_str() ) ;   // 
          DebugPrint("MQTT subscribed on topic "); DebugPrintln(mqttIntopic);
          Update_Log("mqtt", "connected"); 
          return true;
     }
     else 
     {
          DebugPrintln("Connection mqttserver:" + String(mqttBroker));
          DebugPrintln("Connection failed:" + String(MQTT_Client.state()));
          String term = "connection failed state: " + String(MQTT_Client.state());
          Update_Log("mqtt", term);
          if (!--retry)
            break;
          delay(200);
      }
  }
  return false;
}


// **********************************************************************
//                CALL BACK (ingekomen boodschappen)
// **********************************************************************
// the
void MQTT_Receive_Callback(char* topic, byte* payload, unsigned int length) {

StaticJsonDocument <320> doc;
deserializeJson(doc,payload);
int swidx = doc["idx"].as<int>();
int cmd = doc["nvalue"].as<int>();

if (swidx == idxSwitch ) {
  DebugPrintln("found my switchIDX");
     if ( cmd == 1 && digitalRead(RELAY_PIN)==SWITCH_UIT) {
         DebugPrintln("mqtt in: On gevonden");
         checkTimers();
         switch_on_now(false, false, "mqtt in"); // geen mqtt message
        }

     if ( cmd == 0 && digitalRead(RELAY_PIN)==SWITCH_AAN) {
         DebugPrintln("mqtt in: Off gevonden");
         checkTimers(); // maak een eventuele timer onschadelijk
         switch_off_now(false, true, "mqtt in"); //geen mqtt wel checkTimers
       } 
   }
}  

// ******************************************************************************************
//   mqtt message  state of the switch
// ******************************************************************************************

void mqttSwitchupdate() 
{
  if ( !mqttEnabled ){ 
    DebugPrintln("no valid mqtt address or not configured, skipping..");
    return;
  }    
  // update switch state in a json format
  StaticJsonDocument<256> doc;
  //doc["command"] = "switchlight";
  doc["idx"] = idxSwitch;
  //if (value == 0) { doc["switchcmd"] = "Off"; } else { doc["switchcmd"] = "On";}
  if (value == 0) { doc["nvalue"] = 0; } else { doc["nvalue"] = 1;}
  char out[64];
  int b =serializeJson(doc, out);
  MQTT_Client.publish ( mqttOuttopic.c_str(), out );
}

// ******************************************************************************************
//   mqtt message value of the sensor
// ******************************************************************************************
  #ifdef SENSORS
  void sendMqttsensor() {
  String mqttMess;
  if ( !mqttEnabled ){ 
    DebugPrintln("no valid mqtt address or not configured, skipping..");
    return;
  }

  // this function takes care for the transmission of the mqtt messages
  //if a sensor is polled we send an update

  mqttMess="";
  StaticJsonDocument<256> doc;
  doc["idx"] = idxSensor;
  doc["nvalue"] = 0;

  // we already have doc["idx"] = idxSensor and doc[nvalue]=0;
  // we have to add svalue
  String Mess = "";
           
      int hum_stat; // reken een getal uit voor de vochtigheids statitstiek
          if (sensor[0] == '2' || sensor[0] == '3'){
         // 0=normal 1 = comfortabel 2 = droog 3 = wet
                if ( humidity > 70 ) { //nat
                  hum_stat = 3;
                } else if ( humidity < 30 ) { // droog
                  hum_stat = 2; 
                } else if ( humidity >= 30 & humidity <= 45 ) { // normaal
                  hum_stat = 0;
                } else if ( humidity > 45 & humidity <= 70 ) { //comfortabel
                  hum_stat = 1;
                }
           }
        //
        if (sensor[0] == '3') { // de BME 280
        
        // we moeten een waarde berekenen voor de weersvoorspelling aan de hand van de luchtdruk  
         bar_for = 0; // 3=bewoklt 1 = mooi 2 = half bew 4 = regen
                if ( p > 1020.0 ) { //hoge luchtdruk dus mooi
                  bar_for = 1;
                } else if ( p < 1000.0 ) { // laag dus regen
                  bar_for = 4; 
                } else if ( p >= 1000.0 & p <= 1010.0 ) { // onder gem = bewolkt
                  bar_for = 3;
                } else if (p >= 1010.0 & p <= 1020.0 ) { // boven gem = half bew
                  bar_for = 2;
                }   
          Mess=String(temp_c,1) + ";" + String(humidity,1) + ";" + String(hum_stat) + ";" + String(p,1) + ";" + String(bar_for) + ";";        
        }
 
       if (sensor[0] == '2') 
       {
           Mess=String(temp_c,1) + ";" + String(humidity,1) + ";" + String(hum_stat) + ";";
       }

       if (sensor[0] == '1') 
       {
          Mess = String(temp_c,1) + ";";
       } 
       
       if (sensor[0] == '6') { //lightsensor sends lux
           Mess = String(p,1) + ";"; 
       }
       
       if(sensor[0]=='4' || sensor[0]=='7') //motion or digital
       { 
           Mess =  String(digitalRead(3));
       } 
   
   
      doc["svalue"] = Mess;
      DebugPrint("svalue in sendMqtt = "); 
      DebugPrintln(Mess);
    
    // we hebben nu de mqtt json en kunnen deze verzenden
      char out[64];
      int b =serializeJson(doc, out);
      MQTT_Client.publish ( mqttOuttopic.c_str(), out );
}
#endif
