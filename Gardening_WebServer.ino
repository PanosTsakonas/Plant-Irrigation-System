#include<Wire.h>
#include<WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid="Enter your router's SSID name"; //Enter your router's SSID
const char* pass= "Enter your router's password"; // Enter your router's password
const char* ssid1 = "Irrigation System";  // Enter SSID for Web Server here
const char* password1= "12345678";  //Enter Password for Web Server here
/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WebServer server(80);
WiFiClient  client;
const char* thingspeak_key = "Enter your thingspeak's Write API key";
#define dhtpin 23
DHT dht(dhtpin,DHT22);
bool pinstatus = LOW;
float air_hum=0;
float temp=0;
float soil_hum=0;
int n=0;
float a=2000;
int i=0;
int l=0;
String formattedDate;
String dayStamp;
int timeStamp;
int timeStamp1;
String mo;
String te="";
String ai="";
String s="";
int split1;
int split2;
float counter=0;
int wa=13;
int soil=36;
float kol=0;
#define soilinput 18
int days=3600;
float x[2];
float soil_hum1=0;
float soil_hum2=0;
TaskHandle_t Task1;

void setup() {
Wire.begin();
Serial.begin(9600);
delay(500);
WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("Connecting to WiFi");
  delay(1000);
  }
  Serial.println("Connected to WiFi");
  digitalWrite(LED_BUILTIN,HIGH);
