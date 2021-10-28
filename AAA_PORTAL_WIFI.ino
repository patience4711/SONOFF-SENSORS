void setupPortal() {  
  digitalWrite(led_onb, LED_AAN);
  const char* apNaam = swName;
  const char* apPasswd = "123456789";
  if (event==101) Serial.println("restarting softAP");
  WiFi.softAP(apNaam);
  if (event != 101){
    DebugPrintln("\n scan start");
    scanWifi();
   }
}
void setupServer() {  
class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {
  server.on("/index", handlePortalRoot);
  server.on("/home", handlePortalRoot);
  server.on("/fwlink", handlePortalRoot);
  server.on("/redirect", handlePortalRoot);
  server.on("/wifiForm", handleForm);
  server.on("/wifiCon", handleConnect); //in wifiform there is a form with action wifiForm
  server.on("/wifiConfirm", handleConfirm); //in wifiform there is a form with action wifiForm 
  server.on("/wifiClose", handleClose);   
  server.on("/FS_ERASE", eraseFiles);
  server.on("/STATIC_ERASE", eraseStatic);
    }
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }
  void handleRequest(AsyncWebServerRequest *request) {
    //toSend = "<html><body><br><br><center><h1><a href='http://192.168.4.1/index'>GO TO WIFI PORTAL</a></html>";
    //request->send(200, "text/html", toSend);
    handlePortalRoot(request);
  }
};
  
  // configure the server  
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); //only when requested from AP  
  server.begin();
}


void start_portal() {
// start the dnsserver to make all captive
  IPAddress apIP(192, 168, 4, 1);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  
  setupPortal();
  setupServer();

  DebugPrintln("entering the infinitive loop with heartbeat");
  laatsteMeting = millis(); //voor de time-out
 
  // ***************************************************************************
  //              this is the infinitive  loop 
  //****************************************************************************

  static unsigned long heartbeat = 0;
  while (millis() < laatsteMeting + 300*1000UL) { // 5 minuten== 300 30 == 30sec
  if ( millis() > heartbeat + 10*1000UL ) {
    heartbeat = millis(); // elke 10 sec een heartbeat
    //Serial.print("a ");
  }
  if (tryConnectFlag) { // there are credentials provided
      if (wifiConnect())  {
        // can we restart the portal here to restore the lost connection after connecting????
        // in wifiConnect() we preserved the ip in the string maan
        //setupPortal();
        //setupServer();
        
        }  else {
        } // what to do now
   // we try to connect once so make the  flag false
   // tryConnectFlag = false; is done in wifiConnect   
  }
  
  if(Serial.available()) { // make the serial monitor work
    handle_Serial();
   }
   if (actionFlag == 11 || actionFlag == 10 ) break; // if the button is pressed
    
    dnsServer.processNextRequest();

  } 
  // ************************ end while loop ******************************* 

  //we only are here after a timeout. or if we clicked close      
    DebugPrintln("portal timed out, resetting...");
    delay(1000);
    ESP.restart();
}


void handlePortalRoot(AsyncWebServerRequest *request) {
//  always as we are here, portalstart is updated, 
//  so when there is activity in the portal we won't time out
unsigned long laatsteMeting = millis(); // update portalstart

// This function displays the main page of the portal.
// depending on the status of event we display messages or buttons

//Serial.println("we are in handlePortalRoot, event = " + String(event));
toSend = FPSTR(PORTAL_HEAD);
toSend += FPSTR(PORTAL_HOMEPAGE);

toSend.replace("{ma}", String(WiFi.macAddress()) );
//
      // when event = 101 we adapt the page with new data
      if (event == 101) {  // the 2e time that we are here we are connected 101 or not 100
          toSend.replace("hadden", "hidden"); // verberg de configuratieknop
          toSend.replace("close' hidden", "close'"); // show the close button
          String page = "";
          page += F("<div class=\"msg\">");
          page += F("connected to network <strong> unset<a>");
          page.replace("unset", WiFi.SSID());
          page += F("</strong><br><br>");
          page += F("The IP address is <strong><a href=\"http://");
          page += maan;
          page += F("\">");
          page += maan;
          page += F("</a></strong><br><br>");
          page += F("<h3 style='color:#FF0000';>Note down the ip-address and click \"close\"!</h3>");
      toSend.replace("<strong>Currently not connected to a wifi network!</strong>" , page);
      // show the close
      toSend.replace("close' hidden", "close'");
      }
      if (event==100) {  // when not connected
          String page = "";
          page+=F("<span style='color:red;'>Your attempt to connect to the network has failed!</span><br><br>");
          page+=F("Try again, click on wifi configuration!"); 
          toSend.replace("Currently not connected to a wifi network!" , page);
      }
      // hadden hudden hodden hedden
      // verberg de wis knop als gewist en wanneer verbonden
      if ( LittleFS.exists("/basisconfig.json") && event != 101 ) { //show if file is present
       toSend.replace("erase' hidden", "erase'");
      
      }  
       //deze laten we zien als niet 101 en wel static ip
      if( static_ip[0] != '\0' && static_ip[0] != '0' && event!=101) { //show if static is present
       toSend.replace("static' hidden", "static'"); 
      }

       request->send(200, "text/html", toSend); //send the html code to the client
 
       //event = 0; //for the next round 
}

