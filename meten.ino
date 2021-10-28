#ifdef SENSORS


// deze functie meet de weersensoren en schakelt zonodig
void meetENschakel() {
  DebugPrint("meet en schakel functie ");  
  DebugPrintln(minute());
// als er een sensor aanwezig is dan meten we die voor domoticz 
    if (sensor[0] == '0' || sensor[0] == '\0') {return;}
    if (sensor[0] == '1' || sensor[0] == '2' || sensor[0] == '3' || sensor[0] == '6' || sensor[0] == '7') { // alleen als er een temperatuur of licht sensor is ingesteld
          meten(); // stel de meetwaarde vast en verzend mqtt indien van toepassing
         // meteen ook schakelen. Dit kan alleen als er een thermo hygro licht digital is ingesteld
         // als er een foute meting is gedaan dan gaan we terug
         // foute metingen zijn temp_c 1000, 300, -172, p500000 
         if (sensor[0] == '1' || sensor[0] == '2' || sensor[0] == '3') { 
            if ( temp_c == 1000 || temp_c == -127 ) {return;} // sla de thermostaat en mqtt verzending over
         }
         if (sensor[0] == '6' && p == 500000) {
          DebugPrintln("lightsensor error p=500000" );
          return;}
         //nu hebben we alle foutmetingen afgevangen ook voor de luchtdruk, die is fout als de temperatuur fout is
         // de foute metingen worden niet gerapporteerd via mqtt en 'staten werken niet
         if (timer[0] == '3' ) { // thermostaat
                 thermostaat(); // als er geschakeld is door de thermostaat dan zijn de gegevens niet verzonden
              }

         if (timer[0] == '4' ) {
                 hygrostaat();  // als er geschakeld is door de thermostaat dan zijn de gegevens niet verzonden
              }

         if ( timer[0] == '6' ) {
            lichtSensor(); // als er geschakeld is door de thermostaat dan zijn de gegevens niet verzonden
          }

         if (timer[0] == '7' ) {
             digitalSensor();
             }
     // de meetwaarden zijn nu bekend en als er een thermostaat actief is, ook geschakeld
     // dus nu kan mqtt verstuurd worden        
     sendMqttsensor(); //verzend de meetwaarden                    
   }
}

// *******************************************************************************************
//                                    METEN
//*********************************************************************************************
void meten() { 
  
  DebugPrint("function meten, time is ");
  DebugPrintln(minute());
          if(sensor[0] == '1') { 
               DebugPrintln("the ds18B20 is polled");
               sensors.requestTemperatures();
               temp_c = sensors.getTempCByIndex(0);
               temp_c = temp_c + atof(tempCal);
               //domoticz(0, true); 
               } // de ds18B20 sensor lezen
          if(sensor[0] == '2') { 
               DebugPrintln("the dht22 is polled");
               gettemperature(); // de dh22sensor lezen
               temp_c = temp_c + atof(tempCal);
              // deze gegevens naar domoticz seinen
              // domoticz(0);
               } 
           if(sensor[0] == '3') { 
               DebugPrintln("the bme280 is polled");
               getWeather();
               temp_c = temp_c + atof(tempCal);
               if ( humidity == 0 ) {temp_c = 1000; } // als er geen vochtigheid is polled
               } 

//           if( sensor[0] == '7' ) {
//           if (digitalRead(3) == HIGH ) { domoticz(1, false); } else { domoticz(2, false); } 
//           }
           
           if(sensor[0] == '6') { 
               DebugPrintln("the max44009 is polled");
               p = licht.get_lux();
               DebugPrint("the lichtvalue in lux = ");
               DebugPrintln(String(p));
               DebugPrintln("");
               if (licht.begin()) {
                    //Serial.println("geen max44009 sensor aangesloten");
                    p=500000;
                   }
               }        
 }


void getWeather() {
   // count the pollings cannot be >10
  float gemiddeld = 0;
  float som = 0;
  
  humidity = bme.readHumidity();
  temp_c = bme.readTemperature(); // in celsius
  if ( isnan(temp_c) ) 
  {
      temp_c = 1000;
      return;
  }
  //t = t*1.8+32.0;
  dp = temp_c-0.36*(100.0-humidity);
    
  p = bme.readPressure()/100.0F;
  pin = 0.02953*p;

 luchtdruktabel[0]= p;
  // de waarde van p opslaan in een array
  // nu de reeds gedane metingen verplaatsen
  for (int x=weerteller; x>0; x--) 
  {
     luchtdruktabel[x] = luchtdruktabel[x-1];
     som +=luchtdruktabel[x];
  }
 
   // multiply the values by 10 and make it an integer
   gemiddeld = int(som/weerteller*10);
   int huidig = int(luchtdruktabel[0]*10);
    
   if (huidig > gemiddeld) {
    DebugPrintln("the pressure rises");
    ldString="↑↑↑";
  } 
  if (huidig < gemiddeld) {
    DebugPrintln("the pressure lowers");
    ldString="↓↓↓" ;
  } 
  if (huidig == gemiddeld)  {
    DebugPrintln("the pressure unchanged");
    ldString="←→";
  }
 if (weerteller<10) {
  weerteller += 1;  
 }
}
#endif
// ******************************************************************************************
//   mqtt message over de meetwaarde van een sensor en de status van de schakelaar
// ******************************************************************************************

void sendMqttswitch() 
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
