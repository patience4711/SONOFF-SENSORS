#ifdef SENSORS
const char BASISCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
<div id='bo'></div>
  <div id='hlpd'>
  <span class='close' onclick='sl();'>&times;</span><h3>SETTINGS HELP</h3>
  <b>security:</b><br>if switched on manually, automatic switch off after this time.
  Default is 6 hours.<br><br>
    <b>switch name:</b><br>If you avoid spaces, the hostname will be reflected correctly by your router.
  <br><br><b>user passwd:</b><br>Grant housemates limited access with username  
  <span style='color:red;'>user</span> and the <span style='color:red;'>user passw.</span> 
  <br><br><b>active automation:</b><br>The desired behaviour of the automatic switching.
  <br>Eventually setup the connected sensor in the sensors menu. 
  </div>
</div>

<div id='msect'>
  <ul>
  <li><a href='/'>home</a>
  <li><a href='/INFOPAGE'>info</a></li>
  <li><a href='/SENSORCONFIG'>sensors</a></li>
  <li><a href='/TIMERCONFIG'>timers</a></li>
  <li><a href='/IPCONFIG'>wifi</a></li>
  <li><a href='/GEOCONFIG'>time</a></li>
  <li><a href='/MQTT'>mqtt</a></li>
  <li><a href='/FWUPDATE'>update</a></li>
  <li><a href='/REBOOT'>reboot</a></li>
  <li><a onclick="return confirm('are you sure?')" href='/STARTAP'>AP</a></li>
  </ul><br>
</div>
<div id='msect'>
<kop>GENERAL SETTINGS</kop>
</div>
<div id='msect'>
  <div class='divstijl' style='height:50vh;'>
    <form id='fM' method='get' action='basisConfig' oninput='subMit()'>
    <center><table>
    
    <tr><td style='width:140px;'>device name<td><input class='inp6' name='swname' maxlength='21' title='no spaces, see help' value='{nm}'></input></tr>
    <tr><td>user passwd<td><input  class='inp5' name='pw1' length='11' placeholder='max. 10 char' value='{pw1}' pattern='.{4,10}' title='betweeen 4 and 10 characters'></input></tr> 
    <tr><td>security<td><input class='inp2' name='bev' value='{bev}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm' ></input></tr> 
    
    <tr><td>active automation<td><select name='timer' id='timer' class='sb1'>
    <option value='0' tM0>none</option>
    <option value='1' tM1>on/off timers</option>
    <aption1 value='3' tM3>thermostate</option>
    <aption2 value='4' tM4>hygrostate</option>
    <aption3 value='6' tM6>light sensor</option>
    <aption4 value='7' tM7>digital sensor</option>
    </select></td></tr></table></form>
    </center>
  </div><br>
</div>
<br>
<div id='msect'>
  <ul>
  <li><a href='#' onclick='hlpf()'>help</a></li>
  <li><a href='/'>home</a>
  <li id="sub"><a href="#" onclick='submitFunction("/SW=BACK")'>save </a></li>
  </ul><br>
</div>
</body></html>
  )=====";

#else  // webpagina zonder sensors

const char BASISCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
  <div id='hlpd'>
  <span class='close' onclick='sl();'>&times;</span><h3>SETTINGS HELP</h3>
  <b>security:</b><br>if switched on manually or by the count up timer, there will be switched off after the set time.
  Default is 6 hours.<br><br>
    <b>switch name:</b><br>If you avoid spaces, the hostname will be reflected correctly by your router.
  <br><br><b>user passwd:</b><br>Grant housemates limited access with username  
  <span style='color:red;'>user</span> and the <span style='color:red;'>user passw.</span> 
  The user has no access to the settings of the device.
  <br><br>
  </div>
</div>
<div id='msect'>
<div id='bo'></div>
<ul>
<li><a href='/'>home</a></li>
<li><a href='/TIMERCONFIG'>timers</a></li>
<li><a href='/IPCONFIG'>wifi</a></li>
<li><a href='/GEOCONFIG'>time</a></li>
<li><a href='/MQTT'>mqtt</a></li>
<li><a href='/FWUPDATE'>update</a></li>
<li><a href='/REBOOT'>reboot</a></li>
<li><a onclick="return confirm('start AP, are you sure?')" href='/STARTAP'>AP</a></li>
</ul><br>
</div>
<div id='msect'>
  <br><kop>GENERAL SETTINGS</kop><br><br>
</div>

<div id='msect'>
  <div class='divstijl' style='height:50vh;'>

  <form id='fM' method='get' action='basisConfig' oninput='subMit()'>
  <table>
  <tr><td style='width:140px;'>device name<td><input class='inp6' name='swname' maxlength='21' title='no spaces, see help' value='{nm}'></input></tr>
  <tr><td>user passwd<td><input  class='inp5' name='pw1' length='11' value='{pw1}' pattern='.{4,10}' title='between 4 and 10 characters'></input></tr> 
  <tr><td>security<td><input class='inp2' name='bev' value='{bev}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm' ></input></tr> 
  </table></form>
  </div>
