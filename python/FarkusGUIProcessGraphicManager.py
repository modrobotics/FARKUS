import wx

class FarkusGUIProcessGraphicManager():
    "Class to define the some helper functions to manage the process graphic"
    def __init__(self, gui, moduleManager ):
        self.gui = gui
        self.moduleManager = moduleManager
        
        self.gui.moduleCubeID = [None]*10
        self.gui.moduleName1 = [None]*10
        self.gui.moduleName2 = [None]*10
        self.gui.moduleConfigureText = [None]*10
        
        # Setup!
        self.InitGUI()
    
    def setModuleManager(self, manager):
        self.moduleManager = manager
    
    def InitGUI(self):
        
        # Insert the placeholder labels
        
        # Create textfields for Cubelet IDs
	self.gui.moduleCubeID[1] = wx.StaticText(self.gui, -1, '---------', pos=(105,248))
	self.gui.moduleCubeID[2] = wx.StaticText(self.gui, -1, '---------', pos=(208,248))
	self.gui.moduleCubeID[3] = wx.StaticText(self.gui, -1, '---------', pos=(313,248))
	self.gui.moduleCubeID[4] = wx.StaticText(self.gui, -1, '---------', pos=(418,248))
	self.gui.moduleCubeID[5] = wx.StaticText(self.gui, -1, '---------', pos=(518,248))
	self.gui.moduleCubeID[6] = wx.StaticText(self.gui, -1, '---------', pos=(623,248))
    
        self.gui.moduleName1[1] = wx.StaticText(self.gui, -1, 'No Module', pos=(93,100))
        self.gui.moduleName2[1] = wx.StaticText(self.gui, -1, 'Connected', pos=(93,120))
        self.gui.moduleConfigureText[1] = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(88,150))

	self.gui.moduleName1[2] = wx.StaticText(self.gui, -1, 'No Module', pos=(195,100))
        self.gui.moduleName2[2] = wx.StaticText(self.gui, -1, 'Connected', pos=(195,120))
        self.gui.moduleConfigureText[2] = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(191,150))
        
	self.gui.moduleName1[3] = wx.StaticText(self.gui, -1, 'No Module', pos=(297,100))
        self.gui.moduleName2[3] = wx.StaticText(self.gui, -1, 'Connected', pos=(297,120))
        self.gui.moduleConfigureText[3] = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(293,150))
        
	self.gui.moduleName1[4] = wx.StaticText(self.gui, -1, 'No Module', pos=(403,100))
        self.gui.moduleName2[4] = wx.StaticText(self.gui, -1, 'Connected', pos=(403,120))
        self.gui.moduleConfigureText[4] = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(397,150))
        
	self.gui.moduleName1[5] = wx.StaticText(self.gui, -1, 'No Module', pos=(504,100))
        self.gui.moduleName2[5] = wx.StaticText(self.gui, -1, 'Connected', pos=(504,120))
        self.gui.moduleConfigureText[5] = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(500,150))
        
	self.gui.moduleName1[6] = wx.StaticText(self.gui, -1, 'No Module', pos=(607,100))
        self.gui.moduleName2[6] = wx.StaticText(self.gui, -1, 'Connected', pos=(607,120))
        self.gui.moduleConfigureText[6] = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(603,150))
        
        # Create Indicator shapes for part holders
        
    def updateAll(self):
        self.updateModuleNames()
        self.updatePartInformation()
    
    def updateModuleNames(self):
        for i in range(1,7):
            foundModule = self.moduleManager.getModuleByTablePosition(i)
            if(foundModule):
                # We have a module in position i
                self.gui.moduleName1[i].SetLabel(foundModule.getShortName1())
                self.gui.moduleName2[i].SetLabel(foundModule.getShortName2())
            else:
                # This position is empty
                self.gui.moduleName1[i].SetLabel('No Module')
                self.gui.moduleName2[i].SetLabel('Connected')
    
    def updatePartInformation(self):
        
        #for i in range(1,11):
           

            try:
                for i in range(0,11):  # 11 part holders
                    part = self.gui.farkusTable.getConveyance().getConnectedParts()[i]

                    if i == 0:
                        moduleLocation = 1
                    elif i == 2:
                        moduleLocation = 2
                    elif i == 4:
                        moduleLocation = 3
                    elif i == 6:
                        moduleLocation = 4
                    elif i == 8:
                        moduleLocation = 5
                    elif i == 10:
                        moduleLocation = 6
                    else:
                        moduleLocation = None
		    
                    if part is not None:
                        if moduleLocation is not None:
                                self.gui.moduleCubeID[moduleLocation].SetLabel(str(part.getSerialNumber()))
                                #self.LogToGUI("Part Holder #" + str(i) + " has " + self.farkusTable.getConveyance().getConnectedParts()[i].getPartType().getName())
                        elif moduleLocation is not None:
                                #self.LogToGUI("Part Holder #" + str(i) + " is empty")
                                #self.gui.moduleCubeID[moduleLocation].SetLabel("---------" + str(i))
                                pass
                
            #if(partHolder is not None):
            #    # There is a part on this part holder,
            #    if(moduleLocation is not None):
            #        # and this holder is in a module
            #        sn = partHolder.getSerialNumber()
            #        if(sn is not None):
            #            # serial number known
            #            self.gui.moduleCubeID[moduleLocation].SetLabel(str(sn))
            #        else:
            #            # We don't know the serial Number of this part
            #            self.gui.moduleCubeID[moduleLocation].SetLabel(partHolder.getName())
            #    else:
            #        # There is a part here, but it's not in a module
            ##        pass
            #else:
                # This position is empty
                #self.gui.moduleCubeID[modu].SetLabel("---------" + str(i))
            #    pass
            except:
                pass