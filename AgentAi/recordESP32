#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <SD.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Pins
const int SD_CS = 5;
const int AUDIO_PIN = 34;
const int BUTTON_PIN = 32;
const int LED_PIN = 33;

// Configuration for audio recording
const int SAMPLE_RATE = 8000;
const int BIT_DEPTH = 16;
const int RECORD_DURATION = 2;

// WIFI connection
String SSID = "";
String PASSWORD = "";

// Gemini API key
String API_KEY = "";

void setupWifi() {
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status()!= WL_CONNECTED) {
    delay(1000);
    Serial.print("...");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void recordAudio() {
  if (!SD.begin(SD_CS, SPI, 1000000)) {
    Serial.println("SD card initialization failed!");
    while (1);
  } else {
    Serial.println("SD card initialized!");
  }

  if (SD.exists("/tmp.wav")) {
    if (SD.remove("/tmp.wav")) {
      Serial.println("Previous audio file deleted.");
    } else {
      Serial.println("Failed to delete previous audio file.");
      return;
    }
  } else {
    Serial.println("No previous audio file detected, starting new");
  }

  File audioFile = SD.open("/tmp.wav", FILE_WRITE);
  if (!audioFile) {
    Serial.println("Failed to create audio file.");
    return;
  }

  Serial.println("Start recording");
  writeWavHeader(audioFile, SAMPLE_RATE, BIT_DEPTH, 1);

  int numSamples = SAMPLE_RATE * RECORD_DURATION;
  for (int i = 0; i < numSamples; i++) {
    int rawValue = analogRead(AUDIO_PIN);
    int16_t sample = map(rawValue, 0, 4095, -32768, 32767);
    audioFile.write((uint8_t*)&sample, 2);
    delayMicroseconds(1000000 / SAMPLE_RATE);
  }

  audioFile.close();
  Serial.println("Audio recorded to /tmp.wav");
}

void writeWavHeader(File& file, int sampleRate, int bitDepth, int channels) {
  uint32_t byteRate = sampleRate * channels * bitDepth / 8;
  uint16_t blockAlign = channels * bitDepth / 8;

  file.write((const uint8_t*)"RIFF", 4);
  uint32_t fileSize = 36 + RECORD_DURATION * byteRate;
  file.write((uint8_t*)&fileSize, 4); 
  file.write((const uint8_t*)"WAVE", 4);
  file.write((const uint8_t*)"fmt ", 4);
  uint32_t subchunk1Size = 16;
  file.write((uint8_t*)&subchunk1Size, 4);
  uint16_t audioFormat = 1;
  file.write((uint8_t*)&audioFormat, 2);
  file.write((uint8_t*)&channels, 2);
  file.write((uint8_t*)&sampleRate, 4);
  file.write((uint8_t*)&byteRate, 4);
  file.write((uint8_t*)&blockAlign, 2);
  file.write((uint8_t*)&bitDepth, 2);
  file.write((const uint8_t*)"data", 4);
  uint32_t subchunk2Size = RECORD_DURATION * byteRate;
  file.write((uint8_t*)&subchunk2Size, 4);
}

String base64Encode(const uint8_t* data, size_t length) {
  const char* b64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String encodedString = "";
  uint32_t i = 0;
  uint8_t b1, b2, b3;

  while (i < length) {
    b1 = data[i++];
    encodedString += b64_alphabet[b1 >> 2];
    if (i < length) {
      b2 = data[i++];
      encodedString += b64_alphabet[((b1 & 0x03) << 4) | (b2 >> 4)];
    } else {
      encodedString += b64_alphabet[(b1 & 0x03) << 4];
      encodedString += "==";
      break;
    }
    if (i < length) {
      b3 = data[i++];
      encodedString += b64_alphabet[((b2 & 0x0F) << 2) | (b3 >> 6)];
      encodedString += b64_alphabet[b3 & 0x3F];
    } else {
      encodedString += b64_alphabet[(b2 & 0x0F) << 2];
      encodedString += '=';
      break;
    }
  }
  return encodedString;
}

void createAudioJsonRequest() {
  if (SD.exists("/request-tmp.json")) {
    if (SD.remove("/request-tmp.json")) {
      Serial.println("Previous request file deleted.");
    } else {
      Serial.println("Failed to delete previous request file.");
      return;
    }
  } else {
    Serial.println("No previous request file detected, starting new");
  }

  File stringFile = SD.open("/audiostring.txt", FILE_READ);
  if (!stringFile) {
    Serial.println("Failed to open audiostring.txt for reading");
    return;
  }

  // Read the base64 encoded audio data from the file
  String base64EncodedData = stringFile.readString();
  stringFile.close();

  // Create the JSON document
  const size_t jsonBufferSize = 1024 * 48; // Adjust as needed
  DynamicJsonDocument doc(jsonBufferSize);
  JsonArray contents = doc.createNestedArray("contents");
  JsonObject content = contents.createNestedObject();
  JsonArray parts = content.createNestedArray("parts");

  JsonObject textPart = parts.createNestedObject();
  textPart["text"] = "Provide a transcript of this audio clip. Only include words said in the audio.";

  JsonObject audioPart = parts.createNestedObject();
  JsonObject inlineData = audioPart.createNestedObject("inline_data");
  inlineData["mime_type"] = "audio/x-wav";
  inlineData["data"] = base64EncodedData; // Use the data read from the file

  // Open a file on the SD card for writing the JSON request
  File jsonFile = SD.open("/request-tmp.json", FILE_WRITE);
  if (!jsonFile) {
    Serial.println("Failed to open JSON file for writing");
    return;
  }

  // Serialize the JSON document to the file
  serializeJson(doc, jsonFile);
  jsonFile.close();

  Serial.println("JSON request saved to /request-tmp.json");
}

void transcribeAudio() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (http.begin(client, "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + API_KEY)) {
    http.addHeader("Content-Type", "application/json");

    File file = SD.open("/request-tmp.json", FILE_READ);
    if (!file) {
      Serial.println("Failed to open file for reading from SD card");
      return;
    }

    const int BUFFER_SIZE = 64;
    uint8_t fileBuffer[BUFFER_SIZE];

    const int JSON_STRING_SIZE = 65536; // Allocate 64kb for the audio file request. Likely smaller.
    char *jsonString = (char *)malloc(JSON_STRING_SIZE); 
    if (jsonString == NULL) {
      Serial.println("Failed to allocate memory for JSON string");
      file.close();
      return;
    }
    int jsonStringIndex = 0;

    while (file.available()) {
      int bytesRead = file.read(fileBuffer, BUFFER_SIZE);
      for (int i = 0; i < bytesRead && jsonStringIndex < JSON_STRING_SIZE - 1; i++) {
        jsonString[jsonStringIndex++] = fileBuffer[i];
      }
    }
    jsonString[jsonStringIndex] = '\0';

    file.close();
    SD.end(); // Close the SD connection after reading the file
    
    // Serial.println(jsonString);
    int httpCode = http.POST(jsonString);
    free(jsonString);
    Serial.print(F("Http code: "));
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      String responseText = doc["candidates"][0]["content"]["parts"][0]["text"];

      Serial.print("Response: ");
      Serial.println(responseText);
    }
  }
}

