#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Ovdje su definirane dvije web stranice koje se koriste za kontrolu vrata

String homepage =
  "<!DOCTYPE html> <html>"
  "<head>"
  "<style>"
  "body{font-family:Arial;"
  "background-color: #a0c2f2;"
  "}"
  ".spacing{"
  "display: flex;"
  "font-size: 20px;"
  "text-align: center;"
  "gap: 5em;"
  "color: #0b0163;"
  "flex-direction: column;"
  "}"
  ".flex-items{"

  "justify-content: space-around;"
  " }"

  "h1{text-align: center; color:#0b0163; font-size:100px;}"
  "a{"
  "background-color: #0b0163; "
  "color:bisque;"
  "border-radius:10px; "
  "border: none;"
  "padding:3em;"
  "width:50%;"
  "text-align: center;"
  "text-decoration: none;"
  "display: inline-block;"
  "font-size: 30px;"
  " margin: auto;"
  " }"
  "</style>"
  "</head>"
  "<body>"
  "<h1>Body scanner</h1>"
  "<div class=\"spacing\">"
  "<p class=\"flex-items\">note: using this controller will block your access to internet.</p>"
  "<p class=\"flex-items\">In case you keep loosing connection to the server, "
  "try disconnecting your mobile data, since smartphones recognise "
  "that this router is not connected to the internet.</p>"

  "<p class=\"flex-items\">click continue to start using the controller!</p>"
  "<a class=\"flex-items\" href= \"/control\">Continue</a>"
  "</div>"

  "</body>"
  "</html>";

String controller = "<!DOCTYPE html> <html>\n"
                    "<!DOCTYPE html> <html>"
                    "<head>"
                    "<style>"
                    "body{"
                    "font-family: Arial, Helvetica, sans-serif;"
                    "background-color: #a0c2f2;"
                    "}"
                    ".flex{"
                    "display: flex;"
                    "font-size: 20px;"
                    "text-align: centr;"
                    "gap: 5em;"
                    "color: #000000;"
                    "flex-direction: column;"
                    "width:50%;"
                    "margin:auto;"
                    "}"
                    "a{padding: 220px; background-color: yellow; grey: white; text-decoration: none; border-radius:10px; color:#000000;}"
                    "#opener{background-color: green}"
                    "#closer{background-color: red}"
                    "</style>"
                    "</head>"
                    "<body>"
                    "<h1></h1>"
                    "<div class=\"flex\">"
                    "<a id = \"opener\" href=\"/opening\">Open</a>"

                    "<a id =\"closer\" href=\"/closing\">Close</a>"

                    "<a href=\"/control\">Stop</a>"
                    "</div>"
                    "</body>"
                    "</html>";
/////////////////////////////////////////////////////////////////////////////

const char* ssid = "Body Scanner";
const char* pass = "Armarion";

ESP8266WebServer server(80);

int opening = D2;
int closing = D1;

bool opening_status = false;
bool closing_status = false;

int open_endstop = D4;
int closed_endstop = D3;

void setup() {

  Serial.begin(115200);
  Serial.println();


  pinMode(opening, OUTPUT);
  pinMode(closing, OUTPUT);
  pinMode(open_endstop, INPUT);
  pinMode(closed_endstop, INPUT);

  WiFi.softAP(ssid, pass);

  server.on("/", handleOnConnect);
  server.on("/control", handleControl);
  server.on("/opening", handleOpening);
  server.on("/closing", handleClosing);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("server zapocet");
}

void loop() {

  server.handleClient();



  // otvaranje vrata
  if (opening_status == true)
  {
      digitalWrite(opening, HIGH);
      digitalWrite(closing, LOW);

  }
  else
  {
    digitalWrite(opening, LOW);
    digitalWrite(closing, LOW);
  }




  // zatvaranje vrata
  if (closing_status == true)
  {

    digitalWrite(opening, LOW);
    digitalWrite(closing, HIGH);
  }
  else
  {
    digitalWrite(opening, LOW);
    digitalWrite(closing, LOW);
  }
}

void handleOnConnect()
{

  server.send(200, "text/html", homepage);
}
void handleControl()
{
  server.send(200, "text/html", controller);
  opening_status = false;
  closing_status = false;
}
void handleOpening()
{
  Serial.println("otvaranje vrata");
  opening_status = true;
  closing_status = false;
  server.send(200, "text/html", controller);
}

void handleClosing()
{
  Serial.println("Zatvaranje vrata");
  opening_status = false;
  closing_status = true;
  server.send(200, "text/html", controller);
}

void handleNotFound()
{
  server.send(404, "text/plain", "Not found");
}
