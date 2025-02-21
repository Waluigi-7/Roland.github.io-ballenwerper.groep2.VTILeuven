#include <WiFiS3.h>

char ssid[] = "Het Zwart Schaap";  // Use correct Wi-Fi name
char pass[] = "Vijverlaan343010kessello";

WiFiServer server(80);

int led = 9;
String ledStatus = "OFF";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  pinMode(led, OUTPUT);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);

    if (request.indexOf("/H") != -1) {
      digitalWrite(led, HIGH);
      ledStatus = "ON";
    } else if (request.indexOf("/L") != -1) {
      digitalWrite(led, LOW);
      ledStatus = "OFF";
    }

    // Proper HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<html><body>");
    client.println("<h1>LED status is " + ledStatus + "</h1>");
    client.println("<a href=\"/H\">Turn ON</a><br>");
    client.println("<a href=\"/L\">Turn OFF</a>");
    client.println("</body></html>");
    client.println();

    client.stop();
    Serial.println("Client disconnected");
  }
  else Serial.println("waiting for client...");
}
