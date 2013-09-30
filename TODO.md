
MUST HAVE TODO
#######################
*Proper response handling on serial workers, sending back error code
*Respond to emercency stop 
*Hookup "parts" type menu
*Flashing modules final bugfix(s)
*Automatic advance / preventing premature advancement
*Arm position detection, state status reporting
*characterize test: power
*3D print 12 more part faces

++++++++++++++++++++++++

*Update cubelets shieild
*Mechanical safety: EStop buttons, pinch point elimination/covers
*failure mode tracking
*make spare faces
*maintenance schedule: Tracking, alerts
touch screen

MISC TODO
########################

*Known issue: if only conveyance is connected, you won't be able to get to the
    conveyance's config window. which is fine, since there's nothing to do in there anyway
    
*FarkusTable state control system
*Functions to support  farkustesttype, required tests, etc
   - Control Module Actions
   - Update part status after
   - 
*Look for TODOs in code. Duh.
*Enums for Commands
*implement commands to getCurrentConfigState on modules
*Manager class/functions in FarkusTable to manage module location constraints (2 modules in 1 location, etc.)
*Click on part-> Part details window with tests to run, results, serial number, added date/time, etc
*Click on module-> Module details, config.  WE have basic config.  Add buttons/text for manual commands, status debug info
*Ensure everything is properly destroy()'d before close()---memory leak risk
*serial failures aren't properly updating the Statusbar..fix that.
