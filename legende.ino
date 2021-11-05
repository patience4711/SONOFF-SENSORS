// Dit programma is afgeleid van WiFiManager library.
// We willen dit nu gebruiken als voorbeeld sketch
// Op een ESP12 is de blauwe led led_onb aangesloten op pin 2 
// verder hebben we een ontwikkelboard waar we een led kunnen aansluiten op een willekeurig pin
// en de knop op pin 0
// voor de hardware reset kunnen we geen pin gebruiken dus we moeten hier terugvallen op ESP.reset() of ESP.restart()
// er is een interrupt service routine die funtionaliteit in de knopbediening brengt.
// na opslaan van nieuwe gegevens in SPIFFS volgt een reset omdat anders de nieuwe poort niet meteen werkt.


// De esp haalt de tijd op van een NTP server tijdens de opstart.
// deze tijd wordt in de interne klok ingevoerd en bijgehouden
// Nu kunnen we dingen op bepaalde tijden laten gebeuren
// de website reageert alleen tijdens wakker worden, maakt de zaak traag maar het werkt.
// als de batterij leeg is dan moeten we in deepsleep. De laadfet is dan uit dus dat werkt niet
// een oplossing is om de laadfet anders te schakelen. Die sluiten we kort als de spanning te hoog wordt.
// Dit betekent dat als de batterijspanning te laag is, we rustig in de deep sleep kunnen. 
// we gaan onderoeken of we een timer kunnen instellen die automatisch de lamp kan schakelen. 
// bijvoorbeeld automatich aan als het donker wordt en op het ingestelde tijdstip uit.
// wat is het voordeeel? Dat er alleen radioverkeer nodig is om de tijd op te halen
// 
// de lamp gaat aan 10 min na sunset  if (hour()*60 + minute() > sunset + 10 && lampvlag == 0 && lading == 1
// rekenen: als de tijd == 09.10 dan 9*60+10= 550 > sunset+10 maar kleiner dan de uitschakeltijd
// als de uitschakeltijd is 00.10 dan is

// tijd: we kunnen een unixtijd converteren uit de interne klok time_t  t = now()
// ales lijkt te werken maar raakt snel de verbinding kwijt met netwerk? esp blijft wel draaien en schakelt de lamp
// Udp.stop() en server.client().stop() toegevoegd

// handmatig schakelen moet onafhankelijk van de automatische schakeling kunnen gebeuren. We kunnen dan met domoticz schakelen. 
// als de lamp automatisch is ingeschakeld en ik schakel hem uit moet hij uitblijven
// Dit is oke omdat lampvlag niet verandert
// als hij uit is en ik schakel in dan moet hij aanblijven. 
// als ik smiddags inschakel dan komt er een automatisch inschakelmoment en uitschakel, dan gaat hij automatisch uit.
// de lamp al aan is doen we normaal gesproken niet en na de brandtijd levert geen problemen op 
// dus handmatig schakelen geen probleem met automatisch. Anders automatisch disabelen via het AP

//let op nieuwe geheugenchip chip flashen in dout mode

// voor de fimware update is een input in AP formulier toegevoegd. Hier een link invullen
// in wifimanager.h zendpageUpdate toegevoegd

// wifimanager is aangepast om DHCP mogelijk te maken

// let op: deze versie is gecompileerd met 2.4.2 boardmanager. in start wifi een wificonfig lijn ingevoegd na de dhcp verbinding door wifimanager
// omdat anders dns het niet doet en geen tijd wordt opgehaald
// flashen met 1M 64k spiffs dout
// deze versie kent de puya chip niet. Daarom in esp.cpp en esp.h enkele veranderingen aangebracht.
// voor de puya geheugenchip: in c:/users/hans/appdata/arduino15/packages/esp8266/hardware/esp8266/2.4.0-rc2/ 
// esp.cpp en esp.h vervangen door de exemplaren in de map puya
// de nieuwste wijziging is dat externe links van binnen het netwerk zijn toegestaan zonder authentication

// deze sw is voor de nieuwe sonoff met esp 8255 on board
// heeft geen pin14 meer in plaats daarvan gebruiken we pin 1 tx() nu hebben we geen debug meer op serial

//als er een ap wordt opgestart dan zal dit iedere 5 minuten herstarten als er geen verbinding mee wordt gemaakt
// hierdoor zal hij verbinden als na een stroomstoring de router niet up is.
// update portal, restart AP after connection, prevents client disconnect from ap
// added a function to erase the wifi credentials in a certain sector of the flash
// the portal is restarted after connection was made in AP_STA
// save the current location in eeprom
