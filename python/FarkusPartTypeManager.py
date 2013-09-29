import FarkusPartType

class FarkusPartTypeManager():
    "Class to define helper functions for FarkusPartTypes"
    def __init__(self):
        self.partTypes = []
        
        # TODO: Load these from a shelf.  For now we'll statically define them
        self.partTypes.append(FarkusModuleType.FarkusModuleType(1, "Cubelet: Brightness", None))
        self.partTypes.append(FarkusModuleType.FarkusModuleType(2, "Cubelet: Flashlight", None))
        
    def getModuleByLocation(self, location):
        pass
    
    def add(self, module):
        pass
        
    def remove(self, module):
        pass
        
    def removeByLocation(self, location):
        pass
        
    def removeAll(self):
        pass
    
    # returns the moduleType object if found, False if not.
    def getModuleTypeBySerialIDString(self, identifier):
        for moduleType in self.moduleTypes:
            if( moduleType.getSerialIDString() == identifier):
                return moduleType
        return False;
        
   