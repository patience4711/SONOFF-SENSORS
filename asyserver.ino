// ***********************************************************************************
void start_asyserver() {

// Simple Firmware Update
  server.on("/FWUPDATE", HTTP_GET, [](AsyncWebServerRequest *request){
    loginBoth(request, "admin");
    request->send_P(200, "text/html", UPDATE_FORM); 
    });
  server.on("/handleFwupdate", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("FWUPDATE requested");
    if( !Update.hasError() ) {
    toSend="<br><br><center><h2>UPDATE SUCCESS !!</h2><br><br>";
    toSend +="click here to reboot<br><br><a href='/REBOOT'><input style='font-size:4vw;' type='submit' value='REBOOT'></a>";
    } else {
    toSend="<br><br><center><kop>update failed<br><br>";
    toSend +="click here to go back <a href='/FWUPDATE'>BACK</a></center>";
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", toSend);
    response->addHeader("Connection", "close");
    request->send(response);
  
  },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    //Serial.println("filename = " + filename);
    if(filename != "") {
    if(!index){
      Serial.printf("start firmware update: %s\n", filename.c_str());
      Update.runAsync(true);
      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
        Update.printError(Serial);
      }
    }
    } else {
      Serial.println("filename empty, aborting");
//     Update.hasError()=true;
    }
    if(!Update.hasError()){
      if(Update.write(data, len) != len){
        Serial.println("update failed with error: " );
        Update.printError(Serial);
      }
    }
    if(final){
      if(Update.end(true)){
        Serial.printf("firmware Update Success: %uB\n", index+len);
      } else {
        Update.printError(Serial);
      }
    }
  });
// ***********************************************************************************
//                                     homepage
// ***********************************************************************************
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
 
    DebugPrintln("homepage requested");
    loginBoth(request, "both");
    toSend = FPSTR(HTML_HEAD);
    toSend.replace("tieTel", swName ); 
    toSend += FPSTR(HOMEPAGE);
    request->send(200, "text/html", toSend);
});

server.on("/SW=BACK", HTTP_GET, [](AsyncWebServerRequest *request) {
    //if (!request->authenticate("admin", pswd) && !request->authenticate("user", userPaswd)  ) return request->requestAuthentication();
    loginBoth(request, "both");
    //    zendPageHome();
    //    request->send(200, "text/html", toSend);
    Serial.println("de requestUrl = " + requestUrl);
    request->redirect( requestUrl );
    //request->send(302, "text/plane", "");

});

server.on("/STYLESHEET", HTTP_GET, [](AsyncWebServerRequest *request) {
   request->send_P(200, "text/css", STYLESHEET);
});

server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("favicon requested");
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", FAVICON, FAVICON_len);
    request->send(response);
});
// ***********************************************************************************
//                                    timers
// ***********************************************************************************
server.on("/TIMERCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    requestUrl = request->url(); // remember this to come back after reboot
    loginBoth(request, "admin");
    zendPageTimers();
    request->send(200, "text/html", toSend);
});

server.on("/timerconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleTimerconfig(request);
    //request->send(200, "text/html", toSend);
});

server.on("/TIMER", HTTP_GET, [](AsyncWebServerRequest *request) {
    int i = atoi(request->arg("welke").c_str()) ;
    tKeuze = i;
    DebugPrint("tKeuze is "); DebugPrintln(i);
    //nu roepen we zendpageRelevant aan
    zendPageRelevantTimers(); // prepare the page
    request->send(200, "text/html", toSend); //send the html code to the client
});


// ***********************************************************************************
//                                   basisconfig
// ***********************************************************************************
server.on("/BASISCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    requestUrl = request->url();// remember this to come back after reboot
    loginBoth(request, "admin");
    zendPageBasis();
    request->send(200, "text/html", toSend);
});

server.on("/basisConfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    //Serial.println("basisconfig requested");  
    handleBasisconfig(request);
    //request->send(200, "text/html", toSend);
});
// ***********************************************************************************
//                                     IP config
// ***********************************************************************************
server.on("/IPCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    loginBoth(request, "admin");
    zendPageIPconfig();
    request->send(200, "text/html", toSend);
});

server.on("/IPconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleIPconfig(request);
});

// ***********************************************************************************
//                 mqtt  geo  log
// ***********************************************************************************
server.on("/MQTT", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    loginBoth(request, "admin");
    requestUrl = request->url(); // remember this to come back after reboot
    zendPageMQTTconfig();
    request->send(200, "text/html", toSend);
});

server.on("/MQTTconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleMQTTconfig(request);
});

server.on("/GEOCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    loginBoth(request, "admin");
    requestUrl = request->url(); // remember this to come back after reboot
    zendPageGEOconfig();
    request->send(200, "text/html", toSend);
});

server.on("/geoconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println(F("geoconfig requested"));
handleGEOconfig(request);
});
server.on("/LOGPAGE", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    zendPageLog();
    request->send( 200, "text/html", toSend );
});
server.on("/CLEAR_LOG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    loginBoth(request, "admin");
    //Serial.println(F("clear log requested"));
    Clear_Log();
    zendPageLog();
    request->send( 200, "text/html", toSend );  
});

