class FarkusModule():
    "Class to define the ACTUAL MODULES on a FARKUS Table"
    def __init__(self, moduleType, moduleTypeManager, serialPortIdentifier):
        self.isReady=False
        self.tablePosition = None
        
        self.moduleTypeManager = moduleTypeManager
        self.moduleType = self.moduleTypeManager.getModuleTypeBySerialIDString(moduleType)
        
        self.configState = None
        
        self.serialWorker = None
        self.serialPortIdentifier = serialPortIdentifier
        
        # Setup Serial, connect, set isConnected via function
        
        # Wait for RDY, set is_ready, is_ready event
        pass
        
    def connect(self ):
        # Attempt serial connection, wait for RDY, set is ready, is_ready event
        pass
    
    def remove(self ):
        # Estop module
        # Disconnect Serial
        # Set self to false
        pass
    
    #  MODULE COMMANDS ARE HERE ##################
    #    Overall TODO: check for echo of command, boolean return
    #            TODO: constants for commands

    def go(self):
	self.serialWorker.write("GO")
        return True
    
    def eStop(self):
        self.serialWorker.write("ESTOP")
        return True
    
    def blinkForID(self):
        self.serialWorker.write("BLINK")
        return True
    
    def getActualModuleId(self):
        self.serialWorker.write("I")
        return True
    
    def configModule(self, state):
        self.serialWorker.write("C" + state)
        return True
    
    # #############################################
    
    # Housekeeping setters/getters below
    def getTablePosition(self):
        return self.tablePosition
        
    def setTablePosition(self, position):
        self.tablePosition = position
    
    def getName(self):
        if( self.moduleType is not None ):
            return self.moduleType.getName()
        else:
            return False
    
    def setName(self, name):
        if( self.moduleType is not None ):
            self.moduleType.setName(name)
            return True
        else:
            return False
    
    def getConfigState(self):
        return self.configState

    def setConfigState(self, configState):
        if self.configModule(self.configState) is True:
            # Set the local cache
            self.configState = configState
        else:
            return False
        
    
    def getIsReady(self):
        return self.isReady
    
    def setSerialWorker(self, serialWorker):
	self.serialWorker = serialWorker
    
    def getExpectedSerialIDString(self):
        return self.moduleType.getSerialIDString()
        
    def getSerialPortIdentifier(self):
        return self.serialPortIdentifier
    
    
    
    
    