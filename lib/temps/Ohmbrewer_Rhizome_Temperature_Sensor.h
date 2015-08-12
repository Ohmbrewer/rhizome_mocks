/**
 * This library provides Temperature Sensor equipment functions for the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_TEMPERATURE_SENSOR_H
#define OHMBREWER_RHIZOME_TEMPERATURE_SENSOR_H

#define publish_delay 10000
    
#include "application.h"

class Ohmbrewer_Temperature_Sensor {
  
  public:
  
    // Here's some things that we need to keep track of:
    String id;
    unsigned int lastPublish;
    double current_temperature; // Stored as Celsius
    double test_temperature; // Stored as Celsius

    // Event stream names
    String error_stream;
    String debug_stream;
    String temps_stream;

    /**
     * Friendly neighborhood constructor.
     */
    Ohmbrewer_Temperature_Sensor(String newID);
    
    /**
     * Changes the names of the streams to publish to for notifications
     * @param String e Error stream name
     * @param String d Debug stream name
     */
    void modifyStreams(String e, String d);

    /**
     * Publishes the status of the temperature sensor
     */
    void publishStatus();
    
    /**
     * Reports the temperature sensor reading
     * @returns Response code
     */
    int readSensor();
    
    /**
     * Reports the temperature sensor reading in Fahrenheit
     * @returns The temperature sensor reading in Fahrenheit
     */
    double fahrenheit();
    
    
    /** Stuff that's only needed for the mock **/
    
    /**
     * Sets the test temperature value. Should be provided in Celsius.
     * @params String testTemp The new testing temperature
     * @returns int Success code
     */
    int setTestTemperature(String testTemp);
    
    /**
     * Returns a random value within a range.
     * @params int minRand Minimum range
     * @params int maxRand Maximum range
     * @returns int Random integer value within specified range.
     */
    int random(int minRand, int maxRand);

};

#endif


