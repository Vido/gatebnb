#include <WiFi.h>
#include <WebServer.h>
// #include <esp32_https_server.h>
#include <DNSServer.h>
// #include <esp_task_wdt.h>

#include <webHandlers.h>
#include <ledBuiltin.h>
#include <sslCert.h>

#define DNS_PORT 53
#define HTTP_PORT 80
#define HTTPS_PORT 443
#define WATCHDOG_TIMEOUT 10

DNSServer dnsServer;
IPAddress apIP(192, 168, 0, 1);
WebServer server(HTTP_PORT);
// HTTPSServer secureServer(HTTPS_PORT);
GateRequestHandler *myHandler;

// Configuration variables
String ap_ssid = "Chalé ao Pé da Pedra";
String sta_ssid = "Familia Vido";
String sta_password = "26221912";
// String html_template;
// String backend_base_url = "http://gatebnb.lvido.tech/";
String backend_base_url = "http://192.168.1.24:8000";

IPAddress connectToSTA() {
  WiFi.begin(sta_ssid.c_str(), sta_password.c_str());
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    blink();
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to STA");
    deathblink();

  }
  Serial.println("Connected to Internet via STA");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  return WiFi.localIP();
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  blink();
  Serial.begin(115200);
  
  WiFi.setHostname("pedapedra.local");
  WiFi.softAP(ap_ssid.c_str(), NULL, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  connectToSTA();

  dnsServer.start(DNS_PORT, "*", apIP);

  myHandler = new GateRequestHandler();
  server.addHandler(myHandler);
  server.on("/", handleRoot);
  server.onNotFound([]() {
    server.sendHeader("Location", String("http://") + apIP.toString(), true);
    server.send(302, "text/plain", "");
  });

  // secureServer.setServerKeyAndCert(private_key, certificate);
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
  dnsServer.processNextRequest();
  server.handleClient();
  // esp_task_wdt_reset();
}
