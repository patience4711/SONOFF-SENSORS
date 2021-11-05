const char GEOCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
<div id='bo'></div>
  <div id='hlpd'>
  <span class='close' onclick='sl();'>&times;</span><h3>GEOGRAPHICAL SETTINGS HELP</h3>
  These data are necessary to determine the correct time and to calculate the times of sunset and sunrise.<br><br>
  <b>Latitude and longitude</b><br>
  The location of this device. (google)<br><br>
  <b>Timezone:</b><br>minutes offset relative to Greenwich Main Time. <br>This can be a "+" or a "-" value<br><br>
  <b>DST:</b><br>Check when daylight saving time applies.<br><br>
  </div>
</div>

<div id='msect'>
  <br><kop>GEOGRAPHICAL SETTINGS</kop><br><br>
</div>
<div id='msect'>
<div id='bo'></div>
  <div class='divstijl' style='height:62vh;'>
  <form id='fM' method='get' action='geoconfig' oninput='subMit();'>
  <center><table>
  <tr><td style='width:140px;'>longitude &deg<td><input  class='inp3' name='le' length='8' placeholder='lengtegraad' value='{le}'></input></tr> 
  <tr><td>latitude &deg<td><input class='inp3' name='be' length='8' placeholder='latitude' value='{be}'></input></tr> 
  <tr><td>timezone<td><input class='inp2' name='tz' length='6' placeholder='minutes relative to GMT' value='{tz}'></input></tr>
  <tr><td>dst y/n<td><input type='checkbox' style='width:30px; height:30px;' name='ts' #check></input></tr></table> 
  </div>
</div><br></form>
<div id='msect'>
<ul>
<li><a href='#' onclick='hlpf()'>help</a>
<li><a href='/BASISCONFIG'>done</a></li>
<li id="sub"><a href="#" onclick='submitFunction("/SW=BACK")'>save </a></li>
</ul>
</div>
<br></body></html>
)=====";



void zendPageGEOconfig() {
   DebugPrintln("we are on zendPageGeoconfig");
//loginAdmin(AsyncWebServerRequest *request);

toSend = FPSTR(HTML_HEAD);
toSend.replace("tieTel", swName );
toSend += FPSTR(GEOCONFIG);  

// en de gegevens terugzetten
//toSend.replace("{le}",  String(lengte) );
//toSend.replace("{be}",  String(breedte) );
toSend.replace("{le}",  String(longi,3) );
toSend.replace("{be}",  String(lati,3) );
toSend.replace("{tz}",  String(timezone) );

Serial.println("zendPageGEOconfig zomerTijd = " + String(zomerTijd) );
if (zomerTijd) { 
//  Serial.println("zomerTijd = true");
  toSend.replace("#check", "checked");
  } else {
//   Serial.println("zomerTijd = false");   
  }
}

void handleGEOconfig(AsyncWebServerRequest *request) {
//char static_ip2[16] = "";

  //de serverargumenten verzamelen
//strcpy(lengte, request->getParam("le")->value().c_str());
//strcpy(breedte, request->getParam("be")->value().c_str());

longi = request->getParam("le")->value().toFloat();
lati = request->getParam("be")->value().toFloat();

strcpy(timezone, request->getParam("tz")->value().c_str());

//BEWARE CHECKBOX
String dag = "";
if(request->hasParam("ts")) {
dag = request->getParam("ts")->value();  
}
   if (dag == "on") { zomerTijd = true; } else { zomerTijd = false;}
   
     //nu gaan we de confirmpage tonen
      DebugPrintln("we gaan een nieuwe webpage inlezen in toSend");

      //request->send(200, "text/html", toSend); //send the html code to the client
      DebugPrintln("GEOconfig opgeslagen");
     
      wifiConfigsave();

      actionFlag=10; // takes care for reboot 
} 
