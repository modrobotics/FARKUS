import wx

class SerialResultEvent0(wx.PyEvent):
	def __init__(self, data, moduleType, moduleLocation, moduleLongName, wxEventID, module):
		wx.PyEvent.__init__(self)
		self.SetEventType(wxEventID)
		self.data = data
		self.moduleLocation = moduleLocation
		self.moduleType = moduleType
		self.moduleLongName = moduleLongName
		self.module = module