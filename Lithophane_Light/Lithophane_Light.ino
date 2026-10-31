#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <LittleFS.h>
#include <math.h>

// NodeMCU ESP-12E, WS2812B 40 tk; D2 = GPIO4
constexpr uint8_t LED_PIN = D2;
constexpr uint16_t LED_COUNT = 40;
// false: LED 0 all, LED 19 üleval; true: vastupidi
constexpr bool REVERSE_SPIRAL = false;
const char* AP_SSID = "Lithophane-Light";
const char* AP_PASSWORD = "lithophane123";
constexpr const char* SETTINGS_FILE = "/lithophane-v2.txt";

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

enum Mode : uint8_t { STATIC_LIGHT=0, CANDLE=1, BREATHING=2, COLOR_FADE=3, VERTICAL_WAVE=4, FIRE=5 };
struct Settings {
  uint8_t mode = STATIC_LIGHT;
  uint8_t brightness = 120;
  uint8_t speed = 50;
  uint8_t strength = 75;
  uint32_t color = 0xFFE0B0;
  bool power = true;
} s;

uint32_t lastFrame = 0, lastChange = 0;
bool savePending = false, fsOK = false, immediateFrame = true;
float phase = 0.0f;
float candleLevel = 0.8f;
float flameLeft = 0.5f, flameRight = 0.5f;
float fireDetail[LED_COUNT] = {};

