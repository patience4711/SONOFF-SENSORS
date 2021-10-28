const char HTML_HEAD[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>tieTel</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<script type='text/javascript'>
function hlpf() {
document.getElementById("hlpd").style.display = "block";
}
function sl() {  
document.getElementById("hlpd").style.display = "none";
}
function subMit() {
document.getElementById("sub").style.display = "block";
}
function submitFunction(a) {
document.getElementById("bo").innerHTML="<br>wait...<br>processing<br>your<br>request"; 
document.getElementById("bo").style.display="block"; 
document.getElementById("fM").submit();
setTimeout(function(){window.location.href=a;}, 3000 ); 
}
</script>
</head>
)=====";

const char STYLESHEET[] PROGMEM = R"=====(
body {
  background: linear-gradient(#e8f5f7, #caf5fb); 
  color:navy;
  font-family:'lato',Verdana,Sans-serif;
  font-size:16px;
  }

.divstijl {
  background: linear-gradient(#e8edc8, #c8eaed); 
  padding:0px 0px 0px 0px;
  width: 96%;
  height: 99%; 
  }
#msect {
  display: flex;
  flex-basis: 100%;
  align-items: center;
  margin: 0px 0px;
  width:96vw;
}
#msect:before,
#msect:after {
  content: "";
  flex-grow: 1;
  margin: 0px 0px;
}

kop {
  font-size: 4vw;
  font-weight:bold;
  padding-top: 1vh;
  padding-bottom: 1vh;
}

p {
  color: #006600;
  font-family: 'lato',Verdana,Sans-serif;
  font-size:14px;
  }
.bt{
  width:30vw;
  height:20vw;
  padding:6px;
  margin:6px;
  text-align:center;
  border-radius:15px;
  color:white;
  background-color:red;
  font-weight:bold;
  font-size:4vw;
  box-shadow: 0 9px #999;
  }
.red{
    background-color:#b9b9c1;
    border:2px solid black;
  }
.rad {
    background-color:#cc99ff;
    border:2px solid black;
    }
.bt:active {
  background-color: #3e8e41;
  box-shadow: 0 5px #666;
  transform: translateY(4px);
}
input { font-size:24px; }
.inp1 { width:46px; }
.inp2 { width:62px; } 
.inp3 { width:76px; }
.inp4 { width:180px;} 
.inp5 { width:172px;}
.inp6 { width:230px;} 

option {
    direction: ltr;
}
.sb1 {
   font-size:24px;
   height:34px;
  } 
 
tr {height:45px;} 
input[type=checkbox] 
  {
    width:30px; 
    height:30px;
   }
ul {
  list-style-type: none;
  margin: 0 auto 0 auto;
  padding: 0;
  overflow: hidden;
  background-color: #333;
  border-radius:6px;
   width: 96vw;
}
li {
  float: left;
}
li a:hover {
  background-color: #ff0808;
}
a:focus {background-color: red;}

b {
color: #;
font-size:18px;
font-weight:bold;
}
.close {
  color: red;
  float: right;
  font-size: 48px;
  font-weight: bold;
  cursor: pointer;
}
.close:hover {color: green;}

#hlpd {
  background-color: #ffffff; 
  border: solid 2px; 
  display:none; 
  padding:4px;
  width:94vw;
}
th { width:44px; }

#bo {
  width:100%;
  height:100%; /* height of 100% too*/
  position: absolute;
  left:0; top:0;
  background-color:rgba(192,192,192,0.9);
  color:white; 
  z-index:10;
  font-size: 90px;
  font-weight: bold;
  text-align: center;
  display: none;
}

#sub { float: right; background:green;display:none }

li a {
  display: inline-block;
  color: white;
  text-align: center;
  padding: 14px 6px 10px;
  text-decoration: none;
  width:70px;
  font-size:18px;  
}
@media only screen and (max-width: 800px) {
  li a {
    font-size:16px;
    padding: 14px 26px 10px 12px;
    width: 40px;
  }
}  
)=====";
   
const char CONFIRM_IP[] PROGMEM = R"=====(
<html>
<body style='font-size:26px;'>
<script type='text/javascript'>setTimeout(function(){ window.location.href='http://{adres0}/'; }, 6000 ); </script>
processing IP settings..<br><br>
The device is going to reboot. <strong>Wait until the led blinked 3 times!</strong><br><br>
#zin#
</body></html>
)=====";

