#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define LED 16
#define PIN_TRIG D0
#define PIN_ECHO D1

float tiempo;
float distancia;
int a = 0;// plaza 1
int aold=0; // estado anterior

WiFiClient client;

const char* ssid     = "ELBA CABARCAS";
const char* password = "32682418Am";

const int httpPort = 80;
const char* host ="www.google.com";

HTTPClient http;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(10);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);


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


void enviarDato(String numero){
  String URL = "http://automated-parking.herokuapp.com/guardarDato/";
  URL = URL + numero;

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
          Serial.println(payload);

          if(payload == "0"){
            digitalWrite(LED, LOW);
          }

          if(payload == "1"){
            digitalWrite(LED, HIGH);
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


void loop() {
  digitalWrite(PIN_TRIG, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
   
  digitalWrite(PIN_TRIG, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
   
  tiempo = pulseIn(PIN_ECHO, HIGH);
  distancia = tiempo/58.3;

  Serial.println(distancia);
  delay(1000);

  if ( (distancia >=5)&(distancia <=20)){ // definimos la ventana de operacion del sensor
    a = 1;// a tomara valor de 1 que indicara plaza ocupada.
    Serial.println("PLAZA OCUPADA");
//    enviarDato(String (a));
  }
  else{
    a = 0; //indicara plaza libre
    Serial.println("PLAZA VACIA");
//    enviarDato(String (a));
  }
if (a != aold){// ponemos un condicional de si a es diferente de aold 
  enviarDato(String (a));// envie "a" a la tabla
 }
 aold = a;  //le damos el nuevo valor a aold, el cual es el anterior de a 
}
