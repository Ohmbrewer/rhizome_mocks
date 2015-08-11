/**
 * This library provides Pump equipment functions for the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_PUMP_H
#define OHMBREWER_RHIZOME_PUMP_H

#define publish_delay 10000
    
#include "application.h"

// Provide some named default colors
const char RED[8]     = "255,0,0";
const char YELLOW[10] = "255,255,0";
const char BLUE[8]    = "0,0,255";
const char GREEN[8]   = "0,255,0";
const char FULL_BLAST[12] = "255,255,255";
const char BLACKOUT[6]    = "0,0,0";

// We'll also map the default colors to pump speeds, for a nice visual
const int isRed    = 1;
const int isYellow = 2;
const int isGreen  = 3;
const int isBlue   = 4;
const int isOut    = 0;

class Ohmbrewer_Pump {
  
  public:
    // Used to store the intensity level of the individual LEDs, intitialized to off (0)
    int redIntensity;
    int greenIntensity;
    int blueIntensity;

    // Here's some things that we need to keep track of:
    String pumpID;
    unsigned int lastPublish;
    String stopTime;
    String state;
    String speed;

    // Event stream names
    String error_stream;
    String debug_stream;
    String pump_stream;

    /**
     * Friendly neighborhood constructor.
     */
    Ohmbrewer_Pump(String pumpID);
    
    /**
     * Changes the names of the streams to publish to for notifications
     * @param String e Error stream name
     * @param String d Debug stream name
     */
    void modifyStreams(String e, String d);

    /**
     * Sets the LED intensity values according to the global variables.
     */
    void refreshLEDs();

    /**
     * Sets the LED intensity values to off (0).
     */
    void turnOffLEDs();
    
    /**
     * Convenience function for another way to say refreshLEDs()
     */
    void turnOnLEDs();

    /**
     * Parses a String of RGB values and uses the result to set the LED intensities and change the LED color.
     * @param command The comma-delimited RGB value string
     */
    int setColor(String command);

    /**
     * Turns the "pump" on and off
     * @param command Either "on" or "off". Not case sensitive.
     */
    int togglePump(String command);

    /**
     * Sets the "pump" speed, here represented by the on-board LED.
     * @param speed The provided pump speed
     */
    int setSpeed(String speedUpdate);

    /**
     * Sets the "pump" stop time. When the stop time is reached, the pump automatically changes to OFF.
     * @param stateUpdate The state given in the message setting the stop time. Stop Time is reset to '0' when an OFF command is sent.
     * @param stopTimeUpdate The new Stop Time. Ignored if the stateUpdate is OFF.
     */
    int setStopTime(String stateUpdate, String stopTimeUpdate);

    /**
     * Publishes the status of the pump
     * @param state The current state (on/off)
     * @param speed The current speed (1..4)
     */
    void publishStatus();

    /**
     * Compares the current time against the stop time provided in the last instruction and kills the pump
     * if we've reached quitting time.
     */
    void checkForQuittingTime();

    /**
     * Quickly indicates whether the pump is ON or OFF.
     * @returns boolean True if the pump is ON, False if the pump is OFF, NULL otherwise.
     */
    bool isRelayOn();

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
    int pumpCtrl(String command);

};

#endif

