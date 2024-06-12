#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "Toan Anh";      
const char* password = "65701526"; 

const char* host = "192.168.1.8";  
const uint16_t port = 1414;

WebSocketsClient webSocket;

void handleWebSocketMessage(WStype_t type, uint8_t * payload, size_t length) {
    String message = String((char*)payload);

    if (message == "UP") {
        Serial.println("U");
    } 
    else if (message == "DOWN") {
        Serial.println("D");
    } 
    else if (message == "LEFT") {
        Serial.println("L");
    } 
    else if (message == "RIGHT") {
        Serial.println("R");
    } 
    else if (message == "PAUSE") {
        Serial.println("P");
    }
}

void setup() {
    Serial.begin(9600);  // để debug
     // Giao tiếp UART với Arduino

    delay(10);

    // Kết nối WiFi
    Serial.println();
    Serial.println();
    Serial.print("Đang kết nối đến ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi đã kết nối");
    Serial.println("Địa chỉ IP: ");
    Serial.println(WiFi.localIP());

    // Kết nối WebSocket
    webSocket.begin(host, port, "/");

    webSocket.onEvent(handleWebSocketMessage);

    webSocket.setReconnectInterval(5000); // Thử lại kết nối mỗi 5 giây nếu bị mất kết nối
}

void loop() {
    webSocket.loop();
}