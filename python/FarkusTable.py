import FarkusModuleManager
import FarkusModuleTypeManager
import FarkusConveyance

class FarkusTable():
    "Class to define the ACTUAL FARKUS Table"
    # TODO: Combine FarkusTable and FarkusModuleManager classes....why did I do it this way?  Rushed. That's why.
    def __init__(self, gui):
        self.gui = gui
        
        # Initialize FARKUS Manager Singletons
	self.conveyance = FarkusConveyance.FarkusConveyance(False) #this should be reworked to extend the "module" class and not have to be instanciated before it's ready to install
	self.moduleTypeManager = FarkusModuleTypeManager.FarkusModuleTypeManager();
	self.moduleManager = FarkusModuleManager.FarkusModuleManager();
        
    def setConveyance(self, conveyance):
        self.convenyance = conveyance
    
    def getConveyance(self):
        return self.conveyance
    
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
        pass
    
    def start(self):
        #update status variable
        pass
    
    def eStop(self, ):
        #update status variable
        pass
    
    
    
    
