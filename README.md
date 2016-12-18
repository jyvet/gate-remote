CAME TOP432NA remote control with a Arduino
===========================================

Emit CAME TOP432NA remote controls frames (used for gate or garage
door) with a Arduino board and a FS1000A 433Mhz RF Transmitter.


Hardware Setup
--------------

* Connect the FS1000A to the digital pin 12.
* Use a 17cm wire as an antenna.

![Hardware Setup](/data/hardware_setup.png)


Frame characteristics
---------------------

    Start bit (header)  __       10 bits code       __ Ending 2 bits (trailer)
                          |  _________|_________   |
                          0 [X X X X X X X X X X] 0 1

       Shape of the zero pulse                Shap of the one pulse
                 ___                                  ______
             ___|   |___                             |      |___

    Legend: _ is 100µs