</div>
<div id='msect'>
  <ul>
  <li id="sub"><a href="#" onclick='submitFunction("/SW=BACK")'>save </a></li>
  <li><a href='#' onclick='hlpf()'>help</a></li>
  <li><a href='/'>home</a></li>

  </ul><br></div>
</div></body></html>
  )=====";
#endif


void zendPageBasis() {
  DebugPrintln("we are now on zendPageBasis");
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(BASISCONFIG);
  toSend.replace("tieTel", swName );
//  toSend.replace("HAIPS" , String(swName));
  
  // naam terugzetten
  toSend.replace("'{nm}'" , "'" + String(swName) + "'") ;
  toSend.replace("'{pw1}'" , "'" + String(userPasswd) + "'") ;
  toSend.replace("'{bev}'" , "'" + String(aso) + "'") ;

  #ifdef SENSORS
   replace_timerselectbox(); // to set the select for active automatition
  #endif
}

void handleBasisconfig(AsyncWebServerRequest *request) { // form action = handleConfigsave
   // collect the serverarguments   
   //Serial.println("handleBasisconfig");
   
   strcpy(swName, request->arg("swname").c_str());
   strcpy(userPasswd, request->arg("pw1").c_str());
   strcpy(aso, request->arg("bev").c_str());   
    if (swName[0] == '\0') {
         String s = "ESP-" + String(ESP.getChipId());
         s.toCharArray(swName, 21);
    }

#ifdef SENSORS
   strcpy(timer, request->arg("timer").c_str());
   // if this is not 1 than we have to disable all timers
   if(timer[0] != '1') {
      for (int z = 0; z<TIMERCOUNT; z++) timerActive[z] = '0';
      timerConfigsave();
   }
// checken if there is a mismatch between the sensor and the automatition 
// if so, timer becomes 0
   check_mismatch();
#endif
   DebugPrintln("saved basisconfig");
   basisConfigsave();  // save aso, userpass, switchname timer

   actionFlag = 10; //should reboot

}

#if defined SENSORS  
void check_mismatch() {
int klik=0;
  if ( timer[0] == '3' && (sensor[0] != '1' && sensor[0] != '2' &&  sensor[0] != '3' )) {
    int klik=1;
  }  
  if ( timer[0] == '4'  && ( sensor[0] != '2' &&  sensor[0] != '3' )){
    int klik=1;
  }
  if ( timer[0] == '6'  &&  sensor[0] != '6' ){
    int klik=1;
  }
  if ( timer[0] == '7'  &&  sensor[0] != '7' ){
    int klik=1;
  }
  if ( klik == 1 ) {
    timer [0] = '0';
  }
}
#endif


// *********************************************************************************
//          replace the selectbox for active automation only (sensors)
// **********************************************************************************
#ifdef SENSORS
void replace_timerselectbox () {  // wordt door basisconfig gebruikt
toSend.replace("tieTel", swName );
toSend.replace("HANSIART" , String(swName));  

// if there is a temperaturesensor then we haveto show the button for settings
// and also the option for thermostate in the selectbox
if (sensor[0] == '1' || sensor[0] == '2' || sensor[0] == '3') {
DebugPrintln("de waarde van sensor[1] is 1  2 of 3");
//  toSend.replace("'thermostaat' type='hidden'", "'thermostaat' type='button'");
  toSend.replace("aption1", "option"); // 
  } 
 // if there is a dht or BME connected, also the hygrostate
if (sensor[0] == '2' || sensor[0] == '3') {
  toSend.replace("aption2", "option"); //  
}
//if (sensor[0] == '4') {
//  toSend.replace("aption5", "option"); // de option voor de motion sensor

if (sensor[0] == '6') {
  toSend.replace("aption3", "option"); // the option for the light sensor
}
if (sensor[0] == '7') {
  toSend.replace("aption4", "option"); // the option for the digital sensor
}

// zet de geselecteerde option in de select terug
if (timer[0] == '0') {  // geen timers
    toSend.replace ("tM0", "selected");
}
if (timer[0] == '1') { // aan/uit timers
    toSend.replace ("tM1", "selected");
}
if (timer[0] == '3') { //thermostaat
    toSend.replace ("tM3", "selected");
}
if (timer[0] == '4') { //hygrostaat
    toSend.replace ("tM4", "selected");
}
// not needed for the motion sensor, that always works when selected
if (timer[0] == '6') { //lichtsensor
    toSend.replace ("tM6", "selected");
}
if (timer[0] == '7') { //generic digital sensor
    toSend.replace ("tM7", "selected");
}
}

#endif
