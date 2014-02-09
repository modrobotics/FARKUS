#!/usr/bin/python

from threading import *
import wx
from time import sleep
import subprocess
#import serial
import sys
import os
import signal
import shelve
import time
#from serial.tools import list_ports


import serial
from serial.serialutil import SerialException
from serialutils import full_port_name, enumerate_serial_ports, FARKUS_get_serial_ID_string

import SerialWorker
import ProgrammerWorkerThread
import FarkusModuleType
import FarkusModuleTypeManager
import FarkusModule
import FarkusModuleManager
import FarkusConveyance
import FarkusConfigureModuleWindow
import FarkusGUIProcessGraphicManager
import FarkusStandalone

#if os.name == 'nt':
BASE_PATH = "C:\Users\jmoyes\Desktop\ModRobotics\GitHub\FARKUS"
#BASE_PATH = "/home/pi/FARKUS/python"
SETTINGS_DATA_FILE = BASE_PATH + "\Settings.dat"

SETTINGS_DEFAULTS = dict([
	('CIMS_NODE_KEY', "9f0e657558f8c1992676ff2d0356883b"),
	('CIMS_NODE_ID', 2),
	('CIMS_SERVER_KEY', "3LT9HhhZ5s7mZKaF"),
	('CIMS_SERVER_URL', "http://cims.internal.modrobotics.com/CIMSServer/CIMSServer.php"),
	('AUTOMODE', False)
])


# Button definitions
ID_START = wx.NewId()
ID_PAUSE = wx.NewId()
ID_ESTOP = wx.NewId()
ID_ADVANCE = wx.NewId()
ID_ADDPART = wx.NewId()

# Menu Button Definitions
ID_OPTIONS_EDITSETTINGS = wx.NewId()
ID_OPTIONS_STARTDUMB = wx.NewId()
ID_EXIT = wx.NewId
ID_OPTIONS_OPENSERIAL = wx.NewId()
ID_OPTIONS_CLOSESERIAL = wx.NewId()
ID_OPTIONS_CAROUSEL = wx.NewId()
ID_OPTIONS_CONNECT_TO_TESTBED = wx.NewId()
ID_OPTIONS_AUTOCONNECT = wx.NewId()
ID_OPTIONS_CONNECT_TO_TESTBED_MOSS = wx.NewId()
ID_OPTIONS_SELECT_MODULE_ANGLEPOT = wx.NewId()
ID_OPTIONS_SELECT_MODULE_DISTANCE = wx.NewId()
ID_OPTIONS_SELECT_MODULE_FLASHLIGHT = wx.NewId()
ID_OPTIONS_SELECT_MODULE_BTMAIN = wx.NewId()
ID_OPTIONS_SELECT_MODULE_MICROPHONE = wx.NewId()
ID_OPTIONS_SELECT_MODULE_SPINMAIN = wx.NewId()
ID_OPTIONS_CONNECT_TO_TESTBED_CUBELETS = wx.NewId()
ID_OPTIONS_SELECT_MODULE = wx.NewId()

# Settings Window Button Definitions
ID_SETTINGS_CANCEL = wx.NewId()
ID_SETTINGS_SAVE = wx.NewId()

# Thread Communication Events
EVT_NEWSERIALDATA0_ID = wx.NewId()
EVT_NEWPROGRAMMERDATAEVENT_ID = wx.NewId()


def EVT_NEWPROGRAMMERDATAEVENT(win, func):
	win.Connect(-1, -1, EVT_NEWPROGRAMMERDATAEVENT_ID, func)
	
def EVT_NEWSERIALDATA0(win, func):
	win.Connect(-1, -1, EVT_NEWSERIALDATA0_ID, func)

EVT_CONFIGMODULE_ID = wx.NewId()

def EVT_CONFIGMODULE(win, func):
	win.Connect(-1, -1, EVT_CONFIGMODULE_ID, func)


class ConfigModuleEvent(wx.PyEvent):
	def __init__(self, moduleLocation):
		wx.PyEvent.__init__(self)
		self.SetEventType(EVT_CONFIGMODULE_ID)
		self.moduleLocation = moduleLocation

