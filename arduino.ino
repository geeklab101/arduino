#include <Wire.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>]
#include <ArduinoJson.h>
//#include <RFID.h>


// Define RFID pins
#define SDA_DIO 9
#define RESET_DIO 8
int tools[4] = {227, 51, 35, 179};
int product[4] = {1, 1, 1, 1};
int toolstotal[4] = {0, 0, 0, 0};
int masterCopy[4] = {0, 0, 0, 0};
int Total;

int trolly = 0;
int scanning = 0;
int station = 0;
int moving = 0;
int obstacle = 0;
/* Map tool IDs to corresponding tool names */
String toolNames[4] = {"Hammer", "Screwdriver", "Wrench", "Ratchet"};

/* Define color sensor pins */
#define S0 4
#define S1 5
#define S2 6
#define S3 7 
#define sensorOut 10

/* Define motor control pins */
#define MOTOR_PIN_1 11
#define MOTOR_PIN_2 12
#define BUTTON_STATION_SELECT1 14
#define BUTTON_STATION_SELECT2 16
#define BUTTON_STATION_SELECT3 17
#define Add_BUTTON_PIN 15

/* Define  Proximity Sensor pins */
#define SENSOR_PIN_1 A0  // sensor 1
#define SENSOR_PIN_2 A1  // sensor 2
#define BUZZER_PIN 13     // buzzer pin
#define THRESHOLD 600
SoftwareSerial esp8266(2, 3); //rx, tx
const int capacity = JSON_OBJECT_SIZE(3) + 30;
DynamicJsonDocument doc(capacity);



MFRC522 RC522(SDA_DIO, RESET_DIO);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    RC522.PCD_Init();

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
    
    pinMode(MOTOR_PIN_1, OUTPUT);
    pinMode(MOTOR_PIN_2, OUTPUT);


    pinMode(SENSOR_PIN_1, INPUT);
    pinMode(SENSOR_PIN_2, INPUT);

    pinMode(BUTTON_STATION_SELECT1, INPUT);
    pinMode(BUTTON_STATION_SELECT2, INPUT);
    pinMode(BUTTON_STATION_SELECT3, INPUT);
    pinMode(Add_BUTTON_PIN, INPUT);


    Serial.println("System initialized Now ScaN tools Intially");
    scanTools();
    createMasterCopy();
    esp8266.begin(9600);
    // Connect to Wi-Fi
    connectToWiFi();
}
int colorFrequencies[3];
void loop() {
    
    detectColor(colorFrequencies);
    moveTrolley(colorFrequencies[0], colorFrequencies[1], colorFrequencies[2]);
    alertObstacle();
    useToolsAtStation();
    loadToolsBack();
    compareWithMasterCopy();
    returnTools();
    addNewTools();
    selectStation();
    sendJSON();

}

void scanTools() {
  if (RC522.PICC_IsNewCardPresent() && RC522.PICC_ReadCardSerial()) {
    byte i = 0;
    byte j = 0; 
    byte k = 0;
    int toolID = RC522.uid.uidByte[0];
    registerTool(toolID);
    for (i = 0; i < 4; i++){
      scanning = 1; 
        if (tools[i] == toolID){
            Serial.print("Tool Name: ");
            Serial.println(toolNames[i]);
            if (toolstotal[i] == 0){
                Total = Total + product[i];
                toolstotal[i] = 1;
            }
            else{
                Total = Total - product[i];
                toolstotal[i] = 0;
            }
            Serial.print("Total Tools: ");
            Serial.println(Total);
            
            break;
        }
    }scanning = 0;
    delay(1000);  // Adjust delay based on your system requirements
  }
}

void registerTool(int toolID) {
  Serial.print("Tool ID: ");
  Serial.println(toolID);
}

