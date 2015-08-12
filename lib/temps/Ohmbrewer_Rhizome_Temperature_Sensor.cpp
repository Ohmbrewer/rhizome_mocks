#include "Ohmbrewer_Rhizome_Temperature_Sensor.h"

/**
 * Friendly neighborhood constructor.
 */
Ohmbrewer_Temperature_Sensor::Ohmbrewer_Temperature_Sensor(String newID) {
    // Here's some things that we need to keep track of:
    if(newID == NULL || newID.equals("")) {
        id = "1";
    } else {
        id = newID;
    }
    temps_stream = "temps/" + id;
    
    lastPublish = 0;
    current_temperature = 0;
    test_temperature = 0;

    // Event stream names
    this->modifyStreams("rhizome_errors", "rhizome_debug");
}

/**
 * Changes the names of the streams to publish to for notifications
 * @param String e Error stream name
 * @param String d Debug stream name
 */
void Ohmbrewer_Temperature_Sensor::modifyStreams(String e, String d) {
    this->error_stream = e;
    this->debug_stream = d;
}

/**
 * Publishes the status of the pump
 * @param state The current state (on/off)
 * @param speed The current speed (1..4)
 */
void Ohmbrewer_Temperature_Sensor::publishStatus() {
    unsigned long now = millis();
    String jsonStatus = "{ \"id\": \"" + this->id + "\", \"current_temp\": \"" + this->current_temperature + "\"}";
    
    Spark.publish(this->temps_stream, jsonStatus, 30, PRIVATE);
    
    lastPublish = now;
}

//** Handlers **//
    
/**
 * Reports the temperature sensor reading
 * @returns Response code
 */
int Ohmbrewer_Temperature_Sensor::readSensor() {
    this->current_temperature = random((int)(this->test_temperature*100) - 50, (int)(this->test_temperature*100) + 50) / 100;
    this->publishStatus();
    return 0;
}
    
/**
 * Reports the temperature sensor reading in Fahrenheit
 * @returns The temperature sensor reading in Fahrenheit
 */
double Ohmbrewer_Temperature_Sensor::fahrenheit() {
    return (this->current_temperature * 1.8) + 32;
}
    
/**
 * Sets the test temperature value. Should be provided in Celsius.
 * @params String testTemp The new testing temperature
 * @returns int Success code
 */
int Ohmbrewer_Temperature_Sensor::setTestTemperature(String testTemp) {
    this->test_temperature = atof(testTemp.c_str());
    return 0;
}
    
/**
 * Returns a random value within a range.
 * @params int minRand Minimum range
 * @params int maxRand Maximum range
 * @returns int Random integer value within specified range.
 */
int Ohmbrewer_Temperature_Sensor::random(int minRand, int maxRand) {
    return rand() % (maxRand-minRand+1) + minRand;
}


