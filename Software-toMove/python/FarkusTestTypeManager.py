import FarkusTestType

class FarkusTestTypeManager():
    "Class to define the helper functions for working with FarkusTestTypes"
    # Eventually this class should be expanded and generalized to extend a "FarkusActions" class.  But that, my friends, is for another day.
    def __init__(self):
        self.testTypes = []
        
        # TODO: Load these from a shelf.  For now we'll statically define them
            #def __init__(self, ident, name, runBefore, runAfter, commandToStart, specificResponse, runTimeEstimate):
        self.testTypes.append( FarkusTestType.FarkusTestType(1, "Get Cubelet ID", [], [], "SN", False, 1))
        self.testTypes.append( FarkusTestType.FarkusTestType(2, "Test Cubelet Power Transfer Abilities", [], [], ["C0", "GO"], "PASS", 10))
        self.testTypes.append( FarkusTestType.FarkusTestType(3, "Test Application Code Communication Capabilities", [5,7], [4,6], ["C0", "GO"], "PASS", 5))
        self.testTypes.append( FarkusTestType.FarkusTestType(4, "Flash Brightness Cubelet Firmware", [3], [1], ["C0", "GO"], "PASS", 10))
        self.testTypes.append( FarkusTestType.FarkusTestType(5, "Flashlight Cubelet Functional Test", [], [4,1,2,3], ["C0", "GO"], "PASS", 5))
        self.testTypes.append( FarkusTestType.FarkusTestType(6, "Flash Flashlight Cubelet Firmware", [3], [1], ["C1", "GO"], "PASS", 10))
        self.testTypes.append( FarkusTestType.FarkusTestType(7, "Brightness Cubelet Functional Test", [], [4,1,2,3], ["C1", "GO"], "PASS", 5))
        