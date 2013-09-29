import FarkusPartType

class FarkusPart():
    "Class to define the ACTUAL parts on a FARKUS Table"
    def __init__(self, partType):
        self.id = 1  #TODO: Generate UUID
        self.partType = partType
        self.testResults = []
        self.serialNumber = None # Use for Cubelet ID
        
    def getPartType(self):
        return self.partType
    
    def setSerialNumber(self, sn):
        self.serialNumber = sn
    
    def getSerialNumber(self):
        return self.serialNumber
    
    def addTestResult(self, result):
        self.testResults.append(result)
    
    