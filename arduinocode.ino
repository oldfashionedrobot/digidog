#include <WiFi.h>
#include <ESP32Servo.h>

int windOutputPin = 32;
int olfactoryServoPin = 14;

const char *ssid = "The Internet";
const char *password = "donttalkaboutfightclub";

WiFiServer server(80);
Servo myservo;

void setup()
{
  // setup IO pins
  pinMode(windOutputPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //attach servo to pin
  myservo.attach(olfactoryServoPin);
  myservo.write(0);

  // setup WiFi/server stuff
  Serial.begin(9600);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop()
{
  // listen for incoming clients
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");

    // turn on builtin led if there is a client
    digitalWrite(LED_BUILTIN, HIGH);

    // make a String to hold incoming data from the client
    String currentLine = "";

    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: application/json; charset=UTF-8");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("{\"wind\": {\"on\": \"windon\",\"off\": \"windoff\"},\"smell\": {\"on\": \"smellon\",\"off\": \"smelloff\"}}");

            // The HTTP response ends with another blank line:
            client.println();
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        // This seems to be a janky way to detect what path is being requested,
        // but it works for a few simple endpoints
        if (currentLine.endsWith("GET /windon"))
        {
          digitalWrite(windOutputPin, HIGH);
        }
        else if (currentLine.endsWith("GET /windoff"))
        {
          digitalWrite(windOutputPin, LOW);
        }

        if (currentLine.endsWith("GET /smellon"))
        {
          myservo.write(180);
        }
        else if (currentLine.endsWith("GET /smelloff"))
        {
          myservo.write(0);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

  // turn builtin led off if no client
  digitalWrite(LED_BUILTIN, LOW);
}