const char PAGE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="et"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Lithophane Light</title><style>
*{box-sizing:border-box}body{background:#151922;color:#f5f5f5;font:16px system-ui,Arial;margin:0;padding:18px}
main{max-width:460px;margin:auto}h1{font-size:25px;margin:0 0 6px}small{color:#adb5c5}
.panel{background:#242a36;border-radius:12px;padding:20px;margin:16px 0}
label{display:block;margin-bottom:9px}.control{margin-top:22px}.head{display:flex;justify-content:space-between;gap:10px}
output{color:#f3b75b}select,input[type=color]{width:100%;background:#343c4b;color:white;border:1px solid #566174;border-radius:8px;padding:10px;font-size:16px}
input[type=color]{height:55px;padding:5px}input[type=range]{width:100%;accent-color:#edac51}
button{width:100%;padding:14px;color:white;background:#bb7826;border:0;border-radius:8px;font-size:17px;font-weight:600}button.off{background:#59616d}
.note{font-size:13px;color:#adb5c5;line-height:1.4}#status{text-align:center;color:#adb5c5;font-size:13px}
.palette{display:grid;grid-template-columns:repeat(6,minmax(0,1fr));gap:9px;margin-bottom:12px}.swatch{height:42px;padding:0;border:2px solid transparent;border-radius:8px;cursor:pointer}.swatch.active{border:3px solid #fff;outline:1px solid #000}.swatch:focus-visible{outline:3px solid #fff}
</style></head><body><main>
<h1>Lithophane Light</h1><small>NodeMCU ESP8266 · 40 NeoPixel</small>
<div class="panel"><button id="power" onclick="togglePower()">Valgustus sees</button></div>
<div class="panel">
<label for="mode">Valgusrežiim</label><select id="mode" onchange="changed()">
<option value="0">Staatiline valgus</option><option value="1">Küünlavalgus</option>
<option value="2">Hingamine</option><option value="3">Värviüleminek</option>
<option value="4">Vertikaalne valguslaine</option><option value="5">Tuleleek</option></select>
<div class="control" id="colorControl"><label for="color">Valguse värv</label>
<div class="palette" id="palette"></div><input type="color" id="color" value="#ffe0b0" oninput="changed()"></div>
<div class="control"><div class="head"><label for="brightness">Heledus</label><output id="bv"></output></div>
<input type="range" min="0" max="255" id="brightness" oninput="changed()"></div>
<div class="control" id="speedControl"><div class="head"><label for="speed">Efekti kiirus</label><output id="sv"></output></div>
<input type="range" min="1" max="100" id="speed" oninput="changed()"></div>
<div class="control" id="strengthControl"><div class="head"><label for="strength">Efekti tugevus</label><output id="tv"></output></div>
<input type="range" min="0" max="100" id="strength" oninput="changed()"></div>
<p class="note">Heledus määrab üldise valguse tugevuse. Efekti tugevus määrab heleda ja tumeda faasi erinevuse.</p>
</div>
<p id="status">Ühendamine...</p>
</main><script>
const $=x=>document.getElementById(x);let power=true,timer=null,seq=0;
const paletteColors=['#ffb060','#ffd09a','#ffe4c4','#ffffff','#e0efff','#ff3838','#ff7540','#ff9500','#ffcc00','#fff34d','#b7f53f','#44e45e','#00b87b','#2cddbc','#00d7ef','#29a6ff','#2156ff','#1424a0','#7650ff','#ad53ed','#e44bdb','#ff60aa','#ff9bbd','#e7c5ff'];
paletteColors.forEach(c=>{const b=document.createElement('button');b.type='button';b.className='swatch';b.style.background=c;b.title=c;b.setAttribute('aria-label','Värv '+c);b.onclick=()=>{$('color').value=c;changed()};$('palette').appendChild(b)});
function paint(){
 const mode=+$('mode').value,b=+$('brightness').value;
 $('bv').textContent=Math.round(b*100/255)+'%';$('sv').textContent=$('speed').value+'%';$('tv').textContent=$('strength').value+'%';
 $('colorControl').style.display=[0,2,4].includes(mode)?'block':'none';
 $('speedControl').style.display=mode===0?'none':'block';
 $('strengthControl').style.display=mode===0||mode===3?'none':'block';
 $('power').textContent=power?'Valgustus sees':'Valgustus väljas';$('power').classList.toggle('off',!power);
 document.querySelectorAll('.swatch').forEach(el=>el.classList.toggle('active',el.title.toLowerCase()===$('color').value.toLowerCase()));
}
function changed(){paint();clearTimeout(timer);timer=setTimeout(send,140)}
function send(){const n=++seq;const params=new URLSearchParams({mode:$('mode').value,brightness:$('brightness').value,speed:$('speed').value,strength:$('strength').value,color:$('color').value.slice(1),power:power?1:0});
 fetch('/set?'+params).then(r=>{if(!r.ok)throw Error('HTTP '+r.status);if(n===seq)$('status').textContent='Seadistatud'}).catch(()=>{if(n===seq)$('status').textContent='Ühenduse viga'});
}
function togglePower(){power=!power;changed()}
fetch('/state').then(r=>{if(!r.ok)throw Error();return r.json()}).then(v=>{for(const x of ['mode','brightness','speed','strength'])$(x).value=v[x];$('color').value='#'+v.color;power=!!v.power;paint();$('status').textContent='Valmis'}).catch(()=>$('status').textContent='Seadistusi ei saanud lugeda');
</script></body></html>)HTML";

float clampf(float x, float a=0.f, float b=1.f){return x<a?a:(x>b?b:x);}
float heightOf(uint16_t i){float h=float(i)/float(LED_COUNT-1);return REVERSE_SPIRAL?1.f-h:h;}
float amplitude(){return s.strength/100.f;}
void rgb(uint16_t i, float r, float g, float b){
  float k=s.brightness/255.f;
  strip.setPixelColor(i,strip.Color(uint8_t(clampf(r*k/255.f)*255.f+0.5f),uint8_t(clampf(g*k/255.f)*255.f+0.5f),uint8_t(clampf(b*k/255.f)*255.f+0.5f)));
}
void chosen(uint16_t i,float level){rgb(i,((s.color>>16)&255)*level,((s.color>>8)&255)*level,(s.color&255)*level);}
float unitRand(){return random(0,10001)/10000.f;}
void render(float dt){
 strip.clear();
 if(!s.power){strip.show();return;}
 const float a=amplitude();
 // Kerge kuni selgelt nähtav liikumine. Kiirus ei muuda efekti tugevust.
 const float rate=0.3f+2.7f*s.speed/100.f;
 phase+=dt*rate;
 if(phase>2000.f)phase-=2000.f;
 switch(s.mode){
 case STATIC_LIGHT:
   for(uint16_t i=0;i<LED_COUNT;i++)chosen(i,1.f);
   break;
 case CANDLE:{
   // Ühene aeglane värelus + väike ruumiline erinevus.
   float target=unitRand();
   candleLevel+=(target-candleLevel)*clampf(dt*(0.8f+rate*3.f));
   float slow=0.5f+0.5f*sinf(phase*3.f);
   float flick=clampf(0.55f*candleLevel+0.45f*slow);
   float level=1.f-a*(0.82f*flick);
   for(uint16_t i=0;i<LED_COUNT;i++){
     float local=1.f-a*0.13f*(0.5f+0.5f*sinf(phase*4.f+i*0.75f));
     rgb(i,255.f*level*local,155.f*level*local,60.f*level*local);
   }
   break;}
 case BREATHING:{
   // 0% tugevus: ühtlane; 100%: vahemik 12...100%.
   float wave=(1.f-cosf(phase*2.f))/2.f;
   float level=(1.f-a)+a*(0.12f+0.88f*wave);
   for(uint16_t i=0;i<LED_COUNT;i++)chosen(i,level);
   break;}
 case COLOR_FADE:{
   // Küllastunud RGB-toonid ja sujuv liikumine läbi kogu värviringi.
   uint16_t hue=uint16_t(fmodf(phase*3300.f,65536.f));
   uint32_t c=strip.gamma32(strip.ColorHSV(hue,255,255));
   for(uint16_t i=0;i<LED_COUNT;i++)rgb(i,(c>>16)&255,(c>>8)&255,c&255);
   break;}
 case VERTICAL_WAVE:{
   // Tume, kuid nähtav taust; üks lai hele piirkond liigub üles-alla.
   float center=0.5f-0.5f*cosf(phase*1.6f);
   for(uint16_t i=0;i<LED_COUNT;i++){
     float d=fabsf(heightOf(i)-center);
     float peak=expf(-d*d*55.f);
     float level=(1.f-a)+a*(0.12f+0.88f*peak);
     chosen(i,level);
   }
   break;}
 case FIRE:{
   // Korrelatsiooniga leegid: kiiremad muutused all, tipp liigub üles.
   flameLeft+=(unitRand()-flameLeft)*clampf(dt*(2.f+rate*3.f));
   flameRight+=(unitRand()-flameRight)*clampf(dt*(2.f+rate*3.f));
   float wave=0.5f+0.5f*sinf(phase*6.f);
   for(uint16_t i=0;i<LED_COUNT;i++){
     float h=heightOf(i);
     float target=unitRand();
     fireDetail[i]+=(target-fireDetail[i])*clampf(dt*(3.f+rate*5.f));
     float upward=0.5f+0.5f*sinf(phase*7.f-h*9.f);
     float rawMovement=0.33f*flameLeft+0.22f*flameRight+0.25f*upward+0.20f*fireDetail[i];
     float movement=clampf((rawMovement-0.25f)*2.2f);
     float low=0.12f+0.22f*(1.f-h);
     float high=1.0f-0.12f*h;
     float strong=low+(high-low)*movement;
     float quiet=0.78f-0.13f*h;
     float level=(1.f-a)*quiet+a*strong;
     float colorShift=a*(0.65f*movement+0.35f*wave);
     // Punane-oranž-kollane, mitte staatiline soe valge.
     rgb(i,255.f*level,(65.f+125.f*colorShift)*level,(6.f+40.f*colorShift)*level);
   }
   break;}
 default:for(uint16_t i=0;i<LED_COUNT;i++)chosen(i,1.f);
 }
 strip.show();
}

// Kompaktne versioonitud fail, vigaseid/pooleli kirjeid ei rakendata.
void saveSettings(){
 if(!fsOK)return;
 File f=LittleFS.open(SETTINGS_FILE,"w");if(!f){Serial.println("Salvestamine ebaõnnestus");return;}
 f.printf("2 %u %u %u %u %lu %u\n",s.mode,s.brightness,s.speed,s.strength,(unsigned long)s.color,s.power?1:0);
 f.close(); Serial.println("Seadistused salvestatud");
}
void loadSettings(){
 if(!fsOK||!LittleFS.exists(SETTINGS_FILE))return;
 File f=LittleFS.open(SETTINGS_FILE,"r");if(!f)return;
 String line=f.readStringUntil('\n');f.close();
 unsigned int ver,mode,bri,spd,str,power;unsigned long color;
 if(sscanf(line.c_str(),"%u %u %u %u %u %lu %u",&ver,&mode,&bri,&spd,&str,&color,&power)!=7)return;
 if(ver!=2||mode>5||bri>255||spd<1||spd>100||str>100||color>0xFFFFFF||power>1)return;
 s.mode=mode;s.brightness=bri;s.speed=spd;s.strength=str;s.color=color;s.power=power;
}
String colorHex(uint32_t c){char b[7];snprintf(b,sizeof(b),"%06lX",(unsigned long)c);return String(b);}
void state(){
 String json="{\"mode\":"+String(s.mode)+",\"brightness\":"+String(s.brightness)+",\"speed\":"+String(s.speed)+",\"strength\":"+String(s.strength)+",\"color\":\""+colorHex(s.color)+"\",\"power\":"+String(s.power?1:0)+"}";
 server.send(200,"application/json",json);
}
void handleSet(){
 if(server.hasArg("mode"))s.mode=constrain(server.arg("mode").toInt(),0,5);
 if(server.hasArg("brightness"))s.brightness=constrain(server.arg("brightness").toInt(),0,255);
 if(server.hasArg("speed"))s.speed=constrain(server.arg("speed").toInt(),1,100);
 if(server.hasArg("strength"))s.strength=constrain(server.arg("strength").toInt(),0,100);
 if(server.hasArg("color")){
   String x=server.arg("color");bool valid=x.length()==6;
   for(unsigned int i=0;i<x.length();i++)if(!isxdigit((unsigned char)x[i]))valid=false;
   if(valid)s.color=strtoul(x.c_str(),nullptr,16);
 }
 if(server.hasArg("power"))s.power=server.arg("power")=="1";
 phase=0;lastChange=millis();savePending=true;immediateFrame=true;
 server.send(200,"text/plain","OK");
}
void setup(){
 Serial.begin(115200);
 strip.begin();strip.clear();strip.show();
 randomSeed(ESP.getCycleCount());fsOK=LittleFS.begin();loadSettings();
 WiFi.persistent(false);WiFi.mode(WIFI_AP);WiFi.softAP(AP_SSID,AP_PASSWORD);
 server.on("/",HTTP_GET,[](){server.send_P(200,"text/html; charset=utf-8",PAGE);});
 server.on("/state",HTTP_GET,state);
 server.on("/set",HTTP_GET,handleSet);
 server.onNotFound([](){server.send(404,"text/plain","Not found");});
 server.begin();Serial.printf("SSID: %s | IP: %s\n",AP_SSID,WiFi.softAPIP().toString().c_str());
 lastFrame=millis();render(0.033f);immediateFrame=false;
}
void loop(){
 server.handleClient();
 uint32_t now=millis();
 if(savePending&&now-lastChange>=3000){saveSettings();savePending=false;}
 // Staatilises režiimis ei kuluta asjatult aega LED-kaadrite saatmisele.
 bool animated=s.power&&s.mode!=STATIC_LIGHT;
 if(immediateFrame||(animated&&now-lastFrame>=33)){
   float dt=clampf((now-lastFrame)/1000.f,0.f,0.2f);lastFrame=now;
   render(dt);immediateFrame=false;
 }
 yield();
}
