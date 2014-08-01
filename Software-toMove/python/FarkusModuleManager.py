import FarkusModule

#TODO: unify language: "installed module" vs "connected" or "attached" module

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
        self.modules = []       # TODO: potential memory leak here...threads getting abandoned.  Not sure how python handles this.
        self.installedModuleCount = 0
        return True
        
    
    def getModuleBySerialPort(self, port):  #not safe if there are two of the same module installed
        for module in self.modules:
            if( module.getSerialPortIdentifier() == port):
                return module
        return False;
    
    def getModuleByTablePosition(self, position):
        for module in self.modules:
            if( module.getTablePosition() == position):
                return module
        return False;
    
    def getPresentModules (self):
        return self.modules
    
    def getConnectedModules(self):
        out = []
        for module in self.modules:
            #if( module.isConnected() is True):
                out.append(module)
        return out
    
    def moveModule(self, locationStart, locationEnd):
        pass
    
    def getConnectedModuleCount(self):
        return self.installedModuleCount  # TODO: use this function more often. I'm using len(getConnectedModules()) in places
    