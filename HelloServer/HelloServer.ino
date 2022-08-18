#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
SoftwareSerial myserial(14,12);//RX=d5,TX=d6

int weight;
int kind_int;
bool sw; // 1 kind 0 weight
String kind;
int calorie,vc,fiber;

float vc_array[7]={2.2, 0, 0.9, 1.2,14.4, 2.7, 3.0};
float calorie_array[7]={1.12, 1.39, 1.6, 3.2, 6.2, 8.1, 1.5};
float fiber_array[7]={2.9, 0, 0.5, 3.2, 14.8, 1.1, 1.0};

#ifndef STASSID
#define STASSID "Mi 10p"          //WIFI SSID
#define STAPSK  "23401685"        //WIFI password
#endif
#define LED LED_BUILTIN
#define PIN_LIGHT1 D2
#define PIN_DATA1 A0
#define PIN_LIGHT2 D3
#define PIN_LIGHT3 D4
#define PIN_LIGHT4 D1


const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);  //server端口号

String html1 ="<!DOCTYPE html><head><meta charset=\"utf-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\"content=\"ie=edge\"><title>Food Calorie Analysis Platform</title></head><body><h1 align=\"center\">Food Calorie Analysis Platform</h1><script>function local(pin,staut){var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"/pin?pin=\"+pin+\"&light=\"+staut,true);xmlhttp.send()}</script><div><a href=\"#\"onclick=\"local(1,'on')\"><input type=\"button\"value=\"reset\"style=\"font-size: 24px; padding: 10px; margin: auto 10px ;\"></a><a href=\"#\"onclick=\"local(1,'off')\"><input type=\"button\"value=\"start\"style=\"font-size: 24px; padding: 10px; margin: auto 10px; float: right;\"></a></div><div id=\"main2\"style=\"width:80%;height:450px;margin: 150px \"align=\"center\"></div><script src=\"http://echarts.baidu.com/build/dist/echarts.js\"></script><script type=\"text/javascript\">require.config({paths:{echarts:'http://echarts.baidu.com/build/dist'}});require(['echarts','echarts/chart/bar','echarts/chart/line'],function(ec){var myChart2=ec.init(document.getElementById('main2'));var option2={title:{text:'Food Calorie',textStyle:{fontSize:14,fontStyle:'normal',fontWeight:'bold',},},tooltip:{trigger:'axis'},legend:{data:['Calorie','Vitamins','Dietary fiber']},toolbox:{show:true,feature:{dataView:{show:true,readOnly:false},magicType:{show:true,type:['line','bar']},restore:{show:true},saveAsImage:{show:true}}},calculable:true,xAxis:[{type:'category',data:['potato','tomato','Cucumber','corn','egg','reserved'],name:'category',position:'left'}],yAxis:[{type:'value',name:'value(s)',position:'left'}],series:[{name:'Calorie',type:'bar',data:[19.5,19.8,21.7,25.1,32.5,48.9],color:'#CC0066'},{name:'Vitamins',type:'bar',data:[15.0,16.6,17.3,17.4,19.6,22.7],color:'#009999'},{name:'Dietary fiber',type:'bar',data:[8.0,13.6,13.3,11.4,6.6,22.7],color:'#66cccc'}]};myChart2.setOption(option2)});</script></body>";

String add_object()
{
  String ptr = "<!DOCTYPE html>";
  
  ptr+="<head><meta charset=\"utf-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\"content=\"ie=edge\"><title>Food Calorie Analysis Platform</title></head><body><h1 align=\"center\">Food Calorie Analysis Platform</h1><h2 align=\"center\"style=\"margin: 50px\">";
  ptr+=kind;
  ptr+="</h2>";
  ptr+="<h3 align=\"center\">";
  ptr+=weight;
  ptr+="g";
  ptr+="</h3>";
  ptr+="<script>function local(pin,staut){var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"/pin?pin=\"+pin+\"&light=\"+staut,true);xmlhttp.send()}</script><div><a href=\"#\"onclick=\"local(1,'on')\"><input type=\"button\"value=\"start\"style=\"font-size: 24px; padding: 10px; margin: auto 10px ;\"></a><a href=\"#\"onclick=\"local(1,'off')\"><input type=\"button\"value=\"reset\"style=\"font-size: 24px; padding: 10px; margin: auto 10px; float: right;\"></a></div><div id=\"main2\"style=\"width:70%;height:450px;margin:70px auto auto auto \"></div><script src=\"http://echarts.baidu.com/build/dist/echarts.js\"></script><script type=\"text/javascript\">require.config({paths:{echarts:'http://echarts.baidu.com/build/dist'}});require(['echarts','echarts/chart/bar','echarts/chart/line'],function(ec){var myChart2=ec.init(document.getElementById('main2'));var option2={title:{text:'Food Calorie',textStyle:{fontSize:14,fontStyle:'normal',fontWeight:'bold',},},tooltip:{trigger:'axis'},legend:{data:['Calorie','Vitamins','Dietary fiber']},toolbox:{show:true,feature:{dataView:{show:true,readOnly:false},magicType:{show:true,type:['line','bar']},restore:{show:true},saveAsImage:{show:true}}},calculable:true,xAxis:[{type:'category',data:[' '],name:'category',position:'left'}],yAxis:[{type:'value',name:'value(s)',position:'left'}],series:[{name:'Calorie',type:'bar',data:[";
  ptr+=calorie ;
  ptr+="],color:'#CC0066'},{name:'Vitamins',type:'bar',data:[";
  ptr+=vc;
  ptr+="],color:'#009999'},{name:'Dietary fiber',type:'bar',data:[";
  ptr+=fiber;
  ptr+="],color:'#66cccc'}]};myChart2.setOption(option2)});</script></body>";
  return ptr;
}


