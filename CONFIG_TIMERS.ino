// main page row 90 defines TIMERCOUNT

const char TIMERCONFIG_START[] PROGMEM = R"=====(
<body>

<div id='main-sect'>
<div id='bo'></div>
    <ul>
    <li><a href='/'>home</a></li>
    <li><a href='/TIMER?welke=0' >timer0</a></li>
    <li><a href='/TIMER?welke=1' >timer1</a></li>
    <li><a href='/TIMER?welke=2' >timer2</a></li>
    <li><a href='/TIMER?welke=3' >timer3</a></li>
    </ul>
</div>

<div id='main-sect'>
  <center><kop>TIMER CONFIG </kop></center><br>
</div>

<div id='main-sect'>
 <center><div class='divstijl'>
<form id='fM' method='get' action='timerconfig' oninput='subMit();'>
  <center><irame name>
  </div>
<div><br></form>
<div id='msect'>
  <ul>
  <li><a href='/BASISCONFIG'>done</a></li>
  <li id="sub"><a href="#" onclick='submitFunction("/SW=BACK")'>save</a></li>
  </ul>
  <br>
</div>
</body></html>
 )=====";
// voor geen sonoff

 
 const char TIMER_GENERAL[] PROGMEM = R"=====(
<center><table><tr><td style='width:100px;'><h4> TIMER {nr} </h4><td style='width:40px;'>active ?<td><input type='checkbox' name='ta' tActive>
<tr><td>on:<td><input class='inp2' name='inw' value='{inx}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm'>
<td><select name='zonattaan' class='sb1'>
<option value='0' zonattaan_1>absolute</option>
<option value='3' zonattaan_4>before sunrise</option>
<option value='4' zonattaan_5>after sunrise</option>
<option value='1' zonattaan_2>before sunset</option>
<option value='2' zonattaan_3>after sunset</option>

</select></td></tr>
<tr><td>off:<td><input class='inp2' name='uitw' value='{uitx}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm' >
<td><select name='zonattuit' class='sb1'>
<option value='0' zonattuit_1>absolute</option>
<option value='3' zonattuit_4>before sunrise</option>
<option value='4' zonattuit_5>after sunrise</option>
<option value='1' zonattuit_2>before sunset</option>
<option value='2' zonattuit_3>after sunset</option>

</select></td></tr></table>
<br><table>
<thead><tr style='height:20px;'>
<th> su </th>
<th> mo </th>
<th> tu </th>
<th> we </th>
<th> th </th>
<th> fr </th>
<th> sa </th>
</tr></thead>
<tr>
<td><input type='checkbox' name='zo' selzo>
<td><input type='checkbox' name='ma' selma>
<td><input type='checkbox' name='di' seldi>
<td><input type='checkbox' name='wo' selwo>
<td><input type='checkbox' name='do' seldo>
<td><input type='checkbox' name='vr' selvr>
<td><input type='checkbox' name='za' selza>
</table>
</center><br>
 )=====";

void zendPageTimers() {
DebugPrintln("we zijn nu op zendPageTimers");

toSend = FPSTR(HTML_HEAD);
toSend += FPSTR(TIMERCONFIG_START);

toSend.replace("tieTel", swName );
//toSend.replace("HANSIART" , String(swName));
// de waarden terugzetten
//replace_timerselectbox(); // zet de select voor actieve automatisering goed

// initieel wordt de timer geopend die als laatste geraadpleegd is (eeprom)
//tKeuze=0;
zendPageRelevantTimers();

}
// ****************************************************************************
// we gaan nu een pagina tonen met de voor te timer relevante inputs
// ****************************************************************************
void zendPageRelevantTimers() {
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(TIMERCONFIG_START);
  toSend.replace("tieTel", swName );
  //eerst weer de select en keuze terugzetten old
  //replace_timerselectbox();
  
  DebugPrintln("we zijn op zendPageRelevantTimers");
  DebugPrint("tKeuze = "); DebugPrintln(String(tKeuze));
  plaats_timerpage();  //plaats de timerpage voor de gekozen timer
}