########################################################################
class SystemTabPanel(wx.Panel):
	"""
	This will be the first notebook tab
	"""
	#----------------------------------------------------------------------
	def __init__(self, parent):
		wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)
		
		sizer = wx.BoxSizer(wx.VERTICAL)
		#txtOne = wx.TextCtrl(self, wx.ID_ANY, "")
		#txtTwo = wx.TextCtrl(self, wx.ID_ANY, "")
		
		# Create the logger output box
		messagesHeader = wx.StaticText(self, wx.ID_ANY, 'Messages (Newest at Top) ', pos=(300,360))
		self.logDisplay = wx.TextCtrl(self, wx.ID_ANY, pos = (300, 375), size = (401, 350), style = wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_AUTO_URL)
		self.logDisplay2 = wx.TextCtrl(self, wx.ID_ANY, pos = (300, 375), size = (401, 350), style = wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_AUTO_URL)
		#linesInLogBuffer = 0 # initialize a counter variable
	
		sizer = wx.BoxSizer(wx.VERTICAL)
		sizer.Add(messagesHeader, 0, wx.ALL, 5)
		sizer.Add(self.logDisplay, 1, wx.ALL, 5)
		sizer.Add(self.logDisplay2, 1, wx.LEFT, 5)
		self.SetSizer(sizer)
	def MessageToLogger(self, text):
		self.logDisplay.SetValue(text)

	def GetLoggerPanel(self ):
		return self.logDisplay
	
	
########################################################################
class TabPanel2(wx.Panel):
    """
    This will be the first notebook tab
    """
    #----------------------------------------------------------------------
    def __init__(self, parent):
        """"""

        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)
	
        sizer = wx.BoxSizer(wx.VERTICAL)
        #txtOne = wx.TextCtrl(self, wx.ID_ANY, "")
        #txtTwo = wx.TextCtrl(self, wx.ID_ANY, "")

        sizer = wx.BoxSizer(wx.VERTICAL)
        #sizer.Add(txtOne, 0, wx.ALL, 5)
        #sizer.Add(txtTwo, 0, wx.ALL, 5)

        self.SetSizer(sizer)

########################################################################
class TabPanel3(wx.Panel):
    """
    This will be the first notebook tab
    """
    #----------------------------------------------------------------------
    def __init__(self, parent):
        """"""

        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)
	
        sizer = wx.BoxSizer(wx.VERTICAL)
        txtOne = wx.TextCtrl(self, wx.ID_ANY, "")
        txtTwo = wx.TextCtrl(self, wx.ID_ANY, "")

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(txtOne, 0, wx.ALL, 5)
        sizer.Add(txtTwo, 0, wx.ALL, 5)

        self.SetSizer(sizer)


class NotebookDemo(wx.Notebook):
    
	def getSystemLogger(self):
		return self.tabOne.GetLoggerPanel()
	
	#----------------------------------------------------------------------
	def __init__(self, parent):
		wx.Notebook.__init__(self, parent, id=wx.ID_ANY, style=
				     #wx.BK_DEFAULT
				     wx.BK_TOP 
				     #wx.BK_BOTTOM
				     #wx.BK_LEFT
				     #wx.BK_RIGHT
				     )
	    
		# Create the first tab and add it to the notebook
		self.tabOne = SystemTabPanel(self)
		self.AddPage(self.tabOne, "** System ** ")
		#parent.logDisplay = self.tabOne.GetLoggerPanel()
		
		self.tabOne.MessageToLogger("ASDASD")
		# Show how to put an image on one of the notebook tabs,
		# first make the image list:
		#il = wx.ImageList(16, 16)
		#idx1 = il.Add(images.Smiles.GetBitmap())
		#self.AssignImageList(il)
	    
		# now put an image on the first tab we just created:
		#self.SetPageImage(0, idx1)
	    
		# Create and add the second tab
		tabTwo = TabPanel2(self)
		tabTwo.SetBackgroundColour("Red")
		self.AddPage(tabTwo, "TabTwo")
		
		# Create and add the second tab
		tabThree = TabPanel3(self)
		tabThree.SetBackgroundColour("Blue")
		self.AddPage(tabThree, "TabThree")
	    
	    
	
	'''
		self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
		self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
	def OnPageChanged(self, event):
	    old = event.GetOldSelection()
	    new = event.GetSelection()
	    sel = self.GetSelection()
	    print 'OnPageChanged,  old:%d, new:%d, sel:%d\n' % (old, new, sel)
	    event.Skip()
	
	def OnPageChanging(self, event):
	    old = event.GetOldSelection()
	    new = event.GetSelection()
	    sel = self.GetSelection()
	    print 'OnPageChanging, old:%d, new:%d, sel:%d\n' % (old, new, sel)
	    event.Skip()
	'''
	









