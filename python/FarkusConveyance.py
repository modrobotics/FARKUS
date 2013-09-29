import SerialWorker
import SerialResultEventHandler


class FarkusConveyance():
    "Class to define the Conveyance on a FARKUS Table"
    def __init__(self, serialPortIdentifier):
        self.isReady=False
        
        self.configState = None
	self.attachedParts = []
	self.partHolderCount = 12
	self.partHolderPitch = 2
	self.serialIDString = "`0000"
	self.name = "***FARKUS-Conveyance"
	
	self.serialPortIdentifier = serialPortIdentifier
	self.serialWorker = None
        self.isConnected = False
        
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
    
    def go(self):
        #Command to serial, bool
	self.serialWorker.write("GO")
	
	#TODO: check for echo and bool return
        return True;
    
    def eStop(self):
        #Command to serial, bool
        pass
    
    def blinkForID(self):
        #Command to serial, bool
        pass
    
    def getActualModuleId(self):
        #command to serial return
        pass
    
    def getName(self):
        return self.name
    
    def setName(self, name):
        self.name = name
    
    def getConfigState(self):
        return self.configState

    def setConfigState(self, configState):
        self.configState = configState
    
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


    
    