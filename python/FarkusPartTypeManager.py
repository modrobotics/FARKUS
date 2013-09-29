import FarkusPartType

class FarkusPartTypeManager():
    "Class to define helper functions for FarkusPartTypes"
    def __init__(self):
        self.partTypes = []
        
        # TODO: Load these from a shelf.  For now we'll statically define them
        self.partTypes.append(FarkusPartType.FarkusPartType(1, "Cubelet: Brightness", None))
        self.partTypes.append(FarkusPartType.FarkusPartType(2, "Cubelet: Flashlight", None))
        

    # returns partType object if found, False if not.
    def getPartTypeById(self, id):
        for partType in self.partTypes:
            if( partType.getId() == id):
                return partType
        return False;
    
    def getPartTypes(self, ):  # should add a "part family" to the partType class to make separating cubelets/EYVO/tesla motors products easy
        return self.partTypes
    
   