delay(2000);
WiFi.softAP(ssid1, password1);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/systemon", handle_systemon);
  server.on("/systemoff", handle_systemoff);
  server.onNotFound(handle_NotFound);
  
  server.begin();

   xTaskCreatePinnedToCore(
                    irrigation,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 
  pinMode(wa,OUTPUT);
digitalWrite(wa,HIGH);
timeClient.begin();
timeClient.setTimeOffset(3*3600); // change your time offset based on where you live. The time zone is GMT so you have to know what zone you are in
pinMode(soilinput,OUTPUT);
digitalWrite(soilinput,LOW);
pinMode(soil,INPUT);
delay(2000);
 while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
timeStamp=timeClient.getHours();
timeStamp1=timeClient.getMinutes();
Serial.print(timeStamp);
Serial.print(":");
Serial.print(timeStamp1);
Serial.println("");
formattedDate = timeClient.getFormattedDate();

split1 = formattedDate.indexOf("-");
dayStamp=formattedDate.substring(split1 + 1);
split2=dayStamp.indexOf("-");
mo=dayStamp.substring(0,split2);
split1 = mo.indexOf("-");
dayStamp=formattedDate.substring(8,split1);
split2=dayStamp.indexOf("T");
dayStamp=dayStamp.substring(0,split2);
Serial.println(dayStamp);
Serial.println(mo);
delay(2000);
digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
  
   server.handleClient();
  if(pinstatus)
  {
    digitalWrite(wa, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(wa, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  }
}


void handle_systemoff() {
  pinstatus = LOW;
  digitalWrite(wa,HIGH);
  digitalWrite(LED_BUILTIN,LOW);
  digitalWrite(soilinput,HIGH);
   for(i=0;i<=20;i++)
  {
    kol=kol+analogRead(soil)*100/a;
  }
  while(digitalRead(wa)==0)
{ 
digitalWrite(wa,HIGH);
}
  kol=kol/20;
  s=+kol;
  kol=0;
  Serial.println("Irrigation System Status: OFF");
  server.send(200, "text/html", SendHTML(false)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_systemon() {
  pinstatus = HIGH;
  digitalWrite(wa,LOW);
  digitalWrite(LED_BUILTIN,HIGH);
  dht.begin();
  temp=dht.readTemperature();
  air_hum=dht.readHumidity();
  te=+temp;
  ai=+air_hum;
  Serial.println("Irrigation System Status: ON");
  
  server.send(200, "text/html", SendHTML(true)); 
}
void handle_OnConnect() {
  pinstatus = LOW;
  digitalWrite(wa,HIGH);
  digitalWrite(LED_BUILTIN,LOW);
  digitalWrite(soilinput,HIGH);
  for(i=0;i<=20;i++)
  {
    kol=kol+analogRead(soil)*100/a;
  }
  while(digitalRead(wa)==0)
{ 
digitalWrite(wa,HIGH);
}
  kol=kol/20;
  s=+kol;
  kol=0;
  Serial.println("Irrigation System Status: OFF");
  server.send(200, "text/html", SendHTML(pinstatus)); 
}
String SendHTML(uint8_t pinst)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Irrigtion System</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 09px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h3>Soil Humidity: "+s+" %"+"</h3>\n";
  
  if(pinst)
  {ptr +="<p>System Status: ON</p><a class=\"button button-off\" href=\"/systemoff\">OFF</a>\n";
   ptr+="<h3>Temperature: "+te+" C"+"</h3>\n";  
   ptr +="<h3>Air Humidity: "+ai+" %"+"</h3>\n";
  }
  else
  {ptr +="<p>System Status: OFF</p><a class=\"button button-on\" href=\"/systemon\">ON</a>\n";}
  ptr +="<h3>Soil Humidity: "+s+" %"+"</h3>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void irrigation( void * parameter)
{
  for(;;)
  {
   timeStamp=timeClient.getHours();
if (timeStamp==16)
{
  l=0;
}
if(timeStamp==17)
{
  if(l==0)
  {
  digitalWrite(soilinput,HIGH);
  digitalWrite(LED_BUILTIN,HIGH);
delay(200);
air_hum=0;
temp=0;
counter=0;
l=1;
  for(i=1;i<=100;i++)
  {
counter=counter+analogRead(soil);
  delay(100);
  }
  digitalWrite(soilinput,LOW);
  soil_hum=counter/100;
  soil_hum=soil_hum*100/a;
  Serial.print(soil_hum);
  Serial.print(" %");
  Serial.println("");
  Serial.println("Let's start watering the plants");
i=0;
timeStamp1=timeClient.getMinutes();
  while(soil_hum<80)
  {
    digitalWrite(wa,LOW); 
    delay(60000*5);
    digitalWrite(soilinput,HIGH);
     soil_hum1=analogRead(soil);
    soil_hum=soil_hum1*100/a;
    x[i]=soil_hum;
    delay(200);
     Serial.print(soil_hum);
  Serial.print(" %");
  Serial.println("");
    digitalWrite(soilinput,LOW);
    i=i+1;
    if (i==2)
    {
      soil_hum2=(x[0]+x[1])/2;
      soil_hum2=soil_hum2*100/a;
    
    if (soil_hum2 <=10)
    {
      Serial.println("Replace Soil Sensor");
    }
  }
   if (timeClient.getMinutes()-timeStamp1>=11)
    {
      soil_hum=1000;
    }
  }
  delay(400);
  Serial.println(digitalRead(wa));
while(digitalRead(wa)==0)
{
  
digitalWrite(wa,HIGH);
Serial.println(digitalRead(wa));
}
delay(4000);
if (soil_hum>100 and soil_hum<1000)
{
  a=(a+soil_hum1)/2;
  Serial.println(a);
}
dht.begin();
delay(200);
for(i=1;i<=50;i++)
{
  air_hum=air_hum+dht.readHumidity();
  temp=temp+dht.readTemperature();
}
 air_hum=air_hum/50;
  temp=temp/50;
 Serial.print(air_hum);
 Serial.print(" %");
 Serial.println("");
  Serial.print(temp);
  Serial.print(" C");
  Serial.println("");
  
  
WiFiClient client;
  if (!client.connect("api.thingspeak.com", 80)) {
    return;
  }
  String url = "/update?key=";
  url += thingspeak_key;
  url += "&field1=";
  url +=  temp;
  url += "&field2=";
  url += air_hum;
  url += "&field3=";
   url += soil_hum;
    url += "&field4=";
   url += soil_hum1;
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.thingspeak.com\r\n" + 
               "Connection: close\r\n\r\n");
  client.stop();
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
}
}

WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi");
  delay(1000);
  }
  Serial.println("Connected to WiFi");
delay(2000); 
  }
 
}
