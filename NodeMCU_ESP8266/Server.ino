//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
  #include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
    #define D0 16  //On Board LED [ RED ]
    #define D1 5
    #define D2 4
    #define D3 0
    #define D4 2   // WIFI Module LED [ BLUE ]
    #define D5 14
    #define D6 12
    #define D7 13     
//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       TKDssid;              // SERVER WIFI NAME
  char*       TKDpassword;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     1           // MAXIMUM NUMBER OF CLIENTS
  String Message

  /*
   * MAXSC => Maximum Server Clients
   * They Are 6 Because
   * 1. Blue Fighter Suit
   * 2. Red Fighter Suit
   * 3. Corner Referee 1
   * 4. Corner Referee 2
   * 5. Corner Referee 3
   * 6. Corner Referee 4
   */
  
  WiFiServer  TKDServer(9001);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  TKDClient[MAXSC];     // THE SERVER CLIENTS (Devices)
//=====Setup=====

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication
    
    pinMode(D4, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(D0, OUTPUT);          //On Board
    digitalWrite(D4, HIGH);       // Turn WiFi LED Off
    digitalWrite(D4, LOW);        // Turn on Board LED On 
  
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    // Setting Up A Wifi Access Point
    SetWifi("NodeMCU_Server", "");
  }

//====Loop====
  
  void loop()
  {
    // Checking For Available Clients
    AvailableClients();
    // Checking For Available Client Messages
    AvailableMessage();
    // 
  }

//==========Setting Up Access Point==========
  
  void SetWifi(char* Name, char* Password)
  {
    // Stop Any Previous WIFI
    WiFi.disconnect();

    // Setting The Wifi Mode
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WIFI Mode : AccessPoint Station");
    
    // Setting The AccessPoint Name & Password
    TKDssid      = Name;
    TKDpassword  = Password;
    
    // Starting The Access Point
    WiFi.softAP(TKDssid, TKDpassword);
    Serial.println("WIFI < " + String(TKDssid) + " > ... Started");
    
    // Wait For Few Seconds
    delay(100);
    
    // Getting Server IP
    IPAddress IP = WiFi.softAPIP();
    
    // Printing The Server IP Address
    Serial.print("AccessPoint IP : ");
    Serial.println(WiFi.softAPIP());

    // Printing MAC Address
    Serial.print("AccessPoint MC : ");
    Serial.println(WiFi.softAPmacAddress());

    // Starting Server
    TKDServer.begin();
    TKDServer.setNoDelay(true);
    Serial.println("Server Started");
  }

//=====Connected Clients=====

  void AvailableClients()
  {   
    if (TKDServer.hasClient())
    {
      // Read D4 Switch To Low If High.
      if(digitalRead(D4) == HIGH) digitalWrite(D4, LOW);
      
      for(uint8_t i = 0; i < MAXSC; i++)
      {
        //find free/disconnected spot
        if (!TKDClient[i] || !TKDClient[i].connected())
        {
          // Checks If Previously The Client Is Taken
          if(TKDClient[i])
          {
            TKDClient[i].stop();
          }

          // Checks If Clients Connected To The Server
          if(TKDClient[i] = TKDServer.available())
          {
            Serial.println("New Client: " + String(i));
          }

          // Continue Scanning
          continue;
        }
      }
      
      //no free/disconnected spot so reject
      WiFiClient TKDClient = TKDServer.available();
      TKDClient.stop();
    }
    else
    {
      // This LED Blinks If No Clients Where Available
      digitalWrite(D4, HIGH);
      delay(250);
      digitalWrite(D4, LOW);
      delay(250);
    }
  }

//=====Message From Connected Clients=====

  void AvailableMessage()
  {
    //check clients for data
    for(uint8_t i = 0; i < MAXSC; i++)
    {
      if (TKDClient[i] && TKDClient[i].connected() && TKDClient[i].available())
      {
          while(TKDClient[i].available())
          {
            Message = TKDClient[i].readStringUntil('\n');
            TKDClient[i].flush();
            Serial.println("NodeMCU_Client_" + String(i) + " : " + Message);
          }
      }
    }
  }

//======Command Reconstruction=====

 void CommandRecieved()
 {
    //The first 4 bits decide position of the Motor_1
    //The second 4 bits decide position of the Motor_2
    //The third 4 bits decide position of the Motor_3
    //The fourth 4 bits decide position of the Motor_4

}

//==================End==================
