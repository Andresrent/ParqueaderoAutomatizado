#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


#define LED 16
#define segA D1
#define segB D2
#define segC D3
#define segD D4
#define segE D5
#define segF D6
#define segG D7
int x = 0;
int estadoPlaza[2];

WiFiClient client;

const char* ssid     = "ELBA CABARCAS";
const char* password = "32682418Am";

const int httpPort = 80;
const char* host ="automated-parking.herokuapp.com";

HTTPClient http;

void setup() {
  pinMode (segA, OUTPUT);
  pinMode (segB, OUTPUT);
  pinMode (segC, OUTPUT);
  pinMode (segD, OUTPUT);
  pinMode (segE, OUTPUT);
  pinMode (segF, OUTPUT);
  pinMode (segG, OUTPUT);
 
  pinMode(LED, OUTPUT);
  
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 
 
  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);
   Serial.print('.');
  }
 
  // Mostrar mensaje de exito y dirección IP asignada
  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());


  Serial.print("[HTTP] begin...\n");
}


void consulta(String numero){
  String URL;
//  if(numero=="999"){
    URL = "http://automated-parking.herokuapp.com/consultarPlaza/todas";
//  }
//  else{
//    URL = "http://automated-parking.herokuapp.com/consultarPlaza/";
//    URL = URL + numero;
//  }

  Serial.println(URL);
  
  if (http.begin(client, URL)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
//          Serial.println(payload);
          //char json[] = payload;

          DynamicJsonDocument doc(1024);
          deserializeJson(doc, payload);

          for(int i=0;i<=1;i++){
            String estado_c = doc[String(i)]["estado"];
            Serial.print("Estado plaza ");
            Serial.print(String(i+1));
            Serial.print(" :");
            Serial.println(estado_c);
            estadoPlaza[i+1]=estado_c.toInt();
          }
          

          
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
}


// the loop function runs over and over again forever
void loop() {
  // read from port 1, send to port 0:
//  if (Serial.available()) {
    String numero=Serial.readString();
    unsigned int index = numero.length();
    numero.remove(index-1);
    
    Serial.println(numero);
    consulta(numero);
    x= (estadoPlaza[1]+ estadoPlaza[2]);
    segmentos(x);
     Serial.print("x es: ");
     Serial.print (x);
//  }
}








void segmentos(int x){                                            
  if(x==0){
digitalWrite (segA, HIGH);
digitalWrite (segB, HIGH);
digitalWrite (segC, HIGH);
digitalWrite (segD, HIGH);
digitalWrite (segE, HIGH);
digitalWrite (segF, HIGH);
digitalWrite (segG, LOW);
  }
  else if(x == 1){
digitalWrite (segA, LOW);
digitalWrite (segB, HIGH);
digitalWrite (segC, HIGH);
digitalWrite (segD, LOW);
digitalWrite (segE, LOW);
digitalWrite (segF, LOW);
digitalWrite (segG, LOW);
  }
else if (x == 2){
digitalWrite (segA, HIGH);
digitalWrite (segB, HIGH);
digitalWrite (segC, LOW);
digitalWrite (segD, HIGH);
digitalWrite (segE, HIGH);
digitalWrite (segF, LOW);
digitalWrite (segG, HIGH);
 }
else if (x == 3){
digitalWrite (segA, HIGH);
digitalWrite (segB, HIGH);
digitalWrite (segC, LOW);
digitalWrite (segD, HIGH);
digitalWrite (segE, HIGH);
digitalWrite (segF, LOW);
digitalWrite (segG, HIGH);
 }
else if (x == 4){
digitalWrite (segA, LOW);
digitalWrite (segB, HIGH);
digitalWrite (segC, HIGH);
digitalWrite (segD, LOW);
digitalWrite (segE, LOW);
digitalWrite (segF, HIGH);
digitalWrite (segG, HIGH);
 }
else if (x == 5){
digitalWrite (segA, HIGH);
digitalWrite (segB, LOW);
digitalWrite (segC, HIGH);
digitalWrite (segD, HIGH);
digitalWrite (segE, LOW);
digitalWrite (segF, HIGH);
digitalWrite (segG, HIGH);
 }
else if (x == 6){
digitalWrite (segA, LOW);
digitalWrite (segB, LOW);
digitalWrite (segC, HIGH);
digitalWrite (segD, HIGH);
digitalWrite (segE, HIGH);
digitalWrite (segF, HIGH);
digitalWrite (segG, HIGH);
 }
else if (x == 7){
digitalWrite (segA, HIGH);
digitalWrite (segB, HIGH);
digitalWrite (segC, HIGH);
digitalWrite (segD, LOW);
digitalWrite (segE, LOW);
digitalWrite (segF, LOW);
digitalWrite (segG, LOW);
 }
else if (x == 8){
digitalWrite (segA, HIGH);
digitalWrite (segB, HIGH);
digitalWrite (segC, HIGH);
digitalWrite (segD, HIGH);
digitalWrite (segE, HIGH);
digitalWrite (segF, HIGH);
digitalWrite (segG, HIGH);
 }
else if (x == 9){
digitalWrite (segA, HIGH);
digitalWrite (segB, HIGH);
digitalWrite (segC, HIGH);
digitalWrite (segD, LOW);
digitalWrite (segE, LOW);
digitalWrite (segF, HIGH);
digitalWrite (segG, HIGH);
 }
}
