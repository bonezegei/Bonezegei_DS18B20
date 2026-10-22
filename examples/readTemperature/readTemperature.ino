#include <Bonezegei_DS18B20.h>

#define SENSOR_PIN 4

Bonezegei_DS18B20 ds(SENSOR_PIN);

void setup() {
    Serial.begin(115200);
    ds.begin();
}

void loop() {
    if (ds.read()) {
        Serial.print("Temp C: ");
        Serial.print(ds.getTemperatureC());
        Serial.print(" °C | Temp F: ");
        Serial.print(ds.getTemperatureF());
        Serial.println(" °F");
    } else {
        Serial.println("Error reading DS18B20 sensor!");
    }
    delay(2000);
}