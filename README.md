# SMPTE2022controller
The code for the internal microblaze controller which controls the SMPTE2022-5/6 receiver system.

In order to actually use this, a BSP for a specific project will need to be linked in the Xilinx SDK.
This will contain all of the definitions and functions required for this code to compile.

Define __SIM to optimise the program for simulation.

Limitations:

-Untested

-Not all features implemented

-May have issues with modifying matching data for systems using seamless switching
