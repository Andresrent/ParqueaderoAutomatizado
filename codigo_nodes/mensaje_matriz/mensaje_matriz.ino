/*
NodeMCU pines    -> Matrix pines
MOSI-D7-GPIO13   -> DIN (Dato de entrada)
CLK-D5-GPIO14    -> Clk ()
GPIO0-D3         -> CS o LOAD (Pin de matriz)
*/

#include <ESP8266WiFi.h> // Wifi
#include <ESP8266WebServer.h> // Convertir a servidor web
#include <SPI.h> // 
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#define SSID "ELBA CABARCAS"                      // insertar  SSID
#define PASS "32682418Am"                    // insertar  password
//Serial.begin(115200);                           // full speed to monitor

// ******************* Se define el encabezado de la página client-browser ************************************
String form =
"<!DOCTYPE html>"
 "<head>"
"<title>ARQUITECTURA</title>"
"<meta charset='utf-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<style>"
"* {"
  "box-sizing: border-box;"
"}"
"/* Style the header Titulo*/"
"header {"
 " background-color: #468;"
  "padding: 30px;"
  "text-align: center;"
  "font-size: 35px;"
  "color: white;"
"}"

"/* Create two columns/boxes that floats next to each other */"
"nav {"
 " float: left;"
  "width: 30%;"
  "height: 300px; /* only for demonstration, should be removed */"
  "background: #E6E6FA;"
  "padding: 20px;"
"}"
"/* Style the list inside the menu */"
"nav ul {"
 " list-style-type: none;"
  "padding: 0;"
"}"
"article {"
  "float: left;"
  "padding: 20px;"
  "width: 70%;"
  "background-color: #f1f1f1;"
  "height: 300px; /* only for demonstration, should be removed */"
"}"
"/* Clear floats after the columns */"
"section:after {"
  "content: "";"
  "display: table;"
  "clear: both;"
"}"
"/* Style the footer */"
"footer {"
  "background-color: #468;"
  "padding: 10px;"
  "text-align: center;"
  "color: white;"
"}"
"/* Responsive layout - makes the two columns/boxes stack on top of each other instead of next to each other, on small screens */"
"@media (max-width: 600px) {"
 " nav, article {"
    "width: 100%;"
    "height: auto;"
  "}"
"}"
"</style>"
"</head>"
"<body>"
"<header>"
  "<h1>Pantalla LED para publicidad</h1>"
"</header>"
"<section>"
  "<nav>"
    "<ul>"
    "<center>"
    "<img src='http://www.visionpublicitaria.com.mx/wp-content/uploads/2016/10/vision-publicitaria-logo-600.png' width='170' height='150'></center>"
      "<h3>Objetivo</h3>"
"<p>Facilitar que un producto o servicio se dé a conocer mediante pantalla de led, que ofrecen la posibilidad de transmitir el mensaje publicitario a pie de la calle.</p>"
      "</ul>"
  "</nav>"
  "<article>"
    "<form action='msg'><br>Ingresa mensaje<br><input type='text' name='msg'size=40 autofocus><br><br><input type='submit' value='Enviar'></form>"
  "</article>"
"</section>"
"<footer>"
  "<h4>____________________________</h4>"
  "<h4>Ingenieria Electronica</h4>"
  "<h3>By AUTOMATED PARKING</h3>"
  "<h4>_____________________________________</h4>"
  
"</footer>"
"</body>";

ESP8266WebServer server(80);                             // HTTP servidor usando puerto 80
long period;
int offset=1,refresh=0;
int pinCS = 0; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 3;
String decodedMsg;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
//Serial.println(decodeMsg);

String tape = "Arduino";
int wait = 45; // Tiempo en espera en milisegundos
int spacer = 2; // Espacio que existe entre caracter y caracter
int width = 5 + spacer; // Ancho del caracter son 5 pixels

/*
 * maneja los mensajes que provienen del navegador web, restaura algunos caracteres especiales 
 * y construye las cadenas que se pueden enviar a la pantalla Oled.
*/
void handle_msg() {
                        
  matrix.fillScreen(LOW);
  server.send(20, "text/html", form);    // Enviar la misma página para que puedan enviar otro mensaje
  refresh=1;
  // Mostrar mensaje en Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  decodedMsg = msg;
  // Restaurar caracteres especiales malformados a %char por el navegador del cliente
  decodedMsg.replace("+", " ");      
  decodedMsg.replace("%21", "!");  
  decodedMsg.replace("%22", "");  
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");  
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");  
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");  
  decodedMsg.replace("%2C", ",");  
  decodedMsg.replace("%2F", "/");   
  decodedMsg.replace("%3A", ":");    
  decodedMsg.replace("%3B", ";");  
  decodedMsg.replace("%3C", "<");  
  decodedMsg.replace("%3D", "=");  
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");  
  decodedMsg.replace("%40", "@");
  decodedMsg.replace("%164", "~");  
  Serial.println(decodedMsg);                   // impresion en monitor serial 
 
  Serial.println(' ');                          // nueva linea en monitor serial
}

void setup(void) {
matrix.setIntensity(10); // Intensidad del brillo entre 0 a 15

// Adjust to your own needs
//  matrix.setPosition(0, 1, 0); // La primera pantalla está en <0, 0>
//  matrix.setPosition(1, 0, 0); // La segunda pantalla está en <1, 0>

// Adjust to your own needs
  matrix.setPosition(0, 7, 0); // La primera pantalla está en <0, 7>
  matrix.setPosition(1, 6, 0); // La segunda pantalla está en <1, 0>
  matrix.setPosition(2, 5, 0); // La tercera pantalla está en <2, 0>
  matrix.setPosition(3, 4, 0); // And the last display is at <3, 0>
  matrix.setPosition(4, 3, 0); // La primera pantalla está en <0, 0>
  matrix.setPosition(5, 2, 0); // La segunda pantalla está en <1, 0>
  matrix.setPosition(6, 1, 0); // La tercera pantalla está en <2, 0>
  matrix.setPosition(7, 0, 0); //         Y la ultima está en <3, 0>

// La primera pantalla se coloca boca abajo
  matrix.setRotation(0, 3);    
  matrix.setRotation(1, 3);    
  matrix.setRotation(2, 3);    
  matrix.setRotation(3, 3);    
  matrix.setRotation(4, 3);    
  matrix.setRotation(5, 3);    
  matrix.setRotation(6, 3);    
  matrix.setRotation(7, 3);    
  




//ESP.wdtDisable();                               // used to debug, disable wachdog timer, 
  Serial.begin(115200);                           // full speed to monitor
                               
  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);                  // And as regular external functions:
  server.begin();                                 // Start the server 


  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor             
 
  char result[16];
  sprintf(result, "%3d.%3d.%1d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  Serial.println();
  Serial.println(result);
  decodedMsg = result;
  Serial.println("WebServer ready!   ");

  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
  Serial.print(analogRead(A0));
  
}


void loop(void) {

  for ( int i = 0 ; i < width * decodedMsg.length() + matrix.width() - 1 - spacer; i++ ) {
    server.handleClient();                        // checks for incoming messages
    if (refresh==1) i=0;
    refresh=0;
    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
 
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < decodedMsg.length() ) {
        matrix.drawChar(x, y, decodedMsg[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}
