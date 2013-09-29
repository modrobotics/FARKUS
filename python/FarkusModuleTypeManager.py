import FarkusModuleType

class FarkusModuleTypeManager():
    "Class to define helper functions for FarkusModuleTypes"
    def __init__(self):
        self.moduleTypes = []
        
        # TODO: Load these from a shelf.  For now we'll statically define them
        self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823987,  "`0001", "***Cubelets Flashlight/Brightness Test Module"))
        self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823987,  "`0002", "***Cubelets Communication Test Module"))
        self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823987,  "`0003", "***Cubelets Power Test Module"))
        
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
        
   