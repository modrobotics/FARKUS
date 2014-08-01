import wx
import sys

class FarkusConfigureModuleWindow(wx.Dialog):
    
    def __init__(self, parent, position, moduleManager, gui, **kw):
        super(FarkusConfigureModuleWindow, self).__init__(None, **kw)
        self.position = position
        self.moduleManager = moduleManager
        self.gui = gui
        
        self.InitUI(position)
        self.SetSize((620, 400))
        self.lastSelection = None
            
    def updateModuleList(self):
        self.time_zones.Clear()
        
        i=0
        for module in self.moduleManager.getPresentModules():
            if module.getTablePosition() is not None:
                moduleName = module.getName() + " in Position " + str(module.getTablePosition())  #TODO: might need to use the moduleManager to manage this to make swapping easier and without 2 modules in one spot
            else:
                moduleName = module.getName() + " ( POSITION UNASSIGNED )"
                
            self.time_zones.Insert(moduleName,0, module.getSerialPortIdentifier() )
            i+=1

    
    def InitUI(self, position):

        pnl = wx.Panel(self)
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.time_zones = wx.ListBox(pnl, 26, wx.DefaultPosition, (600, 130), [], wx.LB_SINGLE)
        
        # Populate the module list
        self.updateModuleList()
        
        # TODO: select the previous one
        self.time_zones.SetSelection(0)
        self.lastSelection = 0
        
        hbox2 = wx.BoxSizer(wx.HORIZONTAL)
        
        closeButton = wx.Button(self, label='Cancel')
        hbox2.Add(closeButton, flag=wx.LEFT, border=5)

        applyButton = wx.Button(self, label='Apply')
        hbox2.Add(applyButton, flag=wx.LEFT, border=5)
        
        okButton = wx.Button(self, label='OK')
        hbox2.Add(okButton, flag=wx.LEFT, border=5)
        
        vbox.Add(pnl, proportion=1, 
            flag=wx.ALL|wx.EXPAND, border=5)
        vbox.Add(hbox2, 
            flag=wx.ALIGN_CENTER|wx.TOP|wx.BOTTOM, border=10)

        self.SetSizer(vbox)
        
        closeButton.Bind(wx.EVT_BUTTON, self.OnCancel)
        okButton.Bind(wx.EVT_BUTTON, self.OnOk)
        applyButton.Bind(wx.EVT_BUTTON, self.OnApply)
        
        
    def OnCancel(self, e):
        self.Destroy()
        
    def OnOk(self, e):
        self.OnApply(None)
        self.Destroy()
        
    def OnApply(self, e):
        index = self.time_zones.GetSelection()
        
        # Just in case - prevents strange stuff later on
        if index < 0 or index is None or index is False:
            index = self.lastSelection
            
        else:
            self.lastSelection = index
        
        # Save these down for later
        string = self.time_zones.GetString(index)
        clientData = self.time_zones.GetClientData(index)
        
        # Assign the above selected module to the self.location
        selectedModule = self.moduleManager.getModuleBySerialPort(clientData)
        selectedModule.setTablePosition(self.position)
        
        # Update the list
        self.updateModuleList()
        
        # Events de-select the list? WTF?
        self.time_zones.SetSelection(index)
        
        self.gui.processGraphicManager.updateAll()
        