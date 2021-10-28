
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
    <span id='pwdby' style='font-size:11px; display:table;'>powered by Hansiart</span><br>
    <button class='bt' id='knop' onclick='myFunction();' style='background:#b30000;'>SWITCH-ON</button>
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
    if(btn.innerHTML == "SWITCH-ON" ) {
          xhttp.open("GET", "SW=ON", true);
          xhttp.send();
    } else {
          xhttp.open("GET", "SW=OFF", true);
          xhttp.send();
    }
    getData(); // sets the page props
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
        btn.innerHTML = "SWITCH-ON";
        btn.style.background = "#009900";
        document.getElementById("maindiv").style.background="grey";
        document.getElementById("pwdby").style.color="white";
      } else {
        btn.innerHTML = "SWITCH-OFF";
        btn.style.background = "#b30000";
        document.getElementById("maindiv").style.background="linear-gradient(#e8edc8, #c8eaed)";
        document.getElementById("pwdby").style.color="green";       
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
