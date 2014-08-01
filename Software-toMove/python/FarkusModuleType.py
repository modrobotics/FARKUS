class FarkusModuleType():
    "Class to define the types of modules that can be used on a FARKUS Table"
    def __init__(self, id, serialIDString, name, shortName1, shortName2):
        self.id = id
        self.serialIDString = serialIDString
        self.name = name
        self.shortName1 = shortName1
        self.shortName2 = shortName2
        
    def getName(self):
        return self.name
    
    def setName(self, name):
        self.name = name
    
    def getShortName1(self):
        return self.shortName1
    
    def setShortName1(self, name):
        self.shortName1 = name
        
    def getShortName2(self):
        return self.shortName2
    
    def setShortName2(self, name):
        self.shortName2 = name
        
    def getSerialIDString(self):
        return self.serialIDString
    
    def setSerialIDString(self, serialIDString):
        self.serialIDString = serialIDString
    
    def getID(self):
        return self.id
    