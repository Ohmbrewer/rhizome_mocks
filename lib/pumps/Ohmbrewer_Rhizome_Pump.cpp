#include "Ohmbrewer_Rhizome_Pump.h"

/**
 * Friendly neighborhood constructor.
 */
Ohmbrewer_Pump::Ohmbrewer_Pump(String newPumpID) {
    // Used to store the intensity level of the individual LEDs, intitialized to off (0)
    redIntensity = 0;
    greenIntensity = 0;
    blueIntensity = 0;
    
    // Here's some things that we need to keep track of:
    if(newPumpID == NULL || newPumpID.equals("")) {
        pumpID = "1";
    } else {
        pumpID = newPumpID;
    }
    pump_stream = "pumps/" + pumpID;
    
    lastPublish = 0;
    stopTime = "0";
    state    = "off";
    speed    = "-1";

    // Event stream names
    this->modifyStreams("rhizome_errors", "rhizome_debug");
}

/**
 * Changes the names of the streams to publish to for notifications
 * @param String e Error stream name
 * @param String d Debug stream name
 */
void Ohmbrewer_Pump::modifyStreams(String e, String d) {
    this->error_stream = e;
    this->debug_stream = d;
}

/**
 * Sets the LED intensity values according to the global variables.
 */
void Ohmbrewer_Pump::refreshLEDs() {
    RGB.color(redIntensity, greenIntensity, blueIntensity);
}

/**
 * Sets the LED intensity values to off (0).
 */
void Ohmbrewer_Pump::turnOffLEDs() {
    RGB.color(0, 0, 0);
    this->state = "off";
}

/**
 * Convenience function for another way to say refreshLEDs()
 */
void Ohmbrewer_Pump::turnOnLEDs() {
    this->refreshLEDs();
    this->state = "on";
}

/**
 * Parses a String of RGB values and uses the result to set the LED intensities and change the LED color.
 * @param command The comma-delimited RGB value string
 */