server.on("/REBOOT", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("reboot requested");
//  loginAdmin(request);
  loginBoth(request, "admin");
  actionFlag = 10;
  confirm();
  request->send(200, "text/html", toSend);
});

server.on("/STARTAP", HTTP_GET, [](AsyncWebServerRequest *request) {
//    if (!request->authenticate("admin", pswd)) return request->requestAuthentication();
        loginBoth(request, "admin");
        DebugPrintln("erase the wifi data");
        String toSend = F("<!DOCTYPE html><html><head><script type='text/javascript'>setTimeout(function(){ window.location.href='/'; }, 5000 ); </script>");
        toSend += F("</head><body><center><h2>OK the accesspoint is started.</h2>Wait unil the led goes on.<br><br>Then go to the wifi-settings on your pc/phone/tablet and connect to ESP-");
        toSend += String(ESP.getChipId()) + " !";
        request->send ( 200, "text/html", toSend ); //zend confirm
        actionFlag = 11;
        //request->redirect("/");
});

server.on("/INFOPAGE", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    //Serial.println(F("/INFORMATIE requested"));
    handleInfo(request);
});



server.on("/MQTT_TEST", HTTP_GET, [](AsyncWebServerRequest *request) {
    String toMQTT = "{\"test\":\"" + String(mqttOuttopic) + "\"}";
    Serial.println("MQTT_Client.publish the message : " + toMQTT);
    MQTT_Client.publish ( mqttOuttopic.c_str(), toMQTT.c_str() );
    toSend = "sent mqtt message : " + toMQTT;
    request->send( 200, "text/plain", toSend  );
});
#ifdef SENSORS
server.on("/SENSOR", HTTP_GET, [](AsyncWebServerRequest *request) {
    int i = atoi(request->arg("welke").c_str()) ;
    tKeuze = i;
    DebugPrint("tKeuze is "); DebugPrintln(i);
    //nu roepen we zendpageRelevant aan
    zendPageRelevantSensors(); 
    request->send(200, "text/html", toSend); //send the html code to the client
  });

server.on("/SENSORCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    requestUrl = request->url(); // remember this to come back after reboot
    zendPageSensors();
    request->send( 200, "text/html", toSend );
});

server.on("/sensorconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleSensorconfig(request);
    //request->send( 200, "text/html", toSend );
});

server.on("/METEN", HTTP_GET, [](AsyncWebServerRequest *request) {
    meetENschakel();
    confirm();
    request->send( 200, "text/html", toSend );
    });
#endif

server.on("/SW=OFF", HTTP_GET, [](AsyncWebServerRequest *request) {
     loginBoth(request, "both");
     switch_off_now(true, true, "web int");
     value = 0;
     request->send_P(200, "text/html", HOMEPAGE);
});
server.on("/SW=ON", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    value = 2; // for the update of the main page button
    switch_on_now(true, true, "web int");
    request->send_P(200, "text/html", HOMEPAGE);
});

// ********************************************************************
//                    X H T  R E Q U E S T S
//***********************************************************************
server.on("/get.currentTime", HTTP_GET, [](AsyncWebServerRequest *request) {
     String uur = String(hour());
     if(hour() < 10) { uur = "0" + String(hour()); } 
     String minuten = String(minute());
     if(minute() < 10) { minuten = "0" + String(minute()); }
     String json = "{";
     json += "\"uur\":\"" + uur + "\",\"min\":\"" + minuten + "\"}";
     request->send(200, "text/json", json);
     json = String();
});

server.on("/get.Homepagedata", HTTP_GET, [](AsyncWebServerRequest *request) {     
    // set the array into a json object
    int state = value;
    if(state > 1) state = 1;
    //if (value != 0)
    String json = "{";
    json += "\"name\":\"" + String(swName) + "\"";
    json += ",\"state\":\"" + String(state) + "\""; // can be 1 or 0
    json += "}";
     
    //Serial.println("get.Homepagedata reaction = " + json);
    request->send(200, "text/json", json);
    json = String();
});
// if everything failed we come here
server.onNotFound([](AsyncWebServerRequest *request){
  Serial.println("unknown request");
  handleNotFound(request);
});

server.begin(); 
}

void loginBoth(AsyncWebServerRequest *request, String who) {
  String authFailResponse = "<h2>login failed <a href='/'>click here</a></h2>";
  if (who == "admin" ){
  const char* www_realm = "login as administrator."; 
    if (!request->authenticate("admin", pswd)) return request->requestAuthentication(www_realm, authFailResponse);
  }
  if (who == "both" ){
  const char* www_realm = "login as administrator or user."; 
    if (!request->authenticate("admin", pswd) && !request->authenticate("user", userPasswd)) return request->requestAuthentication(www_realm, authFailResponse);
  }
}

void confirm() {
toSend="<html><body onload=\"setTimeout(function(){window.location.href='/';}, 3000 );\"><br><br><center><h3>processing<br>your request,<br>please wait</html>";
}
