#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>

/*

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192,168,0,19);

// fill in your Domain Name Server address here:
IPAddress myDns(1,1,1,1);

char server[] = "www.google.com";

void setup() {
  // start serial port:
  Serial.begin(9600);
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

*/



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDA, 0xBE, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 19);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

//char sensiServer[] = "bussrvstg.sensicomfort.com";
//char sensiPage[] = "/api/authorize";
//char sensiAuthorizeJSON[] = "{\"UserName\":\"team19@globalhack.com\",\"Password\":\"globalhack\"}";
char sensiServer[] = "bussrvstg.sensicomfort.com/api/authorize";
char sensiPage[] = "/";
char sensiAuthorizeJSON[] = "UserName=team19@globalhack.com&Password=globalhack";

// incoming/outgoing client instances:
//EthernetClient incomingClient;
EthernetClient outgoingClient;

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
  
  // get authorization token for the Sensi thermostat
  httpPostRequest();
  //httpGetRequest();
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
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            incomingClient.print("analog input ");
            incomingClient.print(analogChannel);
            incomingClient.print(" is ");
            incomingClient.print(sensorReading);
            incomingClient.println("<br />");
          }
          incomingClient.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
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
  
  
  // if there's incoming data from our outgoing client net connection.
  // send it to the serial port. 
  EthernetClient outgoingClient = server.available();
  if (outgoingClient.available()) {
    char c = outgoingClient.read();
    Serial.print(c);
  }

//  // if there's no net connection, but there was one last time
//  // through the loop, then stop the client:
//  if (!outgoingClient.connected() && lastConnected) {
//    Serial.println();
//    Serial.println("disconnecting.");
//    outgoingClient.stop();
//  }

//  // if you're not connected, and ten seconds have passed since
//  // your last connection, then connect again and send data:
//  if(!outgoingClient.connected() && (millis() - lastConnectionTime > postingInterval)) {
//    httpRequest();
//  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = outgoingClient.connected();

  
}


// this method makes a HTTP connection to the server:
void httpGetRequest() {
  // if there's a successful connection:
  if (outgoingClient.connect(sensiServer, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    outgoingClient.println("GET /latest.txt HTTP/1.1");
    outgoingClient.println("Host: www.google.com");
    outgoingClient.println("User-Agent: arduino-ethernet");
    outgoingClient.println("Connection: close");
    outgoingClient.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    outgoingClient.stop();
  }
}


// this method makes a HTTP connection to a server:
void httpPostRequest() {
  // if there's a successful connection:
  EthernetClient outgoingClient = server.available();
  if (outgoingClient.connect(sensiServer, 80)) {
    Serial.print("Connected to sensiServer... ");
    Serial.print(sensiServer);
    Serial.println(" on port 80");
    // send the HTTP POST request:
    Serial.println("Sending authorization request...");
    char outBuf[128];
    sprintf(outBuf,"POST %s HTTP/1.2",sensiPage);
    outgoingClient.println(outBuf);
    Serial.println(outBuf);
    sprintf(outBuf,"Host: %s",sensiServer);
    outgoingClient.println(outBuf);
    Serial.println(outBuf);

//    outgoingClient.println("POST  HTTP/1.2");
//    Serial.println("POST /latest.txt HTTP/1.2");
//    outgoingClient.println("Host: bussrvstg.sensicomfort.com/api/authorize");
//    Serial.println("Host: bussrvstg.sensicomfort.com/api/authorize");
    outgoingClient.println("Accept:application/json;version=1");
    Serial.println("Accept:application/json;version=1");
    outgoingClient.println("Connection: close");
    Serial.println("Connection: close");
    //outgoingClient.println("User-Agent: Arduino/1.0");
    //Serial.println("User-Agent: Arduino/1.0");
    outgoingClient.println("Content-Type:application/json");
    Serial.println("Content-Type:application/json");
    sprintf(outBuf,"Content-Length: %u",strlen(sensiAuthorizeJSON));
    outgoingClient.println(outBuf);
    Serial.println(outBuf);
    outgoingClient.println();
    Serial.println();
    outgoingClient.print(sensiAuthorizeJSON);
    Serial.print(sensiAuthorizeJSON);
    outgoingClient.println();
    Serial.println();
    

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed to sensiServer");
    Serial.println("disconnecting.");
    outgoingClient.stop();
  }
  
  
  
}
