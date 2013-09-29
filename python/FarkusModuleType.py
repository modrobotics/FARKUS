class FarkusModuleType():
    "Class to define the types of modules that can be used on a FARKUS Table"
    def __init__(self, id, serialIDString, name):
        self.id = id
        self.serialIDString = serialIDString
        self.name = name
        
    def getName(self):
        return self.name
    
    def setName(self, name):
        self.name = name
    
    def getSerialIDString(self):
        return self.serialIDString
    
    def setSerialIDString(self, serialIDString):
        self.serialIDString = serialIDString
    
    def getID(self):
        return self.id
    