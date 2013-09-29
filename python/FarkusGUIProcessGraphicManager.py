import wx

class FarkusGUIProcessGraphicManager():
    "Class to define the some helper functions to manage the process graphic"
    def __init__(self, gui):
        self.gui = gui
        
        # Setup!
        self.InitGUI()
    
    def InitGUI(self):
        
        # Insert the placeholder labels
        
        # Create textfields for Cubelet IDs
	self.gui.module1CubeID = wx.StaticText(self.gui, -1, '---------', pos=(105,248))
	self.gui.module2CubeID = wx.StaticText(self.gui, -1, '---------', pos=(208,248))
	self.gui.module3CubeID = wx.StaticText(self.gui, -1, '---------', pos=(313,248))
	self.gui.module4CubeID = wx.StaticText(self.gui, -1, '---------', pos=(418,248))
	self.gui.module5CubeID = wx.StaticText(self.gui, -1, '---------', pos=(518,248))
	self.gui.module6CubeID = wx.StaticText(self.gui, -1, '---------', pos=(623,248))
      
      
	self.gui.module1Name1 = wx.StaticText(self.gui, -1, 'No Module', pos=(93,100))
        self.gui.module1Name2 = wx.StaticText(self.gui, -1, 'Connected', pos=(93,120))
        self.gui.module1Configure = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(88,150))

	self.gui.module2Name1 = wx.StaticText(self.gui, -1, 'No Module', pos=(195,100))
        self.gui.module2Name2 = wx.StaticText(self.gui, -1, 'Connected', pos=(195,120))
        self.gui.module2Configure = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(191,150))
        
	self.gui.module3Name1 = wx.StaticText(self.gui, -1, 'No Module', pos=(297,100))
        self.gui.module3Name2 = wx.StaticText(self.gui, -1, 'Connected', pos=(297,120))
        self.gui.module3Configure = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(293,150))
        
	self.gui.module4Name1 = wx.StaticText(self.gui, -1, 'No Module', pos=(403,100))
        self.gui.module4Name2 = wx.StaticText(self.gui, -1, 'Connected', pos=(403,120))
        self.gui.module4Configure = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(397,150))
        
	self.gui.module5Name1 = wx.StaticText(self.gui, -1, 'No Module', pos=(504,100))
        self.gui.module5Name2 = wx.StaticText(self.gui, -1, 'Connected', pos=(504,120))
        self.gui.module5Configure = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(500,150))
        
	self.gui.module6Name1 = wx.StaticText(self.gui, -1, 'No Module', pos=(607,100))
        self.gui.module6Name2 = wx.StaticText(self.gui, -1, 'Connected', pos=(607,120))
        self.gui.module6Configure = wx.StaticText(self.gui, -1, '[ Configure ]', pos=(603,150))
        pass
    
    def update(self):
        pass
    
        
   