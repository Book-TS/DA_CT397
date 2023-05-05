// #include <Arduino.h>
// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>
// #include <ArduinoJSon.h>
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <HX711.h>

// // WebSocket
// const char *ssid = "BookTS";
// const char *password = "12345678";

// // const char *ssid = "Yescoffee&tea2G";
// // const char *password = "66668888";

// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");

// struct dataJSON
// {
//     int type;
//     int mangoCost;
//     int longanCost;
//     int durianCost;
//     float weight;
// };

// dataJSON dataESP32;
// int status = 0;

// // LCD
// LiquidCrystal_I2C myLCD(0x27, 16, 2);

// // Loadcell
// #define LOADCELL_DOUT_PIN 16
// #define LOADCELL_SCK_PIN 4

// #define calibValue 103918

// HX711 myLoadCell;

// // Time
// unsigned long timeOld = 0;

// // function
// void Json_to_Value(String s);
// void Value_to_JSon();
// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
// void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
//              AwsEventType type, void *arg, uint8_t *data, size_t len);
// void initWebSocket();

// void initLoadCell();

// void setup()
// {
//     Serial.begin(115200);
//     Wire.begin();
//     myLCD.init();
//     myLCD.backlight();
//     myLCD.setCursor(0, 0);
//     myLCD.print("Cau hinh ...");

//     initLoadCell();

//     WiFi.begin(ssid, password);

//     for (size_t i = 0; i < 10; i++)
//     {
//         if (WiFi.status() != WL_CONNECTED)
//         {
//             myLCD.setCursor(9 + i % 3, 0);
//             myLCD.print(".  ");
//             if (i == 9)
//             {
//                 myLCD.clear();
//                 myLCD.setCursor(2, 0);
//                 myLCD.print("That bai");
//             }
//             delay(1000);
//         }
//         else
//         {
//             Serial.println(WiFi.localIP());
//             myLCD.clear();
//             myLCD.setCursor(3, 0);
//             myLCD.print("Thanh cong");
//             myLCD.setCursor(1, 1);
//             myLCD.print(WiFi.localIP());

//             initWebSocket();
//             server.begin();
//             return;
//         }
//     }
// }

// void loop()
// {
//     if ((unsigned long)(millis() - timeOld) > 500)
//     {
//         char val[80];
//         dataESP32.weight = abs(myLoadCell.get_units(10));
//         sprintf(val, "%.2f", dataESP32.weight);

//         myLoadCell.power_down();
        
//         if (dataESP32.weight > 0.05)
//         {
//             Serial.println(val);
//             ws.cleanupClients();
//             ws.textAll(val);

//             myLCD.setCursor(0, 1);
//             myLCD.print(String(val) + " kg");
//         }
//         else {
//             myLCD.setCursor(0, 1);
//             myLCD.print("                ");
//         }
//         timeOld = millis();
//         myLoadCell.power_up();
//     }
// }

// void initWebSocket()
// {
//     ws.onEvent(onEvent);
//     server.addHandler(&ws);
//     ;
// }

// void Json_to_Value(String s)
// {
//     StaticJsonDocument<200> doc;
//     DeserializationError error = deserializeJson(doc, s);

//     if (error)
//     {
//         Serial.println("Failed to parse JSON string.");
//     }
//     else
//     {
//         dataESP32.type = doc["type"];
//         dataESP32.mangoCost = doc["mangoCost"];
//         dataESP32.longanCost = doc["longanCost"];
//         dataESP32.durianCost = doc["durianCost"];

//         switch (dataESP32.type)
//         {
//         case 1:
//             myLCD.clear();
//             myLCD.setCursor(4, 0);
//             myLCD.print("NS: Xoai");
//             break;
//         case 2:
//             myLCD.clear();
//             myLCD.setCursor(4, 0);
//             myLCD.print("NS: Nhan");
//             break;
//         case 3:
//             myLCD.clear();
//             myLCD.setCursor(1, 0);
//             myLCD.print("NS: Sau Rieng");
//             break;
//         default:
//             break;
//         }

//         Serial.println(String(dataESP32.type) + " | " + String(dataESP32.mangoCost) + " | " + String(dataESP32.longanCost) + " | " + String(dataESP32.durianCost));
//     }
// }

// void Value_to_JSon()
// {
//     StaticJsonDocument<200> jsonVal;

//     jsonVal["type"] = dataESP32.type;
//     jsonVal["weight"] = dataESP32.weight;

//     String jsonString;
//     serializeJson(jsonVal, jsonString);

//     ws.textAll(jsonString);
// }

// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
// {
//     AwsFrameInfo *info = (AwsFrameInfo *)arg;
//     if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
//     {
//         data[len] = 0;
//         Json_to_Value((char *)data);
//     }
// }

// void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
//              AwsEventType type, void *arg, uint8_t *data, size_t len)
// {
//     switch (type)
//     {
//     case WS_EVT_CONNECT:
//         Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
//         myLCD.clear();
//         myLCD.setCursor(0, 0);
//         myLCD.print("Client Connected");
//         break;
//     case WS_EVT_DISCONNECT:
//         Serial.printf("WebSocket client #%u disconnected\n", client->id());
//         break;
//     case WS_EVT_DATA:
//         handleWebSocketMessage(arg, data, len);
//         break;
//     case WS_EVT_PONG:
//     case WS_EVT_ERROR:
//         break;
//     }
// }

// void initLoadCell()
// {
//     myLoadCell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

//     myLoadCell.set_scale(calibValue);
//     myLoadCell.tare();
// }