int Ohmbrewer_Pump::setColor(String command) {
    // String jsonDebugMsg = "{ \"msg\": \"Attempting to set color!\", \"command\": \"" + command + "\"}";
    // Spark.publish(debug_stream, jsonDebugMsg, 60, PRIVATE);
    if (command) {
        char inputStr[64];
        command.toCharArray(inputStr,64);
        char *p = strtok(inputStr,",");
        this->redIntensity = atoi(p);
        p = strtok(NULL,",");
        this->greenIntensity = atoi(p);
        p = strtok(NULL,",");
        this->blueIntensity = atoi(p);
        p = strtok(NULL,",");
        
        this->refreshLEDs();
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * Turns the "pump" on and off
 * @param command Either "on" or "off". Not case sensitive.
 */
int Ohmbrewer_Pump::togglePump(String command) {
    // String jsonDebugMsg = "{ \"msg\": \"Attempting to toggle LEDs!\", \"command\": \"" + command + "\"}";
    // Spark.publish(debug_stream, jsonDebugMsg, 60, PRIVATE);
    if (command.equalsIgnoreCase("on")) {
        this->turnOnLEDs();
        return 1;
    } else if (command.equalsIgnoreCase("off")) {
        this->turnOffLEDs();
        return 0;
    } else {
        String jsonErrorMsg = "{ \"msg\": \"Pump toggle did not work...\", \"command\": \"" + command + "\"}";
        Spark.publish(this->error_stream, jsonErrorMsg, 30, PRIVATE);
        return -136;
    }
}

/**
 * Sets the "pump" speed, here represented by the on-board LED.
 * @param speed The provided pump speed
 */
int Ohmbrewer_Pump::setSpeed(String speedUpdate) {
    // String jsonDebugMsg = "{ \"msg\": \"Attempting to set pump speed!\", \"command\": \"" + speedUpdate + "\"}";
    // Spark.publish(debug_stream, jsonDebugMsg, 60, PRIVATE);
    int speedSuccess = -1;
    if(speedUpdate != NULL) {
        // Set the color
        switch (speedUpdate.toInt()) {
          case isRed:
            speedSuccess = this->setColor(RED);
            break;
          case isYellow:
            speedSuccess = this->setColor(YELLOW);
            break;
          case isGreen:
            speedSuccess = this->setColor(GREEN);
            break;
          case isBlue:
            speedSuccess = this->setColor(BLUE);
            break;
          case isOut:
            // This code should make the color saved in memory equal to no light.
            speedSuccess = this->setColor(BLACKOUT);
            break;
          default:
            this->setColor(FULL_BLAST);
            speedSuccess = -2;
            String jsonErrorMsg = "{ \"msg\": \"Invalid color supplied...\", \"speed\": \"" + speedUpdate + "\"}";
            Spark.publish(this->error_stream, jsonErrorMsg, 30, PRIVATE);
        }
    }
    
    if(speedSuccess == 1) {
        speed = speedUpdate;
    }
    return speedSuccess;
}

/**
 * Sets the "pump" stop time. When the stop time is reached, the pump automatically changes to OFF.
 * @param stateUpdate The state given in the message setting the stop time. Stop Time is reset to '0' when an OFF command is sent.
 * @param stopTimeUpdate The new Stop Time. Ignored if the stateUpdate is OFF.
 */
int Ohmbrewer_Pump::setStopTime(String stateUpdate, String stopTimeUpdate) {
   if(stateUpdate.equalsIgnoreCase("on") && ( (stopTimeUpdate.toInt() < Time.now()) || (stopTimeUpdate.toInt() < 0) )) {
        // You need to provide a stop time in the future...
        String jsonErrorMsg = "{ \"msg\": \"Attempted to set stop time, but time was invalid!\", \"command\": \"" + stopTimeUpdate + "\"}";
        Spark.publish(this->error_stream, jsonErrorMsg, 30, PRIVATE);
        return -Time.now();
    } else if(stateUpdate.equalsIgnoreCase("off")) {
        // Reset the stop time
        stopTime = "0";
    } else {
        // Otherwise, go ahead and set the stop time
        // String jsonDebugMsg = "{ \"msg\": \"Attempting to set stop time!\", \"command\": \"" + stopTimeUpdate + "\"}";
        // Spark.publish(debug_stream, jsonDebugMsg, 30, PRIVATE);
        this->stopTime = stopTimeUpdate;
    }
}

/**
 * Publishes the status of the pump
 * @param state The current state (on/off)
 * @param speed The current speed (1..4)
 */
void Ohmbrewer_Pump::publishStatus() {
    unsigned long now = millis();
    String jsonStatusA = "{ \"id\": \"" + pumpID + "\", \"state\": \"" + state + "\", \"stopTime\":\"" + stopTime + "\", \"speed\": \"" + speed + "\"}";
    String jsonStatusB = "{ \"id\": \"" + pumpID + "\", \"state\": \"" + state + "\", \"stopTime\":\"" + stopTime + "\"}";
    
    if(speed != NULL) {
        Spark.publish(this->pump_stream, jsonStatusA, 30, PRIVATE);
    } else {
        Spark.publish(this->pump_stream, jsonStatusB, 30, PRIVATE);
    }
    
    lastPublish = now;
}

/**
 * Compares the current time against the stop time provided in the last instruction and kills the pump
 * if we've reached quitting time.
 */
void Ohmbrewer_Pump::checkForQuittingTime() {
    // First of all, there's nothing to do if we aren't pumping. That'll happen if
    // 1) Stop time < 1 (it starts at 0)
    // 2) State == Off
    if((this->stopTime.toInt() > 0) && this->state.equalsIgnoreCase("on") ) {
        
        // So we're pumping! We want to shut off if we've reached the stopping time
        if(Time.now() >= this->stopTime.toInt()) {
            // Cut that pump!
            int toggleSuccess = this->togglePump("off");

            if(toggleSuccess != 0) {
                // Pump failed to shut off
                String jsonErrorMsg = "{ \"msg\": \"Stopping time reached! Tried to stop the pump, but for some reason it did not stop!\","
                                       " \"state\": \"" + this->state + "\", \"error_code\": \"" + toggleSuccess + "\"}";
                Spark.publish(this->error_stream, jsonErrorMsg, 30, PRIVATE);
            } else {
                // Success!
                String jsonSuccessMsg = "{ \"msg\": \"Stopping time reached. Pump deactivated.\", \"state\": \"" + this->state + "\"}";
                Spark.publish(this->debug_stream, jsonSuccessMsg, 30, PRIVATE);
                this->publishStatus();
            }
        }
    }
}

//** Handlers **//

/*
 * This funtion will turn on/off the LED and set it to the appropriate color
 * Usage - "args=ON,1" will turn the LED on and set the color to Red
 * Color settings are:
 * 1 - RED
 * 2 - YELLOW
 * 3 - GREEN
 * 4 - BLUE
 * Errors (invalid color choices) will set the LED to full power for all RGB.
 * @param command The current state (on/off) and speed (1..4), comma-delimited and case-insensitive
 */
int Ohmbrewer_Pump::pumpCtrl(String command) {
    int    speedSuccess  = -1;
    int    toggleSuccess = -1;
    char * params        = new char[command.length() + 1];
    strcpy(params, command.c_str());
    
    // Parse the parameters
    String desiredPumpID  = String(strtok(params, ","));
    String stateUpdate    = String(strtok(NULL, ","));
    String stopTimeUpdate = String(strtok(NULL, ","));
    String speedUpdate    = "";
    
    if(stopTimeUpdate != NULL && stopTimeUpdate.toInt() > 0) {
        // Let's try to avoid null pointer exceptions...
        speedUpdate = "" + String(strtok(NULL, ",")); // Note that the quotes are a workaround for a known issue with the String class
    }

    if((desiredPumpID == NULL) || (!desiredPumpID.equalsIgnoreCase(this->pumpID))) {
        // Not doing anything with it right now, but we'll error out if it isn't provided properly
        return -9000;
    }
    
    this->setStopTime(stateUpdate, stopTimeUpdate);
    
    speedSuccess = this->setSpeed(speedUpdate);
    
    if(state != NULL) {
        // Toggle the LED, if appropriate
        toggleSuccess = this->togglePump(stateUpdate);
    }
    
    this->publishStatus();
    if(speedSuccess > 0 && toggleSuccess != -136) {
        return speedSuccess;
    }
    
    return toggleSuccess;
}

