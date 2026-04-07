#include "modules/CameraModule.h"
#ifdef CAMERA_SUPPORTED
#include "esp_camera.h"
#include "esp_timer.h"

CameraModule::CameraModule(){}

void CameraModule::Init()
{
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
    config.xclk_freq_hz = XCLK_FREQ_HZ;
    config.pixel_format = PIXFORMAT_JPEG; 
    config.frame_size = FRAMESIZE_HVGA;
    config.jpeg_quality = JPEG_QUALITY; 
    config.fb_count = FB_COUNT;
    config.grab_mode = CAMERA_GRAB_LATEST;
    
    esp_camera_init(&config);
    startCameraServer();
}

void CameraModule::Loop(CommunicationModule* interface, SensorsData *data){}

esp_err_t CameraModule::stream_handler(httpd_req_t *req)
{
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    char part_buf[64];

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(33);

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);

    if(res != ESP_OK) return res;

    while(true){
        fb = esp_camera_fb_get();
        
        if (!fb) 
        {
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        continue;
        }

        size_t hlen = snprintf(part_buf, 64, _STREAM_PART, fb->len);
        res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        
        if(res == ESP_OK)
        res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        if(res == ESP_OK)
        res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));  
        
        esp_camera_fb_return(fb);
        
        if(res != ESP_OK) break;
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
    return res;
}

void CameraModule::startCameraServer()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.max_uri_handlers = 1;
    config.recv_wait_timeout = 1; 
    config.send_wait_timeout = 1;

    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = CameraModule::stream_handler,
        .user_ctx  = NULL
    };

    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &index_uri);
    }
}
#endif