import SerialWorker
import SerialResultEventHandler

import FarkusPart
import FarkusPartTypeManager
import FarkusPartType
from collections import deque

class FarkusConveyance():
    "Class to define the Conveyance on a FARKUS Table"
    def __init__(self, serialPortIdentifier, partTypeManager, gui):
        
	self.name = "***FARKUS-Conveyance"
	self.serialIDString = "`0000"
	self.partHolderCount = 11
	self.partHolderPitch = None
	
	self.attachedParts = deque([], (self.partHolderCount+2)) # init deque with extra spot for shifting
	for i in range(0,self.partHolderCount+1):
		self.insertEmptyPartHolder()
	
	self.serialPortIdentifier = serialPortIdentifier
	self.serialWorker = None
	self.isConnected = False
	self.isReady=False
	self.configState = None		
	self.partTypeManager = partTypeManager
	self.gui = gui

        # Setup Serial, connect, set isConnected via function
        
        # Wait for RDY, set is_ready, is_ready event
        pass
 
    def connect(self):
	# Attempt serial connection, wait for RDY, set is ready, is_ready event
        pass
    
    def remove(self ):
        # Estop module
        # Disconnect Serial
        # Set self to false
        pass
    
    # adds the part, shifts the parts in the attachedParts array, advances the conveyance
    def insertNewPart(self, partTypeId):
	self.attachedParts.appendleft(FarkusPart.FarkusPart( self.partTypeManager.getPartTypeById(partTypeId) ) )
	self.removePartOnExit()
	self.advanceForward()
	self.gui.processGraphicManager.updatePartInformation()
	pass
	
    def insertEmptyPartHolder(self):
	self.attachedParts.appendleft(None)
	pass
	
    def removePartOnExit(self):
	removedPart = self.attachedParts.pop()
	pass
 
    def getConnectedParts(self):
	return self.attachedParts
    
    def go(self):
        #Command to serial, bool
	self.serialWorker.write("GO")
	
	#TODO: check for echo and bool return
        return True;
    
    def advanceForward(self):
	self.setConfigState(0)  # Go forward TODO: constants for commands
	self.go()
    
    def advanceBackward(self):
	self.setConfigState(1)  # Go backward TODO: constants for commands
	self.go()

    def eStop(self):
        #Command to serial, bool
	self.serialWorker.write("ESTOP")
        pass
    
    def configModule(self, state):
        self.serialWorker.write("C" + str(self.configState))
        return True

    def blinkForID(self):
        #Command to serial, bool
        pass
    
    def getActualModuleId(self):
        #command to serial return
	self.serialWorker.write("I")
        pass
    
    def getName(self):
        return self.name
    
    def setName(self, name):
        self.name = name
    
    def getConfigState(self):
        return self.configState

    def setConfigState(self, configState):
	if(configState != self.configState):
		self.configState = configState
		self.configModule(configState)
    
    def getIsReady(self):
        return self.isReady
    
    def getExpectedSerialIDString(self):
        return self.serialIDString
    
    def setSerialWorker(self, serialWorker):
	self.serialWorker = serialWorker
    
    def getSerialPortIdentifier(self):
        return self.serialPortIdentifier

    def setSerialPortIdentifier(self, identifier):
        self.serialPortIdentifier = identifier


    
    