const char MQTTCONFIG[] PROGMEM = R"=====(
<body>

<div id='msect'>
<div id='bo'></div>
  <div id='hlpd'>
  <span class='close' onclick='sl();'>&times;</span><h3>MOSQUITTO HELP</h3>
  Mosquitto can be used to send short messages back and forth to an mqtt server (broker).
  With these messages you can switch or update sensors. <br><br>
    <b>address:</b><br>The (ip) address of the mqtt-broker cq domoticz<br><br>
  <b>mqtt messages:</b><br>Outgoing sensor update: {"idx":123,"nvalue":0,"svalue":"123.45"}<br>
  outgoing switch update: {"command":"switchlight","idx":456,"switchcmd":"On or Off"}<br>
  Listens to json messages like {"idx":123,"cmd":"Off"}
  </div>
</div>
<div id='msect'>
  <kop>MOSQUITTO CONFIGURATION</kop>
</div>
<div id='main-sect'>
  <div class='divstijl'>
  <form id='fM' method='get' action='MQTTconfig' oninput='subMit();'>
  <center>
  <table>
  <tr><td style='width:130px;'>enable ?<td><input type='checkbox' style='width:30px; height:30px;' name='mqtEn' #check></input></tr> 
  <tr><td >address<td><input class='inp6' name='mqtAdres' value='{mqttAdres}' size='31' placeholder='broker adres'></tr>
  <tr><td >port<td><input class='inp2' name='mqtPort' value='{mqttPort}' size='31' placeholder='mqtt port' readonly></tr>
  <tr><td>receive topic:&nbsp<td><input class='inp6' name='mqtinTopic' value='{mqttinTopic}' placeholder='mqtt topic send' length='60'></tr>
  <tr><td>send topic:&nbsp<td><input class='inp6' name='mqtoutTopic' value='{mqttoutTopic}' placeholder='mqtt topic receive' length='60'></tr>
  <tr><td>idx switch:&nbsp<td><input class='inp2' name='mqidx1' value='{idx1}' size='4' length='4'></td></tr>
  <tr><td>idx sensor:&nbsp<td><input class='inp2' name='mqidx2' value='{idx2}' size='4' length='4'></td></tr>
 
  </td></table></form></center>
  </div>
</div>
<div id='msect'>
  <ul>
  <li id="sub"><a href="#" onclick='submitFunction("/SW=BACK")'>save </a></li>
  <li><a href='/BASISCONFIG'>done</a></li>
  <li><a href='#' onclick='hlpf()'>help</a></li>
  <li><a href='/MQTT_TEST' >test</a></li>
  </ul> 
  <br></div>
</div>
</center></body></html>
)=====";


void zendPageMQTTconfig() {
   DebugPrintln("we are at zendPageMQTTconfig");
toSend = FPSTR(HTML_HEAD);
toSend.replace("tieTel", swName );
toSend += FPSTR(MQTTCONFIG);  

  //altijd de mqtt gegevens terugzetten
 
if (mqttEnabled) { toSend.replace("#check", "checked");}
toSend.replace("{mqttAdres}", mqttBroker);
toSend.replace("{mqttPort}", mqttPort);
toSend.replace("{mqttinTopic}", mqttIntopic);
toSend.replace("{mqttoutTopic}", mqttOuttopic);
//toSend.replace("{mqtu}", Mqtt_Username );
//toSend.replace("{mqtp}", Mqtt_Password );
toSend.replace("{idx1}", String(idxSwitch));
toSend.replace("{idx2}", String(idxSensor) );
}

void handleMQTTconfig(AsyncWebServerRequest *request) {

  //collect serverarguments
  mqttBroker = request->arg("mqtAdres");
  //DebugPrint("mqttBroker na strcpy = "); DebugPrintln(mqttBroker); // oke

  mqttPort = request->arg("mqtPort");
  //DebugPrint("mqttPort = "); DebugPrintln(mqttPort); // ok

  mqttIntopic = request->arg("mqtinTopic");
  //DebugPrint("mqttIntopic na strcpy = "); DebugPrintln(String(mqttIntopic)); // oke

  mqttOuttopic = request->arg("mqtoutTopic");
  //DebugPrint("mqttOuttopic na strcpy = "); DebugPrintln(String(mqttOuttopic)); // oke
  
//  Mqtt_Username = request->arg("mqtUser");
//  DebugPrint("Mqtt_Username na strcpy = "); DebugPrintln(Mqtt_Username); // oke
//
//  Mqtt_Password = request->arg("mqtPas");
//  DebugPrint("Mqtt_Username na strcpy = "); DebugPrintln(Mqtt_Password); // oke

  idxSwitch = request->arg("mqidx1").toInt();
  idxSensor = request->arg("mqidx2").toInt();   
// de selectbox
   char tempChar[1] = "";
   String dag = request->arg("mqtEn");  // mqselect
   if ( dag == "on") { mqttEnabled = true; } else { mqttEnabled = false; }

  DebugPrintln("saved mqttconfig");
  mqttConfigsave();  // 
  actionFlag = 10; // takes care for reboot
}
