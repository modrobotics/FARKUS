import FarkusPartType

class FarkusPartTypeManager():
    "Class to define helper functions for FarkusPartTypes"
    def __init__(self):
        self.partTypes = []
        
        # TODO: Load these from a shelf.  For now we'll statically define them
        cubeRequiredTests = []
        cubeRequiredTests.append(1) # Get Cubelet ID
        cubeRequiredTests.append(2) # Test Power Transfer capabilities
        cubeRequiredTests.append(3) # Test Application Code Communication Capabilities (implies test should be run after Flashing)
        cubeRequiredTests.append(4) # Flash Brightness Cubelet Firmware
        cubeRequiredTests.append(5) # Brightness Cubelet Functional Test
        self.partTypes.append(FarkusPartType.FarkusPartType(1, "Cubelet: Brightness", cubeRequiredTests))
        
        cubeRequiredTests = []
        cubeRequiredTests.append(1) # Get Cubelet ID
        cubeRequiredTests.append(2) # Test Power Transfer capabilities
        cubeRequiredTests.append(3) # Test Application Code Communication Capabilities (implies test should be run after Flashing)...technically this should be different for different block types...again, another day.
        cubeRequiredTests.append(6) # Flash Flashlight Cubelet Firmware
        cubeRequiredTests.append(7) # Brightness Cubelet Functional Test
        self.partTypes.append(FarkusPartType.FarkusPartType(2, "Cubelet: Flashlight", cubeRequiredTests))
        

    # returns partType object if found, False if not.
    def getPartTypeById(self, id):
        for partType in self.partTypes:
            if( partType.getId() == id):
                return partType
        return False;
    
    def getPartTypes(self, ):  # should add a "part family" to the partType class to make separating cubelets/EYVO/tesla motors products easy
        return self.partTypes
    
   