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

//void zendUit() {
//if ( client.connected() ) {
//    DebugPrintln("zendUit(), we zijn verbonden");
//} else {
//     DebugPrintln("zendUit(), we moeten mqtt verbinden");         
//     mqtt_reconnect();
//}        
//
//if ( client.connected() ) {
//          if (client.publish(mqttTOPIC, (char*) mqttMess.c_str())) {
//            DebugPrint("Publish geslaagd, de message = "); DebugPrintln(mqttMess);
//          }
//          else {
//            DebugPrintln("Publish mislukt");
//          }
//    } else {
//    DebugPrintln("zendUit(), we konden niet verbinden, geen mqtt"); 
//    }
//}

// **********************************************************************
//                CALL BACK (ingekomen boodschappen)
// **********************************************************************

void MQTT_Receive_Callback(char* topic, byte* payload, unsigned int length) {

StaticJsonDocument <256> doc;
deserializeJson(doc,payload);
int swidx = doc["idx"].as<int>();
String cmd = doc["cmd"].as<String>();

if (swidx = idxSwitch ) {
  DebugPrintln("found my switchIDX");
     if ( cmd == "On" && digitalRead(RELAY_PIN)==SWITCH_UIT) {
         DebugPrintln("mqtt in: On gevonden");
         checkTimers();
         switch_on_now(false, false, "mqtt in"); // geen mqtt message
        }

     if ( cmd == "Off" && digitalRead(RELAY_PIN)==SWITCH_AAN) {
         DebugPrintln("mqtt in: Off gevonden");
         //checkTimers(); // maak een eventuele timer onschadelijk
         switch_off_now(false, true, "mqtt in"); //geen mqtt wel checkTimers
       } 
   }
}  


// *******************************************************************************
//                   mqtt message over de status van de switch
// *******************************************************************************
//void switchStatus(int beweging) {
//DebugPrintln("functie switchStatus");  
//String toMQTT;
//if (!mqttEnabled) { // als mqtt niet is ingesteld
//return;
//}
//
//if ( client.connected() ) {
//
//        if ( beweging == 1 ) {  //als de stastus van de lamp verandert dan sturen we een status update
//            toMQTT="{\"idx\":{idx},\"nvalue\":1}";
//        } else {
//            toMQTT="{\"idx\":{idx},\"nvalue\":0}";  
//        }
//        toMQTT.replace("{idx}" , String(idxSwitch));
//        MQTT_Client.publish ( mqttOuttopic.c_str(), toMQTT.c_str() );
//        
//    }   
//}
