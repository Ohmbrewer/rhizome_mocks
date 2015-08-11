// This #include statement was automatically added by the Particle IDE.
#include "Ohmbrewer_Rhizome_Pump.h"

Ohmbrewer_Pump ob_p = Ohmbrewer_Pump("1");
    
void setup() {
  //set LED pins to outputs
  RGB.control(true);
  Spark.function("pumps", relayCtrl);
  
  // For good measure, let's also make sure the LED is off when we start:
  ob_p.refreshLEDs();
}

/**
 * Nothing to do here but see if it's time to stop...
 */
void loop() {
    ob_p.checkForQuittingTime();
}


int relayCtrl(String command) {
    ob_p.pumpCtrl(command);
}

