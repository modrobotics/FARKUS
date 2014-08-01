import FarkusModuleManager
import FarkusModuleTypeManager
import FarkusConveyance
import FarkusPartTypeManager
import FarkusTestType
import FarkusTestTypeManager

class FarkusTable():
    "Class to define the ACTUAL FARKUS Table"
    # TODO: Combine FarkusTable and FarkusModuleManager classes....why did I do it this way?  Rushed. That's why.
    def __init__(self, gui):
        self.gui = gui
        
        # Initialize FARKUS Manager Singletons
	self.moduleTypeManager = FarkusModuleTypeManager.FarkusModuleTypeManager();
	self.partTypeManager = FarkusPartTypeManager.FarkusPartTypeManager();
	self.moduleManager = FarkusModuleManager.FarkusModuleManager();
 	self.testTypeManager = FarkusTestTypeManager.FarkusTestTypeManager();
       
        # Initialize Conveyance
        self.conveyance = FarkusConveyance.FarkusConveyance(False, self.partTypeManager, self.gui) #this should be reworked to extend the "module" class and not have to be instanciated before it's ready to install
    
        # Initialize State
        self.state = "OFFLINE"
        
    def setConveyance(self, conveyance):
        self.convenyance = conveyance
    
    def getConveyance(self):
        return self.conveyance
    
    def setPartTypeManager(self, manager):
        self.partTypeManager = manager
    
    def getPartTypeManager(self):
        return self.partTypeManager
    
    def setModuleTypeManager(self, manager):
        self.moduleTypeManager = manager
    
    def getModuleTypeManager(self):
        return self.moduleTypeManager
    
    def setModuleManager(self, manager):
        self.moduleManager = manager
    
    def getModuleManager(self):
        return self.moduleManager
    
    def pause(self):
        #update status variable...should this be pausing modules?
        self.status = "PAUSED"
        pass
    
    def start(self):
        #update status variable
        self.status = "RUNNING"
        pass
    
    def eStop(self, ):
        #update status variable
        self.status = "ESTOP"
        pass
    
    
    
    
