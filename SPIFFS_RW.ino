// ******************   spiffs lezen  *************************

// als er geen spiffs bestand is dan moet hij eigenlijk altijd een ap openenen
void SPIFFS_read() {

  DebugPrintln("mounting FS...");
 if (LittleFS.begin()) {
    DebugPrintln("mounted file system");
// we formatteren als nodig
if (!LittleFS.exists("/check.100")) {
  LittleFS.format();
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>(); 
    json["hahaha"] = event;         
    File configFile = LittleFS.open("/check.100", "w");
    if (!configFile) {
    DebugPrintln("failed to open config file for writing");
    }
    DebugPrintln("check file in SPIFFS geschreven");
    #ifdef DEBUG 
    serializeJson(json, Serial);
    Serial.println(""); 
    #endif
    serializeJson(json, configFile);
    configFile.close();
}
       if( file_open_for_read("/timerconfig.json") ) {     
             DebugPrintln("timerconfig read");
          } else {
          DebugPrintln("timerconfig.json not loaded");
        }
       if( file_open_for_read("/basisconfig.json") ) {     
             DebugPrintln("basisconfig read");
          } else {
          DebugPrintln("basisconfig.json not loaded");
        }  
       if( file_open_for_read("/mqttconfig.json") ) {     
             DebugPrintln("mqttconfig read");
          } else {
          DebugPrintln("mqttconfig.json not loaded");
        }
// we lezen deze als laatste omdat de bool zomerTijd wordt gecorrumpeerd. in mqttconfig is ook een bool
       if( file_open_for_read("/wificonfig.json") ) {
                DebugPrintln("wificonfig read");
          } else {
             DebugPrintln("wificonfig.json not loaded");
          }

  } else {
        DebugPrintln("failed to mount FS");
   }
 // einde spiffs lezen 3 bestanden

// als er geen swName of zomerTijd is gelezen dan zetten we deze op default
      if (swName[0] == '\0') {
           String s = "ESP-" + String(ESP.getChipId());
           s.toCharArray(swName, 21);
              }      
 
 tijd_convert(); // omrekenen van tijdwaarden voor cdwn aso en switchtime

}  


