#include<Wire.h>
#include<WiFi.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid="Enter your router's SSID name";
const char* pass= "Enter your routers password";
WiFiClient  client;
const char* thingspeak_key = "Enter your thingspeak's Write API key";
#define dhtpin 23
DHT dht(dhtpin,DHT22);
float air_hum=0;
float temp=0;
float soil_hum=0;
int n=0;
RTC_DATA_ATTR float a=2000;
int i=0;
String formattedDate;
String dayStamp;
int timeStamp;
int timeStamp1;
String mo;
int split1;
int split2;
float counter=0;
int wa=13;
int soil=36;
#define soilinput 18
int microsec = 1000000;
int days=3600;
float x[2];
float soil_hum1=0;
float soil_hum2=0;

void setup() 
{
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
dht.begin();
delay(200);
pinMode(wa,OUTPUT);
digitalWrite(wa,HIGH);
timeClient.begin();
timeClient.setTimeOffset(3*3600); // change your time offset based on where you live. The time zone is GMT so you have to know what zone you are in
pinMode(soilinput,OUTPUT);
digitalWrite(soilinput,LOW);
digitalWrite(LED_BUILTIN,HIGH);
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
if(timeStamp!=17)
{
 
 timeStamp1=timeClient.getMinutes();
    if(timeStamp1<=30)
    timeStamp1=1800-60*timeStamp1+20;
    else
    timeStamp1=-60*(timeStamp1-30)+1800+20;
    esp_deep_sleep(microsec*timeStamp1);
}

 if(timeStamp==17)

{
digitalWrite(soilinput,HIGH);
delay(200);
air_hum=0;
temp=0;
counter=0;
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
   url += n;
    url += "&field5=";
   url += soil_hum1;
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.thingspeak.com\r\n" + 
               "Connection: close\r\n\r\n");
  client.stop();
  digitalWrite(LED_BUILTIN,LOW);
  delay(5000);
  timeStamp1=timeClient.getMinutes();
   timeStamp1=timeClient.getMinutes();
   if(timeStamp1<=30)
    timeStamp1=1800-60*timeStamp1+20;
    else
   timeStamp1=-60*(timeStamp1-30)+1800+20;
   esp_deep_sleep(microsec*timeStamp1);
  
  
}
}

void loop() 
{
 
}
