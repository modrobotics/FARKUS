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
        
   