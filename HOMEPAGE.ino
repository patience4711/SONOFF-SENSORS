
const char HOMEPAGE[] PROGMEM = R"=====(
<body onload='loadScript()'>
<div id='msect'>
<ul>
<li><a href='/LOGPAGE'>log</a></li>
<li><a href='/INFOPAGE'>info</a></li>
<li style='float:right;'><a id='haha' href='/BASISCONFIG'>settings</a></li>
</ul>
</div>
<div id='msect'>
<kop id='naamveld'>ESP</kop>
</div>
<div id='msect'>
    <div class='divstijl' id='maindiv' style='height:76vh;'>
    <center>
    <span id='pwdby' style='font-size:11px; display:table; color: white;'>powered by Hansiart</span><br>
    <button class='bt' id='knop' onclick='myFunction();' >SWITCH</button>
    <br>
    <br>
    </div>
<div>
</body>
<script>
function loadScript() {
// the first time we load the settings at once
getData();
}


function myFunction() {
var xhttp = new XMLHttpRequest();  
var btn = document.getElementById('knop');
var veld = document.getElementById('veld'); 
    if(!btn.classList.contains('bt_on')) { // the lamp is off
          btn.classList.add('bt_on');
          xhttp.open("GET", "SW=ON", true);
          xhttp.send();
    } else {
          btn.classList.remove('bt_on');
          xhttp.open("GET", "SW=OFF", true);
          xhttp.send();
    }
    //getData(); // sets the page props
    setTimeout(getData, 2000);
}

setInterval(function loadData() {
  getData();
},6000);

function getData() {
var btn = document.getElementById('knop');
var veld = document.getElementById('veld');  
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
      var name = obj.name;
      var state = obj.state;
      if (state == "0") {
        btn.classList.remove('bt_on');
        document.getElementById("maindiv").style.background="grey";
        //document.getElementById("pwdby").style.color="white";
      } else {
        btn.classList.add('bt_on');
        document.getElementById("maindiv").style.background="linear-gradient(#3bb300, #4ce600)";
       // document.getElementById("pwdby").style.color="green";       
      }
      document.getElementById("naamveld").innerHTML=name;
    }
  }
  xhttp.open("GET", "get.Homepagedata", true);
  xhttp.send();
}
</script>
</html>
)=====";

//void zendPageHome(AsyncWebServerRequest *request) {
//
//Serial.println("zendPageHome");
//    if (REPEATYN[0] == 'Y' || repeater_on==true ) { 
//        Serial.println("repeater up");
//        zendPagerepeater();
//    } else {
//        DebugPrintln("going to homepage");
//        toSend = FPSTR(HTML_HEAD);
//        toSend.replace("tieTel", swName ); 
//        toSend += FPSTR(HOMEPAGE);
//        request->send(200, "text/html", toSend);
//    }
//}
