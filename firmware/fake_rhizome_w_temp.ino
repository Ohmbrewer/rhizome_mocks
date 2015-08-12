#include "Ohmbrewer_Rhizome_Temperature_Sensor.h"

Ohmbrewer_Temperature_Sensor ob_ts = Ohmbrewer_Temperature_Sensor("1");
    
void setup() {
    // None of these would be necessary if this weren't a mock. 
    //Instead, you might have a special initialization function for the real-deal.
    Spark.function("temps", tempCtrl);
    Spark.variable("test_temp", &ob_ts.test_temperature, DOUBLE);
    Spark.variable("current_temp", &ob_ts.current_temperature, DOUBLE);
}

/**
 * Nothing to do here but check the sensor.
 */
void loop() {
    // Read the sensor.
    ob_ts.readSensor();
    
    // Wait 30 seconds between reads.
    delay(30000);
}

/**
 * Handler for the /temps function. Won't be necessary for a non-mock.'
 */
int tempCtrl(String command) {
    ob_ts.setTestTemperature(command);
}

