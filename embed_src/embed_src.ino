#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
// #include <esp_task_wdt.h>
// #include <LittleFS.h>

#include <web_handlers.h>
#include <led_builtin.h>

#define WATCHDOG_TIMEOUT 10

// DNS settings
const byte DNS_PORT = 53;
DNSServer dnsServer;

// WebServer on port 80
WebServer server(80);

// Configuration variables
String ap_ssid = "Chalé ao Pé da Pedra";
String sta_ssid = "Familia Vido";
String sta_password = "26221912";
// String html_template;
// String backend_base_url = "http://gatebnb.lvido.tech/";
String backend_base_url = "http://192.168.1.24:8000";

/*
// Function to load config.txt
void loadConfig() {
  File configFile = LittleFS.open("/config.txt", "r");
  if (!configFile) {
    Serial.println("Failed to open config.txt");
    return;
  }

  while (configFile.available()) {
    String line = configFile.readStringUntil('\n');
    line.trim();
    if (line.startsWith("STA_SSID=")) sta_ssid = line.substring(9);
    else if (line.startsWith("STA_PASSWORD=")) sta_password = line.substring(13);
    else if (line.startsWith("BACKEND_URL=")) backend_base_url = line.substring(12);
  }
  configFile.close();
}
*/

/*
// Load HTML template
void loadHtmlTemplate() {
  File htmlFile = LittleFS.open("/index.html", "r");
  if (htmlFile) {
    html_template = htmlFile.readString();
    htmlFile.close();
  } else {
    html_template = "<h1>Error loading HTML template</h1>";
  }
}
*/

void setupAP() {
  WiFi.softAP(ap_ssid.c_str());
  IPAddress apIP(192, 168, 0, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(DNS_PORT, "pedepedra.com.br", apIP);
  dnsServer.start(DNS_PORT, "pedepedra.com.br", apIP);
}

void connectToSTA() {
  WiFi.begin(sta_ssid.c_str(), sta_password.c_str());
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    blink();
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Internet via STA");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.println("Failed to connect to STA");
    deathblink();
  }
}

GateRequestHandler *myHandler;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  blink();
  Serial.begin(115200);
  myHandler = new GateRequestHandler();

/*
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  loadConfig();
  loadHtmlTemplate();
*/

  setupAP();
  connectToSTA();

  //server.addHandler(new GateHandler());
  server.addHandler(myHandler);
  server.on("/", handleRoot);

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  server.begin();

/*
  // Watchdog
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = WATCHDOG_TIMEOUT * 1000,
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
    .trigger_panic = true
  };
  // esp_task_wdt_reconfigure(&wdt_config);
  // esp_task_wdt_add(NULL);
*/
  set_led(LOW);
}

void loop() {
  // dnsServer.processNextRequest();
  server.handleClient();
  // esp_task_wdt_reset();
}
