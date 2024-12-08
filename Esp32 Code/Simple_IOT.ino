#include<WiFi.h>
#include <FireEsp.h>
#include <ArduinoJson.h>

const int light=15;
const char* ssid = "ESP";
const char* password = "10101010#2G";
#define FIREBASE_API ""
#define FIREBASE_Domain ""
#define DATABASE_URL ""
FbServer server(FIREBASE_API,FIREBASE_Domain , DATABASE_URL);
FbAuthentication auth(server);
FbDatabase database(server);
StaticJsonDocument<200> lightinfo;
String lightinfo_string;
int prev_Light_state=0;


void lightON(){
  digitalWrite(light,0);
  //database.update("/light", "state", 1, auth.getIdToken());
}

void lightOFF(){
  digitalWrite(light,1);
  //database.update("/light", "state", 0, auth.getIdToken());
}

void setup() {
  // put your setup code here, to run once:
  pinMode(light,OUTPUT);
  Serial.begin(9600);
  ConnectToWifi();
  server.initialize();
  lightOFF();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("hello");
  lightinfo_string = database.get("/light", auth.getIdToken());
  deserializeJson(lightinfo, lightinfo_string);
  
  int light_state = lightinfo["state"];
 // Serial.println(light_state);
  if(light_state!=prev_Light_state){
    if(light_state) lightON();
    else lightOFF();
  }
  //delay(500);
  prev_Light_state=light_state;

  

}

void ConnectToWifi(){
  WiFi.begin(ssid,password);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(200);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
}