//-----------控制light fun------//
void pin() {
  String data = server.arg("pin");
  switch(data[0]) {
    case '1':
      controller(PIN_LIGHT1);break;
    case '2':
      controller(PIN_LIGHT2);break;
    case '3':
      controller(PIN_LIGHT3);break;
    case '4':
      controller(PIN_LIGHT4);break;
  }
}
void controller(int Pin){
  String json = "{\"mag\":\"OK\"}";
 // if (server.arg("light") == "on") {
//    myservo.write(servoon);
 //   server.send(200, "text/plain", json);
    Serial.println("D2 High");
    digitalWrite(Pin,HIGH);
    delay(1);
    Serial.println("D2 LOW");
    digitalWrite(Pin,LOW);
    digitalWrite(LED,HIGH);
    delay(500);
    digitalWrite(LED,LOW); 
}


//----------data API-----------//
void data(){
  String json = "";
  int value = analogRead(PIN_DATA1);
  int data = map(value, 0, 1024, 0, 20000);
  json = String(data);
  server.send(200,"text/plain",json);
}

//--------返回html文件-----------//
void handleRoot() {
  responseHtml(html1);
}

void responseHtml(String html){
  digitalWrite(LED,LOW);
  server.send(200, "text/html", add_object());
  digitalWrite(LED,HIGH);
}


//----------响应信息--------------//
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(PIN_LIGHT,HIGH);
}

//-----------更新数据-------------//
void update_data(int mkind_int, int mweight)
{
   switch(mkind_int)
   {
    case 1: kind = "Corn";calorie=mweight*calorie_array[0];vc=mweight*vc_array[0];fiber=mweight*fiber_array[0];break;
    case 2: kind = "Egg";calorie=mweight*calorie_array[1];vc=mweight*vc_array[1];fiber=mweight*fiber_array[1];break;
    case 3: kind = "Cucumber";calorie=mweight*calorie_array[2];vc=mweight*vc_array[2];fiber=mweight*fiber_array[2];break;
    case 4: kind = "Carrot";calorie=mweight*calorie_array[3];vc=mweight*vc_array[3];fiber=mweight*fiber_array[3];break;
    case 5: kind = "Chili";calorie=mweight*calorie_array[4];vc=mweight*vc_array[4];fiber=mweight*fiber_array[4];break;
    case 6: kind = "Potato";calorie=mweight*calorie_array[5];vc=mweight*vc_array[5];fiber=mweight*fiber_array[5];break;
    case 7: kind = "Tomato";calorie=mweight*calorie_array[6];vc=mweight*vc_array[6];fiber=mweight*fiber_array[6];break;
    default: kind=" ";calorie = vc = fiber = 0;
   }
}
void setup(void) {
//------------初始化------------//  

  pinMode(PIN_LIGHT1, OUTPUT);
  pinMode(PIN_LIGHT2, OUTPUT);
  pinMode(PIN_LIGHT3, OUTPUT);
  pinMode(PIN_LIGHT4, OUTPUT);
  
  pinMode(PIN_DATA1, INPUT);
  pinMode(LED,OUTPUT);
  myserial.begin(9600); 
  Serial.begin(9600);
  sw = 1;
  weight = 0;
  kind_int = 0;
//-----------WIFI模式-------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println(WiFi.softAPIP());
  Serial.println("");

//------------检查连接----------------
  while (WiFi.status() != WL_CONNECTED) {
//    break;
    delay(200);
    Serial.print(".");
    digitalWrite(LED, HIGH);
    delay(200);
    Serial.print(".");
    digitalWrite(LED, LOW);
  }
  //WIFI connection 
  digitalWrite(LED, LOW);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
///******************************************///
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

//-------------路由------------------
  server.on("/", handleRoot);        //主页
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  
//-------------响应-----------------
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  // 判断软串口是否传输数据进来，如有则进行分析
  if ( myserial.available()>0 ) {  
    int pos = myserial.parseInt();   // 解析软串口数据中的整数信息并赋值给变量pos
    if(pos!=0)
    {
    Serial.print("data from e203:");
    Serial.println(pos);
    kind_int = (int)(pos/1000);
    weight   = pos - 1000*kind_int;
    Serial.print("[info] weight from E203: ");
    Serial.println(weight);           
    Serial.print("[info] kind from E203: ");
    Serial.println(kind_int);
    update_data(kind_int,weight);
    responseHtml(html1);  
    }
    delay(15);
  }
}
