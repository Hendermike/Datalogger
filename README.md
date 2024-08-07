# Datalogger

This is a minimalistic integration of a ATMega328p (Arduino Nano), Real-Time Clock and SD card datalogger module. 
It was originally developed to quantify the autonomy of electronic products under development, signaled by the state
of specific measurent points in the targeted equipment. That is why it only stores minimum value of measurements over 
a 60 seconds window. <br\>

Depending on signal of interest's nature and levels, appropiate conditioning circuits must be considered in order to map
the information within the ATMega328p analog pin range (optocoupling, rectification, scaling, etc.).