// deze functie wordt opgeroepen na timer instellingen
//*******************************************************************************************
//              voorbereiden voor opslaan van de gegevens
// *****************************************************************************************
void handleTimerconfig(AsyncWebServerRequest *request) { // form action = handletimerConfig
// we krijgen alleen serverargumenten van de betreffende timer bijv zonattaan0 dus
//lees de serverargumenten en plaats deze in de betreffende variabelen
// de waarde in de select voor de actieve autom slaan we altijd op
//strcpy(timer,request->arg("timer").c_str()); in basisconfig

String dag = "";
if(request->hasParam("ta")) {
dag = request->getParam("ta")->value();  
}
   if (dag == "on") { timerActive[tKeuze] = '1'; } else { timerActive[tKeuze] = '0';}
    // eerst terugzetten welke geselecteerd zijn in zonattaan en zonnatuit
             String temp = "";
             char tempChar[1]="";
    
              temp = request->arg("zonattaan").c_str(); // arumenten zijn 0 tm 6
              temp.toCharArray(tempChar, 2);
              relToSunOn[tKeuze]=tempChar[0];  // de variabele relToSunOn bijwerken
    
              DebugPrint("tempChar = "); DebugPrintln(String(tempChar));
              DebugPrint("relToSunOn = "); DebugPrintln(String(relToSunOn));
    
              temp = request->arg("zonattuit").c_str(); // arumenten zijn 0 tm 4
              temp.toCharArray(tempChar, 2);
              relToSunOff[tKeuze]=tempChar[0];         
              DebugPrint("relToSunOff = "); DebugPrintln(String(relToSunOff));                    
            
              //nu de inschakeltijd
              temp = request->arg("inw").c_str();
              temp.toCharArray(tempChar, 6);
              for (int i=0; i<5; i++){
                 switchOn[tKeuze*5+i] = tempChar[i];
              }
              temp = request->arg("uitw").c_str();
              temp.toCharArray(tempChar, 6);
              for (int i=0; i<5; i++){
                 switchOff[tKeuze*5+i] = tempChar[i];
              }      
              // de waarden van de dagen in een array weekdag opslaan
              char *grap[] = {"zo", "ma", "di", "wo", "do", "vr", "za"};
              String wd = ""; 
              dag = "";
              //Char ddag[7] = "  
              //for (int i = 0; i<aantal; i++) {    
                   for (int x = 0; x < 7; x++) {
                    dag = request->arg(grap[x]) ; //dus zo+1
                    //bijv dag=server.arg(zo1)
                     if (dag == "on") {
                    // als tKeuze=5 dan begint het bij 35 en eindigt bij 42
                    // als tKeuze=0 dan begint het bij 0 en eindigt bij 6  
                    weekDag[tKeuze*7+x] = 'y'; 
                    } else {
                    weekDag[tKeuze*7+x] = 'n';
                    }  
                  }
// deze funties werken
// nu zenden we de confirm 
 //DebugPrintln("we gaan een nieuwe webpage inlezen in toSend");
 // toSend = FPSTR(CONFIRM);
  
 // request->send(200, "text/html", toSend); //send the html code to the client

 timerConfigsave(); // alles opslaan in SPIFFS
 DebugPrintln("timer instellingen opgeslagen");

actionFlag = 10;
}
  


// deze functie geeft de waarde van vervang terug
// input is bijvoorbeeld "1" en zooattaan
String zonatt_replace( String argument1, String argument2) {

String vervang = "";
switch (argument1[0]) {
  case '0': //absoluut
    vervang = argument2 + "_1";
    break;
  case '1': //voor zons ondergang
    vervang = argument2 + "_2";
    break;
  case '2': //na zons ondergang
    vervang = argument2 + "_3";
    break;
  case '3': // voor zons opkomst
    vervang = argument2 + "_4";
    break;
  case '4': // na zons opkomst
    vervang = argument2 + "_5";
    break;
    }
return vervang;
}



