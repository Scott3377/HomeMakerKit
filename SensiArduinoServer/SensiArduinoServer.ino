#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDA, 0xBE, 0xDA, 0xBE, 0xDA, 0xBE
};
IPAddress ip(192, 168, 0, 19);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

/*
char sensiServer[] = "bussrvstg.sensicomfort.com";
char sensiPage[] = "/api/authorize";
char sensiAuthorizeJSON[] = "{\"UserName\":\"team19@globalhack.com\", \"Password\":\"globalhack\"}";
String sensiResponse;
String sensiToken;

char currentRequest;
*/

// incoming/outgoing client instances:
EthernetClient incomingClient;
//EthernetClient outgoingClient;

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 60*1000;  // delay between updates, in milliseconds

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // disable SD SPI
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server Arduino Uno started at IP address ");
  Serial.println(Ethernet.localIP());
  
  delay(2000);
  
}


void loop() {
  // maintain for DHCP
  //Ethernet.maintain();
  
  // listen for incoming clients
  EthernetClient incomingClient = server.available();
  if (incomingClient) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (incomingClient.connected()) {
      if (incomingClient.available()) {
        char c = incomingClient.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          incomingClient.println("HTTP/1.1 200 OK");
          incomingClient.println("Content-Type: text/html");
          incomingClient.println("Connection: close");  // the connection will be closed after completion of the response
          incomingClient.println("Refresh: 5");  // refresh the page automatically every 5 sec
          incomingClient.println();
          incomingClient.println("<!DOCTYPE HTML>");
          incomingClient.println("<html>");
          // output the value of each analog input pin
          incomingClient.println("Team XIX<p>");

          int window = analogRead(0);
          //digitalWrite(5, window);
          incomingClient.print("W=");
          incomingClient.print(window);
          incomingClient.print("<br />");

          int sensorCO = analogRead(1);
          //digitalWrite(6, sensorCO);
          incomingClient.print("C=");
          incomingClient.print(sensorCO);
          incomingClient.print("<br />");

          int door = analogRead(2);
          //digitalWrite(7, door);
          incomingClient.print("D=");
          incomingClient.print(door);
          incomingClient.print("<br />");

          incomingClient.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    incomingClient.stop();
    Serial.println("incomingClient disconnected");
  }
  
  // store the state of the connection for next time through
  // the loop:
  lastConnected = incomingClient.connected();
  
}






//   B O N E Y A R D

//// this method makes a HTTP connection to the server:
//void httpGetRequest() {
//  // if there's a successful connection:
//  if (outgoingClient.connect(sensiServer, 80)) {
//    Serial.println("connecting...");
//    // send the HTTP PUT request:
//    outgoingClient.println("GET /latest.txt HTTP/1.1");
//    outgoingClient.println("Host: www.google.com");
//    outgoingClient.println("User-Agent: arduino-ethernet");
//    outgoingClient.println("Connection: close");
//    outgoingClient.println();
//
//    // note the time that the connection was made:
//    lastConnectionTime = millis();
//  } 
//  else {
//    // if you couldn't make a connection:
//    Serial.println("connection failed");
//    Serial.println("disconnecting.");
//    outgoingClient.stop();
//  }
//}


/*
  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!outgoingClient.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
*/
