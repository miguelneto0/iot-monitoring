#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h> // for telegram bot
#include <HTTPClient.h>


// ===========================
// Select camera model in board_config.h
// ===========================
#include "board_config.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "Tamires_2G";
const char *password = "Tamires#32";
String botToken = "8392803102:AAEXOniY44AhlSZ0k4gupEV0ESxqRX1872c"; // telegram token
String chatId   = "691297288"; // telegram chat

void startCameraServer();
void setupLedFlash();
void sendPhotoTelegram();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  // config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size   = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count     = 1;
  config.grab_mode    = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location  = CAMERA_FB_IN_PSRAM;


  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  Serial.print("pixel_format = ");
  Serial.println(config.pixel_format);
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  Serial.print("pid = ");
  Serial.println(s->id.PID);
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
  
  delay(20000); // espera 20 segundos

  for (int i = 0; i < 3; i++) {
    sendPhotoTelegram();
    delay(2000); // 2s entre fotos
  }

}

void loop() {
  // Do nothing. Everything is done in another task by the web server
  delay(10000);
}

void sendPhotoTelegram() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Falha ao capturar imagem");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure(); // ignora SSL (necessário no ESP32)

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Falha ao conectar no Telegram");
    esp_camera_fb_return(fb);
    return;
  }

  String boundary = "ESP32CAM";
  String head =
    "--" + boundary + "\r\n"
    "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n" +
    chatId + "\r\n"
    "--" + boundary + "\r\n"
    "Content-Disposition: form-data; name=\"photo\"; filename=\"image.jpg\"\r\n"
    "Content-Type: image/jpeg\r\n\r\n";

  String tail = "\r\n--" + boundary + "--\r\n";

  uint32_t contentLength = head.length() + fb->len + tail.length();

  client.println("POST /bot" + botToken + "/sendPhoto HTTP/1.1");
  client.println("Host: api.telegram.org");
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println("Content-Length: " + String(contentLength));
  client.println();

  client.print(head);
  client.write(fb->buf, fb->len);
  client.print(tail);

  esp_camera_fb_return(fb);

  // opcional: ler resposta
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }

  Serial.println("Foto enviada para o Telegram");
  
  while (client.available()) {
    Serial.write(client.read());
  }

}