# GUI Frame class that spins off the worker thread
class MainFrame(wx.Frame):
	def onPause( self, event ):
	
		self.LogToGUI("System entering paused state")
		self.farkusTable.pause()

		return True

	
	def onAdvance( self, event ):
		self.farkusTable.getConveyance().insertEmptyPartHolder() # empty part holder
		pass
	
	def onAddPart( self, event ):
		while ( self.farkusTable.getConveyance().isClearToAdvance()  is not True ):
			self.LogToGUI("Waiting for CTA")			# TODO: CTA should be an event thrown when all modules are done, onAddPart and onAdvance should set an indicator bit that the onCTA handler checks
			pass
		self.farkusTable.getConveyance().insertNewPart(1) # new brightness onboard!
		pass
	
	def onStart( self, event ):
		self.OnOpenSerial(False);  # Discover Modules, establish connections	
		pass
	
	def onEstop( self, event ):
	
		eStopNotice = wx.MessageDialog(self, "EMERGENCY STOP ACTIVATED\n\nPress OK to RESUME", "", wx.OK | wx.ICON_HAND)
		self.LogToGUI("EMERGENCY STOP ACTIVATED!!")
		for i in self.serialWorkers:
			try:
				i.write("ESTOP")
			except:
				pass
	   
		result = eStopNotice.ShowModal() == wx.OK
		eStopNotice.Destroy()
		self.LogToGUI("Emergency Stop Released.  Entering PAUSED State.");	
		
		pass
	
	def __OnLeftDown( self, event ):
		pos = event.GetPositionTuple()
		self.getProcessGraphicClickEventHandler(pos[0], pos[1]);
		#self.LogToGUI("X: " + str(pos[0]) + " Y: " + str(pos[1]))
		event.Skip()
		
	def getProcessGraphicClickEventHandler(self, positionX, positionY):
		if( self.isInSquare(positionX, positionY, 75, 59, 160, 214 ) ):   # Module 1
			wx.PostEvent(self, ConfigModuleEvent(1))
		elif( self.isInSquare(positionX, positionY, 178, 59, 264,214 ) ): # Module 2
			wx.PostEvent(self, ConfigModuleEvent(2))
		elif( self.isInSquare(positionX, positionY, 282, 59, 365, 214 ) ):  # Module 3
			wx.PostEvent(self, ConfigModuleEvent(3))
		elif( self.isInSquare(positionX, positionY, 385, 59, 469, 214 ) ): # Module 4
			wx.PostEvent(self, ConfigModuleEvent(4))
		elif( self.isInSquare(positionX, positionY, 487, 59, 573, 214 ) ):  # Module 5
			wx.PostEvent(self, ConfigModuleEvent(5))
		elif( self.isInSquare(positionX, positionY, 590, 59, 677, 214 ) ): # Module 6
			wx.PostEvent(self, ConfigModuleEvent(6))
		elif( self.isInSquare(positionX, positionY, 18, 287, 88, 334 ) ): # Conveyance
			wx.PostEvent(self, ConfigModuleEvent(0))
		else:
			#self.LogToGUI("No button pressed")
			pass
		pass
	
	def ConfigModule(self, event ):	
		# Configure new window, show, save, destroy...but only if we have modules connected
		if(self.farkusTable.getModuleManager().getConnectedModuleCount() > 0):
			chgdep = FarkusConfigureModuleWindow.FarkusConfigureModuleWindow(None, event.moduleLocation, self.farkusTable.getModuleManager(), self, title='Details: Module ' + str(event.moduleLocation) )
			chgdep.ShowModal()
			chgdep.Destroy()
		else:
			dlg = wx.MessageDialog(None, "There are no modules connected.\n\nPlease run \"Connect and Initialize\" first.", "Fark!", wx.OK | wx.ICON_WARNING)
			dlg.ShowModal()
			dlg.Destroy()
			
	
	
	def isInSquare(self, posX, posY, startX, startY, endX, endY ):  #start=top left, end=bottom right. 
		# is in X range
		if( posX >= startX and posX <=endX):
			#Y in range
			if( posY >= startY and posY <=endY):
				#Y in range
				return True
		return False
	
	def __init__(self, parent, id):
		wx.Frame.__init__(self, parent, id, 'Modular Robotics FARKUS Desk v0.1', size=(935,535), style= wx.SYSTEM_MENU | wx.CAPTION | wx.CLOSE_BOX)
		
		panel = wx.Panel(self)

		notebook = NotebookDemo(panel)
		sizer = wx.BoxSizer(wx.VERTICAL)
		sizer.Add(notebook, wx.ALL, wx.EXPAND)
		panel.SetSizer(sizer)
		self.logDisplay = notebook.getSystemLogger()
		
		self.Layout()
		self.Show()
		
		
		# Initialize FARKUS Manager Singletons
		self.farkusTable = FarkusStandalone.FarkusStandalone(self);
		
		# Set window BG Color to match BG of logo image
		self.SetBackgroundColour((203,226,244))
		
		# Add the FARKUS Background
		#backgroundPath = BASE_PATH + '/inc/background.png'
		#png = wx.Image(backgroundPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		#pngBitMap = wx.StaticBitmap(self, -1, png, (10, 5), (png.GetWidth(), png.GetHeight()))
		#pngBitMap.Bind( wx.EVT_LEFT_DOWN, self.__OnLeftDown) 
		
		
		# Create menus...
		self.menubar = wx.MenuBar()
		self.fileMenu = wx.Menu()
		self.optionsMenu = wx.Menu()
		self.connectMenu = wx.Menu()
		
		self.connectToTestbedSubMenu = wx.Menu()
		
		self.autoConnect = wx.MenuItem(self.connectToTestbedSubMenu, ID_OPTIONS_AUTOCONNECT, 'AutoConnect to Modules', '')
		self.optionsMenu.AppendItem(self.autoConnect)

		self.disconnectAll = wx.MenuItem(self.optionsMenu, ID_OPTIONS_CLOSESERIAL, 'Disconnect All', 'Disconnect from the FARKUS Array')
		self.optionsMenu.AppendItem(self.disconnectAll)

		'''
		self.optionsMenu.AppendMenu(ID_OPTIONS_CONNECT_TO_TESTBED, 'Connect to Testbed', self.connectToTestbedSubMenu)

		self.connectToTestbedCubeletsSubMenu = wx.Menu()
		self.connectToTestbedSubMenu.AppendMenu(ID_OPTIONS_CONNECT_TO_TESTBED_CUBELETS, 'Cubelets', self.connectToTestbedCubeletsSubMenu)

		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Cubelets Motherboard-A v20 (w/ FARKUS Carousel)', ''))
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Cubelets Motherboard-A AND Motherboard-D v20 (Standalone)', ''))
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Cubelets Bluetooth 2.0 FIRST PASS (w/ testbed, enhanced mode)', ''))
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Cubelets Bluetooth 2.0 REWORK ONLY', ''))
		
		self.connectToTestbedCubeletsSubMenu.AppendSeparator()
		
		# Legacy stuff here
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Legacy: Cubelets Motherboard-A All v10 and v11 Battery (Standalone)', ''))
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Legacy: Cubelets Motherboard-A All v11 (ATMEGA168V/P) (Standalone)', ''))
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Legacy: Cubelets Bluetooth v10 Bootloader (ATMEGA162) (Standalone)', ''))
		self.connectToTestbedCubeletsSubMenu.AppendItem(wx.MenuItem(self.connectToTestbedCubeletsSubMenu, ID_OPTIONS_SELECT_MODULE, 'Legacy: Cubelets Bluetooth v10 Test/Config Routine (ATMEGA162) (Standalone)', ''))
		'''
		
		
				
		# Create the logger output box
		#self.logDisplay = wx.TextCtrl(self, id = -1, pos = (10, 375), size = (10, 10), style = wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_AUTO_URL)
		#self.linesInLogBuffer = 0 # initialize a counter variable
		
		# Bind GUI events to their handlers
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.autoConnect)
		self.Bind(wx.EVT_MENU, self.OnCloseSerial, self.disconnectAll)
	
		self.quitItem = wx.MenuItem(self.fileMenu, wx.ID_EXIT, '&Quit', 'Exit the Modular Robotics FARKUS Desk')
		self.fileMenu.AppendItem(self.quitItem)
		self.Bind(wx.EVT_MENU, self.OnQuitApp, self.quitItem)

		self.menubar.Append(self.fileMenu, '&File')
		self.menubar.Append(self.optionsMenu, '&Connect')
		self.SetMenuBar(self.menubar)
		 
		# Variable to hold worker threads
		self.serialWorkers = []
		self.serialWorkers.append(None)
		self.serialWorkers.append(None)
		self.serialWorkers.append(None)
		self.serialWorkers.append(None)
		self.serialWorkers.append(None)
		self.serialWorkers.append(None)
	
		# Variable to hold worker threads
		self.programmerWorkers = []
		self.programmerWorkers.append(None)
		self.programmerWorkers.append(None)
		self.programmerWorkers.append(None)
		self.programmerWorkers.append(None)
		self.programmerWorkers.append(None)
		self.programmerWorkers.append(None)

		# Declare Thread events for inter-thread communication
		EVT_NEWSERIALDATA0(self,self.OnNewSerialData)
		EVT_NEWPROGRAMMERDATAEVENT(self,self.OnNewProgrammerEvent)
		#EVT_CONFIGMODULE(self,self.ConfigModule)
		
		# Icon Me!
		#	iconFile = BASE_PATH + "/inc/icon.ico"
		#	icon1 = wx.Icon(iconFile, wx.BITMAP_TYPE_ICO)
		#	self.SetIcon(icon1)
		
		# Show the application center in the user's screen
		self.Centre()
		
		#self.processGraphicManager.setModuleManager(self.farkusTable.getModuleManager())
		
		# Redirect STDOUT, STDERR to our logger now that we've rendered
		sys.stdout=RedirectSTDOUT_STDERR(self)
		sys.stderr=RedirectSTDOUT_STDERR(self)
		
		# Detect and attach to modules on startup
		self.OnSelectModule(False)

		
	def OnSelectModule(self, event):
		#self.LogToGUI("Searching For Modules....")
		eventSource = False
		try:
			eventSource = event.GetId()
		
		except:
			pass
		#Hacky!  
		if eventSource == ID_OPTIONS_SELECT_MODULE_ANGLEPOT:
			self.LogToGUI("Angle")
		elif eventSource == ID_OPTIONS_SELECT_MODULE_DISTANCE:
			self.LogToGUI("Distance")
		elif eventSource == ID_OPTIONS_SELECT_MODULE_BTMAIN:
			self.LogToGUI("BT Main")
		elif eventSource == ID_OPTIONS_SELECT_MODULE_FLASHLIGHT:
			self.LogToGUI("Flashlight")
		elif eventSource == ID_OPTIONS_SELECT_MODULE_MICROPHONE:
			self.LogToGUI("Mic")
		elif eventSource == ID_OPTIONS_SELECT_MODULE_SPINMAIN:
			self.LogToGUI("Spin Main")
		#else:
			#self.LogToGUI("Unknown Module")
				
		# search the serial ports for available devices
		self.OnOpenSerial(False)
		
	def OnOpenSerial(self, event):
	
		# This function will remove all current devices and reconnect.
		# TODO: allows discovery/connection of NEW modules only.  Maybe that could run period 
		
				# Close existing connections
		#self.statusBar.SetStatusText('Disconnecting All Devices') 
		
		for i in self.serialWorkers:
			try:
				if i.isAlive() and i.ser.isOpen():
					i.ser.close()
					i.abort()
			except:
				pass
		
		self.availablePorts = []
		self.availableModuleTypes = []
		self.availableModuleLocations = []
		self.availableModuleLongNames = []
		
		# Remove all of the modules currently "on the table"
		self.farkusTable.getModuleManager().removeAll()
		#self.processGraphicManager.updateAll()

		#self.statusBar.SetStatusText('Searching for FARKUS-Compatible Modules') 
		self.LogToGUI("Searching for FARKUS-Compatible Modules")
		
		self.foundDevices = 0
		
		# Windows
		if os.name == 'nt':
			ports = enumerate_serial_ports()
			if ports:
				for portname in ports:
					try:
						self.GetSerialDeviceIDAndAdd(portname)
					except:
						continue;
		# Unix / OSX
		else:
			for port in list_ports.comports():
				# TODO put this back!
				pass
		
		
		if len(self.availablePorts) > 0:
			
			#self.statusBar.SetStatusText('Attempting to connect to discovered devices...') 
			pass
		else:
			print "No compatible devices were found"
			#self.processGraphicManager.updateStatusBar()  # offline
			pass
			
		# Close existing connections
		for i in self.serialWorkers:
			try:
				if i.isAlive() and i.ser.isOpen():
					i.ser.close()
					i.abort()
			except:
				pass
			
		# Open connections to the modules we found
		
		for i in range(len(self.availablePorts)):
			temp = None
			temp = self.farkusTable.getModuleManager().getModuleBySerialPort(self.availablePorts[i])

			# Create and set the serial worker
			self.serialWorkers[i] = SerialWorker.SerialWorkerThread0(self, self.availablePorts[i], self.availableModuleTypes[i], self.availableModuleLocations[i], self.availableModuleLongNames[i], EVT_NEWSERIALDATA0_ID, None)
			temp.setSerialWorker(self.serialWorkers[i]) #bind serialworker and module
			self.serialWorkers[i].setModule(temp)

			# Create and set the system (programmer) worker
			self.programmerWorkers[i] = ProgrammerWorkerThread.ProgrammerWorkerThread(self, False, EVT_NEWPROGRAMMERDATAEVENT_ID, BASE_PATH)
			temp.setProgrammerWorker(self.programmerWorkers[i]) #bind serialworker and module
			self.programmerWorkers[i].setModule(temp)

		# Everything is connected, update our UI	
		#self.processGraphicManager.updateAll()
		pass

	def GetSerialDeviceIDAndAdd(self, port):
		#try:
		#self.statusBar.SetStatusText('Trying port ' + str(port)) 
		#s = serial.Serial(port[0], timeout=2)
		s = serial.Serial(port, timeout=2)
		
		#self.statusBar.SetStatusText('found something on' + port) 
		# maybe wait a hot second here for the bootloader to take a chill pill.
		s.write("I")
		
		identity = s.read(4)
		identity = identity.strip(' \t\n\r')
		
		if(len(identity) > 0):
			# We got something back
			
			# Search the module types DB for a matching ID string
			foundModuleType = self.farkusTable.getModuleTypeManager().getModuleTypeBySerialIDString(identity)
			
			if ( foundModuleType ):  # Did we find a standard module?
				self.availablePorts.append(port)
				self.availableModuleLongNames.append(foundModuleType.getName())
				self.availableModuleTypes.append(foundModuleType.getSerialIDString())
				self.availableModuleLocations.append(False)
				self.LogToGUI("Found " + foundModuleType.getName() + " at " + str(port))
				self.farkusTable.getModuleManager().add(
					FarkusModule.FarkusModule(foundModuleType.getSerialIDString(), self.farkusTable.getModuleTypeManager(), port, self) )
				self.foundDevices+=1
				#self.statusBar.SetStatusText('Searching for FARKUS-Compatible Devices - Found ' + str(foundDevices) + ' Devices.') 
			# TODO put this back
			#elif identity == self.farkusTable.getConveyance().getExpectedSerialIDString(): # Did we find a conveyance?
			#	availablePorts.append(port[0])
			#	availableModuleLongNames.append(self.farkusTable.getConveyance().getName())
			#	availableModuleTypes.append(self.farkusTable.getConveyance().getExpectedSerialIDString())
			#	availableModuleLocations.append(False)
			#	#self.LogToGUI("Found " + self.farkusTable.getConveyance().getName() + " at " + str(port[0]))
			#	self.farkusTable.getConveyance().setSerialPortIdentifier(port[0])
			#	foundDevices+=1
			#	self.statusBar.SetStatusText('Searching for FARKUS-Compatible Devices - Found ' + str(foundDevices) + ' Devices.') 
			else:
				self.LogToGUI("Unknown device found at " + str(port))
		else:
			self.LogToGUI("The device at " + str(port) +" failed to identify itself.")
		s.close()
		#except serial.SerialException:
		#	pass
	def OnCloseSerial(self, event):
		self.LogToGUI('Attempting to close Serial Connections')
		# Close existing connections
		for i in self.serialWorkers:
			try:
				if i.isAlive() and i.ser.isOpen():
					i.abort()
			except:
				pass
	
	def OnNewSerialData(self, event): # This handler is shared by all of the serialworkers.  Retrieve details on which from the `event` variable
		if event.module is not None:
			# we have somewhere to route this event
			if event.module.isConveyance is False:  # TODO: remove the isConveyance thing and check the object's type instead
				# it's a FarkusModule, pass it along
				event.module.onNewMessageFromSerial(event)
			else:
				# This is the FarkusConveyance
				pass
		else:
			#self.LogToGUI("Received a message from an orphaned SerialWorker")
			pass
		
		# These event handlers should really be ONLY system-level event actions (disconnection state, GUI mgmt, etc)
		if event.data is None:
			pass
		elif event.data == "$$$OPEN$$$":
			#self.LogToGUI("apple Connection Established: "+event.moduleLongName+" @ Location "+str(event.moduleLocation));
			pass
		elif event.data == "$$$CONNECTFAIL$$$":
			#self.LogToGUI("Connection to "+event.moduleLongName+ " @ Location "+str(event.moduleLocation)+ " was lost.");
			
			# Remove the module from the table
			
			# Check WTF we need to do now that guy is gone..
			
			# Update GUI
			#self.processGraphicManager.updateAll()
			pass
		elif event.data[:15] == "$$$COMMFAULT$$$":
			#self.LogToGUI("Communication failure @ Location "+str(event.moduleLocation)+ " "+event.moduleLongName);
			# Remove the module from the table
			
			# Check WTF we need to do now that guy is gone..
			
			# Update GUI
			#self.processGraphicManager.updateAll()
			pass
		else:
			#self.LogToGUI('Message from Location ' + str(1) + ': ' + event.data)
			pass
		
	def OnNewProgrammerEvent(self, event):
		if event.module is not None:
			pass
		else:
			self.LogToGUI("Received a message from an orphaned Programmer worker")
			pass
		
		if event.module is not None:
			# we have somewhere to route this event
			event.module.onNewMessageFromProgrammer(event)
		else:
			#self.LogToGUI("Received a message from an orphaned SerialWorker")
			pass
				
	def OnEditSettings(self, event):
		EditSettingsDialog(self, -1, 'Edit Settings')
	
	def OnQuitApp(self, event):
		try:
			self.serialWorker.abort()
		except Exception:
			pass
		self.Close(True)
		return
		
	def QuitApp(self, exitCode):
		try:
			# TODO: Abort all serialworkers
			self.serialWorker.abort()
		except Exception:
			pass
		self.Close(True)
		return

	def LogToGUI(self, message):
		if message.__len__() < 2:
			return
		self.logDisplay.SetValue(message + "\n" + self.logDisplay.GetValue())
		self.logDisplay.Update()
		
	def OnStartDumbMode(self, event):
		self.LogToGUI("Starting Dumb Mode")
		pass;

# A class to replace sys.stdout and sys.stderr to redirect those pipes to our logger
class RedirectSTDOUT_STDERR(object):
	def __init__(self,_parentWindow):
		self.parentWindow=_parentWindow
 
	def write(self,message):
		self.parentWindow.LogToGUI(message)

class MainApp(wx.App):
	def OnInit(self):
		self.frame = MainFrame(None, -1)
		self.frame.Show(True)
		self.SetTopWindow(self.frame)
		return True


# Ensure we're running in the __main__ thread, and that only one instance is running at a time
if __name__ == '__main__':
	#if os.path.exists("C:\modrobot\Bootloader\.LOCKFILE"):
	#	print 'There is already an instance of this application running! Closing... '
	#	sleep(2)
	#	sys.exit(-1)
	#else:
	#	open("C:\modrobot\Bootloader\.LOCKFILE", 'w').write("1")
	#try:
		app = MainApp(0)
		app.MainLoop()
	#finally:
		#os.remove("C:\modrobot\Bootloader\.LOCKFILE")
		#os._exit(0)
	