void detectColor(int colorFrequencies[3]) {
    // Setting red filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);

    // Reading the output frequency
    colorFrequencies[0] = pulseIn(sensorOut, LOW);
    Serial.print("R= ");
    Serial.print(colorFrequencies[0]);
    Serial.print("  ");
    delay(500);

    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);

    // Reading the output frequency
    colorFrequencies[1] = pulseIn(sensorOut, LOW);
    Serial.print("G= ");
    Serial.print(colorFrequencies[1]);
    Serial.print("  ");
    delay(500);

    // Setting Blue filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);

    // Reading the output frequency 
    colorFrequencies[2] = pulseIn(sensorOut, LOW);
    Serial.print("B= ");
    Serial.print(colorFrequencies[2]);
    Serial.println("  ");
    delay(500);
}



void moveTrolley(int redFrequency, int greenFrequency, int blueFrequency) {
  
  int redThreshold = 500;  // Adjust this threshold based on your color sensor readings

  if (redFrequency > redThreshold) {
    digitalWrite(MOTOR_PIN_1, HIGH);
    digitalWrite(MOTOR_PIN_2, HIGH);
    moving = 1;
  } else {
    digitalWrite(MOTOR_PIN_1, LOW);
    digitalWrite(MOTOR_PIN_2, LOW);
    moving = 0;
  }
}

void alertObstacle() {
  // Read sensor values
  int sensorValue1 = analogRead(SENSOR_PIN_1);
  int sensorValue2 = analogRead(SENSOR_PIN_2);

  // Check if obstacle is detected by either sensor
  if (sensorValue1 > THRESHOLD || sensorValue2 > THRESHOLD) {
    digitalWrite(MOTOR_PIN_1, LOW);
    digitalWrite(MOTOR_PIN_2, LOW);
    Serial.println("Obstacle detected. Trolley stopped.");
    beepAlert();
    obstacle = 1;
  }else {
    obstacle = 0;
  }
  
}

void beepAlert() {
  tone(BUZZER_PIN, 1000);
  delay(500);             // Adjust the duration of the beep
  noTone(BUZZER_PIN);
}

void useToolsAtStation() {
  Serial.println("User is using tools at the station.");
  int toolID;
  while (RC522.PICC_IsNewCardPresent() && RC522.PICC_ReadCardSerial()) {
    toolID = RC522.uid.uidByte[0];
    Serial.print("Tool unloaded: ");
    Serial.println(toolID);
    scanning = 1;
    delay(1000); // Adjust delay as needed
  }
  scanning = 0;
}

void loadToolsBack() {
  Serial.println("User is loading tools back into the trolley.");
  int toolID;
  while (RC522.PICC_IsNewCardPresent() && RC522.PICC_ReadCardSerial()) {
    toolID = RC522.uid.uidByte[0];
    Serial.print("Tool loaded back: ");
    Serial.println(toolID);
    scanning = 1;
    trolly = 1;
    delay(1000); // Adjust delay as needed
  }
  scanning = 0;
  
}

void returnTools() {
  Serial.println("User is returning tools to the tools store.");
  int toolID;
  while (RC522.PICC_IsNewCardPresent() && RC522.PICC_ReadCardSerial()) {
    toolID = RC522.uid.uidByte[0];
    Serial.print("Tool returned: ");
    Serial.println(toolID);
    scanning = 1;
    // Update master copy (1: loaded, 0: not loaded)
    int toolIndex = findToolIndex(toolID);
    if (toolIndex != -1) {
      masterCopy[toolIndex] = 1;  // Assuming tools are returned to the store
    }

    delay(1000); // Adjust delay as needed
  }
  scanning = 0;
  trolly = 0;
}


void addNewTools() {
  Serial.println("User wants to add new tools.");
  if (digitalRead(Add_BUTTON_PIN) == HIGH) {
    Serial.println("Scanning new tools. Please place each tool on the RFID reader.");

    for (int i = 0; i < 4; i++) {
      // Scan each tool one by one until done
      while (RC522.PICC_IsNewCardPresent() && RC522.PICC_ReadCardSerial()) {
        int toolID = RC522.uid.uidByte[0];
        Serial.print("New tool added: ");
        Serial.println(toolID);
        scanning = 1;
        // Updating master copy (
        int toolIndex = findToolIndex(toolID);
        if (toolIndex != -1) {
          masterCopy[toolIndex] = 1;
        }

        delay(1000); 
      }
    }
    scanning = 0;
    Serial.println("New tools added successfully. Updated master copy.");
  }
}