void saveAudioString() {
  File audioFile = SD.open("/tmp.wav", FILE_READ);
  if (!audioFile) {
    Serial.println("Failed to open audio file for reading");
    return;
  }

  size_t fileSize = audioFile.size();
  uint8_t* audioData = (uint8_t*)malloc(fileSize);
  if (audioData == NULL) {
    Serial.println("Failed to allocate memory for audio data");
    audioFile.close();
    return;
  }
  audioFile.read(audioData, fileSize);
  audioFile.close();

  String base64AudioData = base64Encode(audioData, fileSize);
  free(audioData);

  File stringFile = SD.open("/audiostring.txt", FILE_WRITE);
  if (!stringFile) {
    Serial.println("Failed to open audiostring.txt for writing");
    return;
  }
  stringFile.print(base64AudioData);
  stringFile.close();

  Serial.println("Audio base64 string saved to /audiostring.txt");
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  WRITE_PERI_REG(RTC_CNTL_WDTCONFIG0_REG, 0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  while (!Serial);
  
  setupWifi();

  if (!SD.begin(SD_CS, SPI, 1000000)) {
    Serial.println("SD card initialization failed!");
    while (1);
  } else {
    Serial.println("SD card initialized!");
  }
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
    
    // This delay is to debounce the button and allow time to speak
    delay(500); 

    recordAudio();
    digitalWrite(LED_PIN, LOW);
    saveAudioString();
    createAudioJsonRequest();
    transcribeAudio();
  }
}
