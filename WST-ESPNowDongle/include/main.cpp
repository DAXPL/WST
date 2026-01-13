#include <esp_now.h>
#include <WiFi.h>

void setup(){

}

void loop(){
    int throttle = 0;
    if (Serial.available() > 0){
        throttle = Serial.parseInt();

        if (Serial.read() == '\n'){
            Serial.print("Throttle: ");
            Serial.println(throttle);

            uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

            // Structure to hold the data to be sent
            typedef struct DroneControlData {
                int throttle;
                int yaw;
                int pitch;
                int roll;
            } DroneControlData;

            DroneControlData myData;
            myData.throttle = throttle;

            // Send message via ESP-NOW
            esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        }
    }
}