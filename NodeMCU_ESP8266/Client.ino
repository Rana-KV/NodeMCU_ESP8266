//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
  #include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
  #include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
    #define D0 16   // On Board LED [ RED ]
    #define D1 5
    #define D2 4
    #define D3 0
    #define D4 2    // WIFI Module LED [ BLUE ]
    #define D5 14
    #define D6 12
    #define D7 13        
//------------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------------
  String Data;
  const String  Name = "NodeMCU_Client_1";
//------------------------------------------------------------------------------------
// WIFI Authentication Variables
//------------------------------------------------------------------------------------
/* This Client Is Going To Connect To A WIFI Server Access Point
 * So You Have To Specify Server WIFI SSID & Password In The Code Not Here
 * Please See The Function Below Name (WiFi.Begin)
 * If WIFI dont need Any password Then WiFi.begin("SSIDNAME")
 * If WIFI needs a Password Then WiFi.begin("SSIDNAME", "PASSWORD")
 */
  char*         TKDssid;            // Wifi Name
  char*         TKDpassword;        // Wifi Password
//------------------------------------------------------------------------------------
// WIFI Module Role & Port
//------------------------------------------------------------------------------------
/* This WIFI Module Code Works As A Client
 * That Will Connect To A Server WIFI Modul With (IP ADDress 192.168.4.1)
 */
  int             TKDServerPort  = 9001;
  IPAddress       TKDServer(192,168,4,1);
  WiFiClient      TKDClient;
//====================================================================================

  void setup() 
  {
    // Setting The Serial Port ----------------------------------------------
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins ---------------------------------------------
    pinMode(D4, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(D0, OUTPUT);          //On Board
    digitalWrite(D4, HIGH);       // Turn WiFi LED Off
    digitalWrite(D0, LOW);
    
    // Print Message Of I/O Setting Progress --------------------------------
    Serial.println("\nI/O Pins Modes Set .... Done");

    // Starting To Connect --------------------------------------------------
    if(WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      delay(50);
    }

    /* in this part it should load the ssid and password 
     * from eeprom they try to connect using them */
    
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.begin("NodeMCU_Server", "");      // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

    // WiFi Connectivity ----------------------------------------------------
    CheckWiFiConnectivity();        // Checking For Connection

    // Stop Blinking To Indicate Connected ----------------------------------
    digitalWrite(D4, !HIGH);
    Serial.println("!-- Client Device Connected --!");

    // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TKDServer);
    Serial.print  ("Server Port Num   : ");
    Serial.println(TKDServerPort);
    // Printing MAC Address
    Serial.print  ("Device MC Address : ");
    Serial.println(String(WiFi.macAddress()));
    // Printing IP Address
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
    
    // Connecting The Device As A Client -------------------------------------
    TKDRequest();
  }

//====================================================================================
  
  void loop()
  {

    Read_Data_And_Send();
    CheckWiFiConnectivity();
  
  }

//====================================================================================

  void Read_Data_And_Send()
  {
    if(Serial.available() > 0) // Don't read unless there is no data
    {
      // Reading The Data
      Data = Serial.readString();
      
      Serial.println( Name + ": " + Data ); // printing the data in the Client serial monitor 
      TKDClient.println( Data ); // printing the data in the Server serial monitor
      TKDClient.flush();
    }
  }

//====================================================================================

  void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(D4, !HIGH);
        delay(250);
        digitalWrite(D4, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
      digitalWrite(D0, HIGH);
    }
    digitalWrite(D0, LOW);
  }
        
//====================================================================================

  void TKDRequest()
  {
    // First Make Sure You Got Disconnected
    TKDClient.stop();

    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, TKDServerPort))
    {
      Serial.println    ("<" + Name + "-CONNECTED>");
      TKDClient.println ("<" + Name + "-CONNECTED>");
    }
  }

//====================================================================================