void createMasterCopy() {
  Serial.println("Creating master copy after initial scanning.");
  for (int i = 0; i < 4; i++) {
    if (toolstotal[i] > 0) {
      masterCopy[i] = 1;
    }
  }
}

void compareWithMasterCopy() {
  Serial.println("Comparing loaded tools with the master copy.");
  for (int i = 0; i < 4; i++) {
    if (toolstotal[i] > 0 && masterCopy[i] == 0) {
      Serial.print("Missing tool: ");
      Serial.println(tools[i]);
    }
  }
}

int findToolIndex(int toolID) {
  // Find the index of the tool in the tools array
  for (int i = 0; i < 4; i++) {
    if (tools[i] == toolID) {
      return i;
    }
  }
  // Return -1 if tool not found
  return -1;
}

void selectStation() {
  Serial.println("User is selecting a station.");

  int selectedStation = 0; // Initialize with the first station
  int maxStations = 3;

  while (true) {
    if (digitalRead(BUTTON_STATION_SELECT1) == HIGH) {
       selectedStation = 0;
      Serial.print("Selected Station: ");
      Serial.println(selectedStation + 1); 
      station = 1;
      moveTrolleyToStation(selectedStation);

      break;
    }else if (digitalRead(BUTTON_STATION_SELECT2) == HIGH){
       selectedStation = 1;
      Serial.print("Selected Station: ");
      Serial.println(selectedStation + 1); 
      station = 2;
      moveTrolleyToStation(selectedStation);

      break;
    }else if (digitalRead(BUTTON_STATION_SELECT3) == HIGH){
       selectedStation = 2;
      Serial.print("Selected Station: ");
      Serial.println(selectedStation + 1); 
      station = 3;
      moveTrolleyToStation(selectedStation);

      break;
    }else {
      station = 0;
    }

    delay(1000);

    selectedStation = (selectedStation + 1) % maxStations;
  }
}

void moveTrolleyToStation(int station) {
    Serial.println("Moving trolley to the selected station...");
    moveTrolley(colorFrequencies[0], colorFrequencies[1], colorFrequencies[2]);
}



void connectToWiFi() {
  Serial.println("Connecting to WiFi");

  sendCommand("AT", 1000);  // Check if ESP8266 is responding

  sendCommand("AT+CWMODE=1", 1000);  // Set ESP8266 to Station mode

  sendCommand("AT+CWJAP=\"10R\",\"234567888\"", 5000);  // Connect to WiFi as my is 10R and Pass is 234567888

  sendCommand("AT+CIFSR", 1000);  // Get IP address

  Serial.println("Connected to WiFi");
}

void sendCommand(String command, int timeout) {
  esp8266.println(command);
  delay(timeout);
  while (esp8266.available()) {
    Serial.write(esp8266.read());
  }
}

void sendJSON() {
    Serial.println("Sending JSON data");

    // Data to be sent in JSON format
    JsonObject data_to_send = doc.to<JsonObject>();
    data_to_send["trolly"] = trolly;
    data_to_send["scanning"] = scanning;
    data_to_send["station"] = station;
    data_to_send["moving"] = moving;
    data_to_send["obstacle"] = obstacle;

    // Convert JSON data to a String
    String json_data;
    serializeJson(data_to_send, json_data);

    // Specify the headers (content type as JSON)
    String headers = "POST /trolly/api/jsonapi.php HTTP/1.1\r\n"
                    "Host: 192.168.1.1\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: " + String(json_data.length()) + "\r\n\r\n";

    // Make the POST request to the server
    sendCommand("AT+CIPSTART=\"TCP\",\"192.168.1.1\",80", 5000);  // Establish TCP connection
    sendCommand("AT+CIPSEND=" + String(headers.length() + json_data.length()), 1000);  // Send headers length
    sendCommand(headers, 1000);  // Send headers
    sendCommand(json_data, 1000);  // Send JSON data

    // Close the TCP connection
    sendCommand("AT+CIPCLOSE", 1000);

    Serial.println("JSON data sent");
}