// **************************************************************************** 
//                      de gegevens opslaan in SPIFFS                         *  
// ****************************************************************************
void wifiConfigsave() {
   DebugPrintln("saving config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();   
    json["ip"] = static_ip;
    json["pswd"] = pswd;
//    json["lengte"] = lengte;
 //   json["breedte"] = breedte;
    json["longi"] = longi;
    json["lati"] = lati;
    json["timezone"] = timezone;
    json["zomerTijd"] = zomerTijd; 

    File configFile = LittleFS.open("/wificonfig.json", "w");
    if (!configFile) {
      DebugPrintln("failed to open config file for writing");
    }
    DebugPrintln("wificonfig.json in SPIFFS geschreven");
    #ifdef DEBUG 
//    json.printTo(Serial);
    serializeJson(json, Serial);
    Serial.println(""); 
    #endif
//    json.printTo(configFile);
    serializeJson(json, configFile);
    configFile.close();
}

void timerConfigsave() {   
DebugPrintln("saving config");
//    DynamicJsonBuffer jsonBuffer;
//    JsonObject& json = jsonBuffer.createObject();
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();
    json["timerActive"] = timerActive;
    json["weekDag"] = weekDag;
    json["relToSunOn"] = relToSunOn;
    json["relToSunOff"] = relToSunOff;   
    json["switchOn"] = switchOn;   
    json["switchOff"] = switchOff;  

 #ifdef SENSORS
    json["switchcdwn"] = switchcdwn;
    json["switchTemp"] = switchTemp;
    json["switchHL"] = switchHL;
    json["switchMoist"] = switchMoist;
    json["switchLicht"] = switchLicht;
    json["BS"] = BS;
 #endif    
    File configFile = LittleFS.open("/timerconfig.json", "w");
    if (!configFile) {
      DebugPrintln("failed to open timerconfig file for writing");
    }
    DebugPrintln("timerconfig.json in SPIFFS geschreven");
    #ifdef DEBUG 
    serializeJson(json, Serial);
    Serial.println("");     
    #endif
    serializeJson(json, configFile);
    configFile.close();
}
void basisConfigsave() {
   DebugPrintln("saving basis config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>(); 
    json["swName"] = swName;
    json["userPasswd"] = userPasswd;
    json["aso"] = aso;

//#ifdef MINI
//    json["SCHAKELAAR"] = SCHAKELAAR;
//    #endif    
#ifdef SENSORS
    json["timer"] = timer;
    json["sensor"] = sensor;
    json["tempCal"] = tempCal; 
    json["meetRes"] = meetRes;   

#endif 
    
    File configFile = LittleFS.open("/basisconfig.json", "w");
    if (!configFile) {
      DebugPrintln("failed to open config file for writing");
    }
    DebugPrintln("basisconfig.json in SPIFFS geschreven");
    #ifdef DEBUG 
    serializeJson(json, Serial);
    Serial.println("");     
    #endif
    serializeJson(json, configFile);
    configFile.close();
}

void mqttConfigsave() {
   DebugPrintln("saving mqtt config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>(); 
    json["mqttEnabled"] = mqttEnabled;
    json["mqttBroker"] = mqttBroker;
    json["mqttIntopic"] = mqttIntopic;
    json["mqttOuttopic"] = mqttOuttopic;
//    json["Mqtt_Username"] = Mqtt_Username;
//    json["Mqtt_Password"] = Mqtt_Password;
//    json["mqttPort"] = mqttPort;
    json["idxSwitch"] = idxSwitch;
    json["idxSensor"] = idxSensor;
    
    File configFile = LittleFS.open("/mqttconfig.json", "w");
    if (!configFile) {
      DebugPrintln("failed to open config file for writing");
    }
    DebugPrintln("mqttconfig.json in SPIFFS geschreven");
    #ifdef DEBUG 
    serializeJson(json, Serial);
    Serial.println("");     
    #endif
    serializeJson(json, configFile);
    configFile.close();
}


bool file_open_for_read(String bestand) {
//bool checkBool = false;
      DebugPrint("we are in in file_open_for_read, bestand = "); DebugPrintln(bestand); 
      if (LittleFS.exists(bestand)) {
      //file exists, reading and loading
      DebugPrintln("file exists");
        File configFile = LittleFS.open(bestand, "r");
        if (configFile) {
           DebugPrint("opened config file "); DebugPrintln(bestand);
           size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
           std::unique_ptr<char[]> buf(new char[size]);
           configFile.readBytes(buf.get(), size);

           DynamicJsonDocument doc(1024);
           auto error = deserializeJson(doc, buf.get());
           #ifdef DEBUG 
           serializeJson(doc, Serial); Serial.println("");
           #endif
             if (!error) {
              DebugPrintln("\nparsed json");
              String jsonStr = ""; // we printen het json object naar een string
            // nu kunnen we eerst controleren of een bepaalde entry bestaat
            // zoniet slaan we die over anders crasht de ESP
            serializeJson(doc, jsonStr); // zet het opbject in een string jsonStr
            if (bestand == "/wificonfig.json") {
                      if(jsonStr.indexOf("ip") > 0){ strcpy(static_ip, doc["ip"]);}
                      if(jsonStr.indexOf("pswd") > 0){ strcpy(pswd, doc["pswd"]);}
//                      if(jsonStr.indexOf("lengte") > 0){ strcpy(lengte, doc["lengte"]);}
//                      if(jsonStr.indexOf("breedte") > 0){ strcpy(breedte, doc["breedte"]);}
                      if(jsonStr.indexOf("longi") > 0){longi = doc["longi"].as<float>();}
                      if(jsonStr.indexOf("lati") > 0){lati = doc["lati"].as<float>();}                      
                      if(jsonStr.indexOf("timezone") > 0){ strcpy(timezone, doc["timezone"]);}
                      if(jsonStr.indexOf("zomerTijd") > 0){zomerTijd = doc["zomerTijd"].as<bool>();}
// Serial.println("spiffs read zomerTijd = " + String(zomerTijd) );
// if(zomerTijd) checkBool = true;
// Serial.println("checkBool na read wificonfig = " + String(checkBool) );
            } else
            if ( bestand == "/timerconfig.json" ) {

//                      if(jsonStr.indexOf("timerActive") > 0){timerActive = doc["timerActive"].as<array>();}
                      if(jsonStr.indexOf("timerActive") > 0){ strcpy(timerActive, doc["timerActive"]);}                      
                      if(jsonStr.indexOf("weekDag") > 0){ strcpy(weekDag, doc["weekDag"]);}
                      if(jsonStr.indexOf("relToSunOn") > 0){ strcpy(relToSunOn, doc["relToSunOn"]);}
                      if(jsonStr.indexOf("relToSunOff") > 0){ strcpy(relToSunOff, doc["relToSunOff"]);}
                      if(jsonStr.indexOf("switchOn") > 0){ strcpy(switchOn, doc["switchOn"]);}        
                      if(jsonStr.indexOf("switchOff") > 0){ strcpy(switchOff, doc["switchOff"]);}
            #ifdef SENSORS          
                     if(jsonStr.indexOf("switchcdwn") > 0)  { strcpy(switchcdwn, doc["switchcdwn"]);}
                     if(jsonStr.indexOf("switchTemp") > 0) { strcpy(switchTemp, doc["switchTemp"]);}
                     if(jsonStr.indexOf("switchHL") > 0  ){ strcpy(switchHL, doc["switchHL"]);}
                     if(jsonStr.indexOf("switchMoist") > 0)   { strcpy(switchMoist, doc["switchMoist"]);}
                     if(jsonStr.indexOf("switchLicht") > 0)   { strcpy(switchLicht, doc["switchLicht"]);}
                     if(jsonStr.indexOf("BS") > 0  ){ strcpy(BS, doc["BS"]);}
             #endif   
//Serial.println("spiffs read na timerconfig zomerTijd = " + String(zomerTijd) );
//Serial.println("checkBool na read timerconfig = " + String(checkBool) );

            } else

            if (bestand == "/basisconfig.json"){
                     if(jsonStr.indexOf("swName") > 0){strcpy(swName, doc["swName"]);}
                     if(jsonStr.indexOf("userPasswd") > 0){ strcpy(userPasswd, doc["userPasswd"]);}
                     if(jsonStr.indexOf("aso") > 0){ strcpy(aso, doc["aso"]);}

            #ifdef SENSORS
                     if(jsonStr.indexOf("timer") > 0){ strcpy(timer, doc["timer"]);}
                     if(jsonStr.indexOf("sensor") > 0){ strcpy(sensor, doc["sensor"]);} 
                     if(jsonStr.indexOf("tempCal") > 0){ strcpy(tempCal, doc["tempCal"]);}
                     if(jsonStr.indexOf("meetRes") > 0){ meetRes = doc["meetRes"].as<int>();}       
            #endif         
//Serial.println("spiffs read na basisconfig zomerTijd = " + String(zomerTijd) );
//Serial.println("checkBool na read basisconfig = " + String(checkBool) );

            } else

            if (bestand == "/mqttconfig.json"){
               if(jsonStr.indexOf("mqttEnabled") > 0)  { mqttEnabled =  doc["mqttEnabled"].as<bool>();}   
               if(jsonStr.indexOf("mqttBroker") > 0)   { mqttBroker =   doc["mqttBroker"].as<String>();}
               if(jsonStr.indexOf("mqttIntopic") > 0)  { mqttIntopic =  doc["mqttIntopic"].as<String>();}         
               if(jsonStr.indexOf("mqttOuttopic") > 0) { mqttOuttopic = doc["mqttOuttopic"].as<String>();}         
               if(jsonStr.indexOf("idxSwitch") > 0)     { idxSwitch = doc["idxSwitch"].as<int>();}
               if(jsonStr.indexOf("idxSensor") > 0)     { idxSensor = doc["idxSensor"].as<int>();}     

//Serial.println("spiffs read na timerconfig zomerTijd = " + String(zomerTijd) );
//Serial.println("checkBool na read timerconfig = " + String(checkBool) );
            }             
            return true; // if ! error
           } else {
            return false;
           }
     }
  }  
}

        
