#include <SPI.h>
#include <WiFi.h>


//  Hue constants
const char hueHubIP[] = "192.168.1.6";
const char hueUsername[] = "my_hue_app#arduino_ebenezer";
const int hueHubPort = 80;

boolean activated = false;


//  Hue variables
boolean hueOn;
int hueBri;  // brightness value
long hueHue;  // hue value
String hueCmd;  // Hue command

unsigned long buffer=0;  //buffer for received data storage
unsigned long addr;

// Wireless network
char ssid[] = "ifiw_ym";
char pass[] = "mypassword";
int status = WL_IDLE_STATUS;
WiFiClient client;

//0:B2:8F:6A:C4:28

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Firmware needs upgrade");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.print("connected to the network");
  printCurrentNet();
  printWifiData();
}

void loop() {
  Serial.print("in loop");
  // printCurrentNet();
  alterHueColors();
}

void alterHueColors() {
  if(true){
    client.println("Hue is activated");
        Serial.print("activated ");

     // A series of four sample commands, which colour fades two lights between red and pink. Read up on the Hue API
    // documentation for more details on the exact commands to be used, but note that quote marks must be escaped.
    String command = "{\"on\": true,\"hue\": 50100,\"sat\":255,\"bri\":255,\"transitiontime\":"+String(random(15,25))+"}";
    setHue(3,command);
    command = "{\"on\": true,\"hue\": 65280,\"sat\":255,\"bri\":255,\"transitiontime\":"+String(random(15,25))+"}"; 
    setHue(4,command);
    command = "{\"hue\": 65280,\"sat\":255,\"bri\":255,\"transitiontime\":"+String(random(15,25))+"}";
    setHue(3,command);
    command = "{\"hue\": 50100,\"sat\":255,\"bri\":255,\"transitiontime\":"+String(random(15,25))+"}"; 
    setHue(4,command);
    
    activated = true;
  }
  else{
      activated = false;
      client.println("deactivated");
      Serial.print("not activated ");
      //was activated, so send a single off command
      String command = "{\"on\": false}";
      setHue(3,command);
      setHue(4,command);
  }
}

/* setHue() is our main command function, which needs to be passed a light number and a 
 * properly formatted command string in JSON format (basically a Javascript style array of variables
 * and values. It then makes a simple HTTP PUT request to the Bridge at the IP specified at the start.
 */
boolean setHue(int lightNum,String command) {
  if (client.connect(hueHubIP, hueHubPort))
  {
  while (status == WL_CONNECTED) {
    {
      client.print("PUT /api/");
      client.print(hueUsername);
      client.print("/lights/");
      client.print(lightNum);  // hueLight zero based, add 1
      client.println("/state HTTP/1.1");
      client.println("keep-alive");
      client.print("Host: ");
      client.println(hueHubIP);
      client.print("Content-Length: ");
      client.println(command.length());
      client.println("Content-Type: text/plain;charset=UTF-8");
      client.println();  // blank line before body
      client.println(command);  // Hue command
    }
    client.stop();
    return true;  // command executed
  }
  } else
    return false;  // command failed
}
/* A helper function in case your logic depends on the current state of the light. 
 * This sets a number of global variables which you can check to find out if a light is currently on or not
 * and the hue etc. Not needed just to send out commands
 */
boolean getHue(int lightNum) {
  if (client.connect(hueHubIP, hueHubPort))
  {
    client.print("GET /api/");
    client.print(hueUsername);
    client.print("/lights/");
    client.print(lightNum);  
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(hueHubIP);
    client.println("Content-type: application/json");
    client.println("keep-alive");
    client.println();
    while (client.connected())
    {
      if (client.available())
      {
        client.findUntil("\"on\":", "\0");
        hueOn = (client.readStringUntil(',') == "true");  // if light is on, set variable to true
 
        client.findUntil("\"bri\":", "\0");
        hueBri = client.readStringUntil(',').toInt();  // set variable to brightness value
 
        client.findUntil("\"hue\":", "\0");
        hueHue = client.readStringUntil(',').toInt();  // set variable to hue value
        
        break;  // not capturing other light attributes yet
      }
    }
    client.stop();
    return true;  // captured on,bri,hue
  }
  else
    return false;  // error reading on,bri,hue    
}


void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void printCurrentNet() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