// **********************************************************************
//        de timerpagina met de actuele waarden terugzetten
// **********************************************************************
void plaats_timerpage() 
{

// we moeten de timerpagina plaatsen 
    toSend.replace("<irame name>" , FPSTR(TIMER_GENERAL));  
    toSend.replace("{nr}" , String(tKeuze)); // vervang timer nummer
    
    if(timerActive[tKeuze] == '1') toSend.replace("tActive", "checked");
    
    // we gaan nu de "selected" terugzetten voor de option in de select zonattaanwelke_1 2 3 4 of 5 
    DebugPrintln("pagina_replace");
    // terugzetten waar selected moet komen
    toSend.replace(zonatt_replace(String(relToSunOn[tKeuze]), "zonattaan"), "selected"); //door zonatt_replace wordt vervang gereturned
    toSend.replace(zonatt_replace(String(relToSunOff[tKeuze]), "zonattuit"), "selected"); //door zonatt_replace wordt vervang gereturned
    
     // de aangevinkte selectboxen terugzetten 
    char *grap[] = {"selzo", "selma", "seldi", "selwo", "seldo", "selvr", "selza"};
    String vervang = "";
    //weekDag
      DebugPrintln("vervangen van de checkboxen om de gecheckte te tonen");
    // voor deze timer doen, 7x
    //int i = tKeuze;
        for(int x=0; x<7; x++){ // bij 3 is dit van 21 tot 27 kleiner dan 28

          vervang = String(grap[x]); // als i=3 dan van 21-21 naar 27

               if (weekDag[tKeuze*7+x] == 'y') { toSend.replace(vervang, "checked");}
         }
    
    // we hebben een char met alle tijden, kunnen we dit vertalen naar een veld in de pagina
    //String temp = "";
    //temp = String (switchOnOff[0],5);
      DebugPrintln("vervangen van de tijdwaarden");
    // vervang de nulwaarden voor de tijden in html2 door de waarden uit spiffs, als die er niet zijn wordt het 00:00
     
    //de waarden van de input voor de inschakeltijden werkt!
    toSend.replace("{inx}"  , "" + String(switchOn[tKeuze*5]) + String(switchOn[tKeuze*5+1]) + String(switchOn[tKeuze*5+2]) + String(switchOn[tKeuze*5+3]) + String(switchOn[tKeuze*5+4]));
    //de waarden van de input voor de uitschakeltijden
    toSend.replace("{uitx}" , "" + String(switchOff[tKeuze*5]) + String(switchOff[tKeuze*5+1]) + String(switchOff[tKeuze*5+2]) + String(switchOff[tKeuze*5+3]) + String(switchOff[tKeuze*5+4])); 
}  

#ifdef SENSORS
void timerSwitch() 
{
//for each timer see if it has to switch
int welke = check_hasSwitched(); // check which switch is on 
    if (welke == 100) // all off
    {
    // check for all timers if mustSwitch is applied
    // if yes switch on
        for(int i=0; i<TIMERCOUNT; i++) 
        {
          if ( mustSwitch[i] ) 
             {
               if ( now() > (inschakeltijd[i]) && now() < uitschakeltijd[i] && timer[0] == '1') 
                  { 
                      switch_on_now(true, false, "timer " + String(i)); // meteen de lamp aan
                      value = 3 + i; // 3 of 4 of 5 of 6
                      hasSwitched[i] = true;
                      break; //if we switched one, we break otherwise more than one is switched
                  }
            }
         } // end for loop
    } 
    else 
    {
    //we know now that switch "welke" is on
    // check if we must switch this particulary timer off
        if (now() > uitschakeltijd[welke] ) { 
              switch_off_now(true, false, "timer " + String(welke)); 
              value = 0;
              mustSwitch[welke] = false;
              hasSwitched[welke]=false;
              }

    } // end else
} 
#else
// not sensors
void timerSwitch() 
{
//for each timer see if it has to switch
int welke = check_hasSwitched(); // check which switch is on 
    if (welke == 100) // all off
    {
    // check for all timers if mustSwitch is applied
    // if yes switch on
        for(int i=0; i<TIMERCOUNT; i++) 
        {
          if ( mustSwitch[i] ) 
             {
               if ( now() > (inschakeltijd[i]) && now() < uitschakeltijd[i]) 
                  { 
                      switch_on_now(true, false, "timer " + String(i)); // meteen de lamp aan
                      value = 5 + i; // 5 of 6 of 7 of 8
                      hasSwitched[i] = true;
                      break; //if we switched one, we break otherwise more than one is switched
                  }
            }
         } // end for loop
    } 
    else 
    {
    //we know now that switch "welke" is on
    // check if we must switch this particulary timer off
        if (now() > uitschakeltijd[welke] ) { 
              switch_off_now(true, false, "timer " + String(welke)); 
              value = 0;
              mustSwitch[welke] = false;
              hasSwitched[welke]=false;
              }

    } // end else
}
#endif
int check_hasSwitched() {
    // if switched by a timer, return timernbr 
    for (int x=0; x<TIMERCOUNT; x++){
      if ( hasSwitched[x] ) return x;
    }
return 100;
}
