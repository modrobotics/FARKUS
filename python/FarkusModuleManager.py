import FarkusModule

class FarkusModuleManager():
    "Class to define helper functions for FarkusModule"
    def __init__(self):
        self.modules = []
        self.installedModuleCount = 0
        
    def getModuleByLocation(self, location):
        pass
    
    def add(self, module):
        self.modules.append(module);
        self.installedModuleCount +=1
        pass
        
    def remove(self, module):
        pass
        
    def removeByLocation(self, location):
        pass
        
    def removeAll(self):
        pass
    
    def getModuleBySerialPort(self, port):  #not safe if there are two of the same module installed
        for module in self.modules:
            if( module.getSerialPortIdentifier() == port):
                return module
        return False;
    
    def getPresentModules (self):
        return self.modules
    
    def moveModule(self, locationStart, locationEnd):
        pass