// *********************************************************************
//                          pagina voor invoeren credentials
// *********************************************************************
void handleForm(AsyncWebServerRequest *request) {
   laatsteMeting = millis();  // update the timeout
   toSend = FPSTR(PORTAL_HEAD);
   toSend += FPSTR(PORTAL_WIFIFORM);
   toSend.replace("{pw}",  String(pswd) );

if (networksFound == 0) {
      toSend.replace("aplijst" ,"WiFi scan not found networks. Restart configuration portal to scan again.");
    } else {
      // can we do this with only one scan at the first time the portal was started?
      DebugPrintln("make a list of found networks");
      String lijst = makeList(networksFound);
      DebugPrintln("lijst = "); 
      DebugPrintln(lijst);
    toSend.replace("aplijst", lijst);  
    }

  request->send(200, "text/html", toSend); //send the html code to the client
}

 void handleConnect(AsyncWebServerRequest *request) {
  DebugPrintln("/wifiCon requested");

  // this function saves the password and takes care for connectin
  laatsteMeting = millis(); //om de timeout te verversen
//      grab the serverparameters
strcpy( ssid, request->getParam("s")->value().c_str() );
strcpy( pass, request->getParam("p")->value().c_str() );  
strcpy( pswd, request->getParam("pw")->value().c_str() );

  wifiConfigsave(); // save the admin passwd
 
  tryConnectFlag = true;  // this takes care for the connectattempts in the loop
    
AsyncWebServerResponse *response = request->beginResponse(200, "text/html", toSend);
   response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
   response->addHeader("Pragma", "no-cache");
   response->addHeader("Expires", "-1");
}

void handleConfirm(AsyncWebServerRequest *request) {
  toSend = FPSTR(PORTAL_HEAD);
  toSend += FPSTR(PORTAL_CONFIRM);
  request->send(200, "text/html", toSend);
}

// **************************************************************************
//               C L O S I N G  T H E  P O R T A L 
// **************************************************************************

void handleClose(AsyncWebServerRequest *request) {
   laatsteMeting = millis();
  String toSend = F("<!DOCTYPE html><html><head>");
  toSend += F("<script type='text/javascript'>setTimeout(function(){ window.location.href='/'; }, 1000 ); </script>");
  toSend += F("</head><body><br><br><center><h1>The esp is going to restart.!<br><br>Do not forget to note down the ip address!!<br><br>");
  toSend += F("The ip address is : ");
  toSend += WiFi.localIP().toString();
  toSend += F("</h2></body></html>");
  request->send(200, "text/html", toSend);
  actionFlag = 11; // set a flag for restart
}

// **************************************************************************
//               E R A S E    F I L E S Y S T E M  
// **************************************************************************
void eraseFiles(AsyncWebServerRequest *request) {
  DebugPrintln("LittleFS wipe and format");
  String toSend = F("<!DOCTYPE html><html><head>");
  toSend += F("<script type='text/javascript'>setTimeout(function(){ window.location.href='/redirect'; }, 1000 ); </script>");
  toSend += F("</head><body><h2>erasing the filesystem, please wait... !</h2></body></html>");
  request->send ( 200, "text/html", toSend ); //zend bevestiging

     if (LittleFS.exists("/wifiConfig.conf")) { LittleFS.remove("/wificonfig.json"); } 
     if (LittleFS.exists("/mqttconfig.json")) { LittleFS.remove("/mqttconfig.json"); }
     if (LittleFS.exists("/basisconfig.json")) { LittleFS.remove("/basisconfig.json"); }
     if (LittleFS.exists("/timerconfig.json")) { LittleFS.remove("/timerconfig.json"); }
     LittleFS.format();
     DebugPrintln("We gaan LittleFS wissen");
}

