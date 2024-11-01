#include "esp_camera.h"
#include <Wifi.h>
#include <Arduino.h>
#include "soc/soc.h"
#include "camera_config.h"

const char ssid[] = "Filip wifi";
const char password[] = "tadeuszsznuk";
const int server_port = 1314;
int status = WL_IDLE_STATUS;
IPAddress server(20, 215, 192, 11);

WiFiClient cam_client;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Camera configuration
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
  config.xclk_freq_hz = 15000000;
  config.pixel_format = PIXFORMAT_JPEG; // For streaming JPEG

  // Frame size and quality
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // Higher resolution
    config.jpeg_quality = 10;           // Lower number means better quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA; // Lower resolution
    config.jpeg_quality = 10;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed: 0x%x\n", err);
    delay(1000);
    ESP.restart();
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  // Set resolution for lower quality (CIF)

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("...");
  }
  Serial.println("WiFi connected.");

  // Connect to TCP server
  while (!cam_client.connect(server, server_port)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nCamera connected to TCP server.");
}

void loop() {

  cam_client.print("cam_start");

  // Capture an image
  camera_fb_t *fb = esp_camera_fb_get(); // Capture the frame
  if (!fb) {
    Serial.println("Camera capture failed.");
    delay(1000);
    return;
  }

  uint8_t *fb_buf = fb->buf;  // Pointer to image buffer
  size_t fb_len = fb->len;    // Length of the image

  Serial.printf("Image size: %zu bytes\n", fb_len);

  // Send the image in chunks of 1024 bytes
  for (size_t n = 0; n < fb_len; n += 1024) {
    size_t chunk_size = (n + 1024 < fb_len) ? 1024 : fb_len - n;  // Adjust last chunk size
    cam_client.write(fb_buf + n, chunk_size);  // Send each chunk
  }
  esp_camera_fb_return(fb);  // Return the frame buffer after sending
  delay(30);
  cam_client.print("cam_end");
  delay(20);  // Adjust this to control the capture interval
}