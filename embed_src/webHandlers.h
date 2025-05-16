#pragma once

#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <ledBuiltin.h>
#include <templates_html.h>

extern String backend_base_url;
extern WebServer server;

// Handle root URL
bool handleRoot() {
  blink();
  server.send(200, "text/html", index_html);
  blink();
  return true;
}

class GateRequestHandler : public RequestHandler {
public:
  bool canHandle(WebServer &server, HTTPMethod method, const String &uri) override { 
      // Example: /gate/e808f789b390456dbf60c0516ac00da5/open 
      Serial.println(uri.startsWith("/gate"));
      return uri.startsWith("/gate");
    }

  bool handle(WebServer &server, HTTPMethod requestMethod, const String &requestUri) override {
    blink();
    // Split path into components
    String path = server.uri(); 
    Serial.println(requestUri);

    /*
    requestUri.remove(0, 6); // Remove "/gate/" prefix
    int slashIndex = path.indexOf('/');
    if (slashIndex == -1) {
      server.send(400, "text/plain", "Missing token or action");
      return true;
    }
    String token = path.substring(0, slashIndex);
    String action = path.substring(slashIndex + 1);
    */
    server.send(200, "text/html", gate_html);
    blink();

    return true;
  }
};

int autorize_gate(const String &token, const String &action) {
    String url = backend_base_url + token + action;
    Serial.println(url);

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        server.send(500, "application/json", "{\"error\": \"Invalid JSON from backend\"}");
      } else {
        String status = doc["status"] | "unknown";
        String message = doc["message"] | "No message";

        StaticJsonDocument<200> response;
        response["status"] = status;
        response["message"] = message;

        String responseBody;
        serializeJson(response, responseBody);
        server.send(httpCode, "application/json", responseBody);
      }
    } else {
      String error = "{\"error\": \"Failed to connect to backend\"}";
      server.send(500, "application/json", error);
    }
    http.end();
    blink();
    return true;
}