//const char CONFIRM_IP[] PROGMEM = R"=====(
//<body style='font-size:16px;'>
//<script type='text/javascript'>setTimeout(function(){ window.location.href='http://{adres0}/'; }, 6000 ); </script>
//<h1>  IP INSTELLINGEN OPSLAAN</h1><br><br><div class='divstijl' style='width: 800px;'>
//De IP instellingen worden opgeslagen. <br><br>
//Uw Esp device gaat nu opnieuw opstarten. <strong>Wacht tot de led 3x heeft geknipperd!</strong><br><br>
//{zin}
//</body></html>
//)=====";
//#ifndef SENSORS
//const char CONFIRM_REPEATER[] PROGMEM = R"=====(
//<!DOCTYPE html><html><head><meta charset='utf-8'>
//<title>sonoff pow</title>
//<meta name="viewport" content="width=device-width, initial-scale=1">
//<link rel="stylesheet" type="text/css" href="/STYLESHEET">
//</head>
//<body style='font-size:16px;'>
//<script type='text/javascript'>setTimeout(function(){window.location.href='/';}, 3000 ); </script>
//<h1>   INSTELLINGEN OPSLAAN</h1><br><br><div class='divstijl' style='width: 800px;'>
//De instellingen worden opgeslagen. <br><br>
//Deze pagina sluit automatisch na enkele seconden<br><br>
//<br>Controleer of de instellingen correct zijn.</div>
//<br><br> <a class='bt' href='/SW=BACK'>OK</a></body></html>
//)=====";
//#endif
const char UPDATE_FORM[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<style> 
input {
  font-size:3vw;
}  
</style>
<script>
function showDiv() {
document.getElementById('main').style.background="#b3ffd9";  
document.getElementById('hiddendiv').innerHTML="<marquee><kop>updating software, do not interrupt .....</kop></marquee>";
}
</script></head>
<body><div id='main'>
<center><kop> HANSIART FIRMWARE UPDATER</kop>
<br><br><br><table><tr><td>
<form method='POST' action='/handleFwupdate' enctype='multipart/form-data' onsubmit='showDiv()'>
<input id='inP' type='file' name='update' pattern='\S+' title='cannot be empty' required>
<tr><td><br><tr><td><input type='submit' value='update'></form>
<tr><td><br><tr><td><a href='/BASISCONFIG'><input type='submit' value='cancel'></a></table><br>
<div id='hiddendiv' style='margin: 50px; width: 40%; background:#e7f2bd;'></div><br><br>
</div></center></body></html>  
)=====";

#define FAVICON_len 318
const uint8_t FAVICON[] PROGMEM = {
0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x01, 0x00, 0x04, 0x00, 0x28, 0x01,
0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00,
0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0xDB,
0xD6, 0x00, 0xD1, 0x0F, 0x18, 0x00, 0xB4, 0xDD, 0xDE, 0x00, 0x66, 0xE1, 0xE3, 0x00, 0xD3, 0xE6,
0xE5, 0x00, 0x81, 0xE1, 0xE3, 0x00, 0x9A, 0xDA, 0xDB, 0x00, 0x75, 0xDF, 0xE0, 0x00, 0xB5, 0x14,
0x9D, 0x00, 0xB6, 0xE3, 0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44,
0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x49, 0x91, 0x99, 0x91, 0x19, 0x99, 0x19, 0x94, 0x49, 0x21,
0x22, 0x12, 0x21, 0x22, 0x12, 0x94, 0x49, 0x21, 0x66, 0x16, 0x61, 0x66, 0x12, 0x94, 0x49, 0x21,
0x55, 0x15, 0x51, 0x55, 0x12, 0x94, 0x49, 0x21, 0x57, 0x71, 0x17, 0x71, 0x11, 0x94, 0x49, 0x26,
0x50, 0x00, 0x00, 0x75, 0x62, 0x94, 0x49, 0x26, 0x57, 0x33, 0x30, 0x75, 0x62, 0x94, 0x48, 0x86,
0x58, 0x83, 0x80, 0x75, 0x62, 0x84, 0x48, 0x86, 0x58, 0x80, 0x88, 0x75, 0x68, 0x84, 0x48, 0x86,
0x58, 0x87, 0x88, 0x88, 0x88, 0x84, 0x48, 0x88, 0x88, 0x85, 0x58, 0x85, 0x88, 0x94, 0x48, 0x86,
0x68, 0x86, 0x66, 0x82, 0x82, 0x94, 0x48, 0x82, 0x28, 0x82, 0x22, 0x88, 0x82, 0x94, 0x48, 0x89,
0x98, 0x89, 0x99, 0x98, 0x99, 0x94, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
