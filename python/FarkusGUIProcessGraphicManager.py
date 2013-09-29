import wx

class FarkusGUIProcessGraphicManager():
    "Class to define the some helper functions to manage the process graphic"
    def __init__(self, gui, moduleManager ):
        self.gui = gui
        self.moduleManager = moduleManager
        
        # Setup!
        self.InitGUI()
    
    def setModuleManager(self, manager):
        self.moduleManager = manager
    
    def InitGUI(self):
        
        # Insert the placeholder labels
        
        # Create textfields for Cubelet IDs
	self.gui.module1CubeID = wx.StaticText(self.gui, -1, '---------', pos=(105,248))
	self.gui.module2CubeID = wx.StaticText(self.gui, -1, '---------', pos=(208,248))
	self.gui.module3CubeID = wx.StaticText(self.gui, -1, '---------', pos=(313,248))
	self.gui.module4CubeID = wx.StaticText(self.gui, -1, '---------', pos=(418,248))
	self.gui.module5CubeID = wx.StaticText(self.gui, -1, '---------', pos=(518,248))
	self.gui.module6CubeID = wx.StaticText(self.gui, -1, '---------', pos=(623,248))
      
        self.gui.moduleName1 = [None]*10
        self.gui.moduleName2 = [None]*10
        self.gui.moduleConfigureText = [None]*10
        
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
        