void eraseStatic(AsyncWebServerRequest *request) {
   DebugPrintln("static ip wissen");
  String toSend = F("<!DOCTYPE html><html><head>");
  toSend += F("<script type='text/javascript'>setTimeout(function(){ window.location.href='/redirect'; }, 500 ); </script>");
  toSend += F("</head><body><h2>erasing ip settings, please wait... !</h2></body></html>");
  static_ip[0] = '\0';
  static_ip[1] = '\0';
  wifiConfigsave();  
  request->send ( 200, "text/html", toSend ); //zend bevestiging
}

// **************************************************************************************
//                  P O R T A L  H E L P F U N C T I O N S
// **************************************************************************************

//**********************************************************************
//      try to connect
// **********************************************************************
bool wifiConnect() {
  // we are here because bool tryConnectFlag is true in the loop
      digitalWrite(led_onb, LED_UIT);
       
       tryConnectFlag=false;
       laatsteMeting = millis();

      DebugPrintln("we are in wifiConnect");

      WiFi.mode(WIFI_AP_STA);
      delay(500);
      Serial.println("Connecting to " + String(ssid));
      Serial.println("password =  " + String(pass));

      if (connectWifi() == 1) {
         ledblink(3, 200); 
         // imediately restart the portal so that there is no disconnection
         event=101; 
         setupPortal();
         DebugPrintln("yoepy, connected");
         maan = WiFi.localIP().toString();
       } else {
         DebugPrintln("could not connect, try again");
         digitalWrite(led_onb, LED_AAN); // 
         event=100;
        } 
 }

// ************************************************************************
//          try to connect with new credentials
// ************************************************************************
int connectWifi() {  
  DebugPrintln("connect with new credentials");
  if (ssid != "") {
    DebugPrintln("new ssid, trying to connect to that");
    //trying to fix connection in progress hanging
    ETS_UART_INTR_DISABLE();
    wifi_station_disconnect();
    ETS_UART_INTR_ENABLE();
    WiFi.begin(ssid, pass);

  } else { 
    // we don't have new ssid, so we see if there is one saved
    if (WiFi.SSID().length() > 0) {
      DebugPrintln("no new, but there are saved credentials, use these");
      //trying to fix connection in progress hanging
      ETS_UART_INTR_DISABLE();
      wifi_station_disconnect();
      ETS_UART_INTR_ENABLE();
      WiFi.begin();
    }
  }
  int connectAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print("*");
     connectAttempts += 1;
     if (connectAttempts == 10) {break;}
  }
   //Serial.println("\nwe are out of the for=loop, event = " + String(event) );

   if(connectAttempts < 10 ) {
      DebugPrint ("Connected : ");
      DebugPrintln(WiFi.localIP());
      checkFixed(); // set static ip if configured
      return 1;
   } else {
     DebugPrint ("NOT Connected : ");
     return 0;
   }
}


// ************************************************************************
//          S C A N   W I F I  F U N C T I O N S
// ************************************************************************
void scanWifi() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  DebugPrintln("scan done");
  if (n == 0) {
    DebugPrintln("no networks found");
  } else {
    DebugPrint(n);
    DebugPrintln(" networks found");
    //for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      //Serial.print(i + 1);
      //Serial.print(": ");
      //Serial.print(WiFi.SSID(i));
      //Serial.print(" (");
      //Serial.print(WiFi.RSSI(i));
      //Serial.print(")");
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      //delay(10);
    //}
  }
  //Serial.println("");
  networksFound = n;
  //return n; // # of found networks
  }

// ********************************************************
//      zet de gevonden netwerken in een string
// ********************************************************
String makeList(int aantal) { // aantal is het aantal gevonden netwerken

//eerst sorteren
      int indices[aantal];
      for (int i = 0; i < aantal; i++) {
        indices[i] = i;
      }

      // RSSI SORT

      // old sort
      for (int i = 0; i < aantal; i++) {
        for (int j = i + 1; j < aantal; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }   
   
  String lijst="";
  for (int i = 0; i < 8; i++) {

        String item = FPSTR(PORTAL_LIST);
        String rssiQ;
        //int quality = (WiFi.RSSI(i));
        int quality = WiFi.RSSI(indices[i]);
        DebugPrint("quality = "); 
        DebugPrintln(quality);
        // -50 is groter dan -80
        if (quality < -99){ continue; } // skip als kleiner dan -65 bijv -66
//        item.replace("{v}", WiFi.SSID(i));
        item.replace("{v}", WiFi.SSID(indices[i]));        
        item.replace("{r}", String(quality));
        item.replace("{i}", "");
        lijst += item;
  }
return lijst;  
}  
