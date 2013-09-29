#!/usr/bin/python

from threading import *
import wx
from time import sleep
import subprocess
import serial
import sys
import os
import signal
import shelve
import time
from serial.tools import list_ports

import SerialWorker
import FarkusModuleType
import FarkusModuleTypeManager
import FarkusModule
import FarkusModuleManager
import FarkusConveyance
import FarkusConfigureModuleWindow
import FarkusGUIProcessGraphicManager
import FarkusTable

SETTINGS_DATA_FILE = "/home/pi/FARKUS/python" + "/Settings.dat"

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

# Menu Button Definitions
ID_OPTIONS_EDITSETTINGS = wx.NewId()
ID_OPTIONS_STARTDUMB = wx.NewId()
ID_EXIT = wx.NewId
ID_OPTIONS_OPENSERIAL = wx.NewId()
ID_OPTIONS_CLOSESERIAL = wx.NewId()
ID_OPTIONS_CAROUSEL = wx.NewId()

# Settings Window Button Definitions
ID_SETTINGS_CANCEL = wx.NewId()
ID_SETTINGS_SAVE = wx.NewId()

# Thread Communication Events
EVT_NEWSERIALDATA0_ID = wx.NewId()

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


#class SerialResultEvent0(wx.PyEvent):
#	def __init__(self, data, moduleType, moduleLocation, moduleLongName):
#		wx.PyEvent.__init__(self)
#		self.SetEventType(EVT_NEWSERIALDATA0_ID)
#		self.data = data
#		self.moduleLocation = moduleLocation
#		self.moduleType = moduleType
#		self.moduleLongName = moduleLongName



# GUI Frame class that spins off the worker thread
class MainFrame(wx.Frame):
    def onPause( self, event ):
	self.LogToGUI("PAUSE")

	
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
	self.LogToGUI("X: " + str(pos[0]) + " Y: " + str(pos[1]))
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
    
    def ConfigModule(self, event ):	
	# Configure new window, show, save, destroy
	chgdep = FarkusConfigureModuleWindow.FarkusConfigureModuleWindow(None, event.moduleLocation, self.farkusTable.getModuleManager(), self, title='Details: Module ' + str(event.moduleLocation) )
	chgdep.ShowModal()
	chgdep.Destroy()    
	
    
    def isInSquare(self, posX, posY, startX, startY, endX, endY ):  #start=top left, end=bottom right. 
	# is in X range
	if( posX >= startX and posX <=endX):
		#Y in range
		if( posY >= startY and posY <=endY):
			#Y in range
			return True
	return False
    
    def __init__(self, parent, id):
	wx.Frame.__init__(self, parent, id, 'Modular Robotics FARKUS Desk v0.1', size=(935,525), style= wx.SYSTEM_MENU | wx.CAPTION | wx.CLOSE_BOX)
	
	# Set window BG Color to match BG of logo image
	self.SetBackgroundColour((243,240,233))
	
	# Add the FARKUS Background
	backgroundPath = '/home/pi/FARKUS/inc/background.png'
	png = wx.Image(backgroundPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
	pngBitMap = wx.StaticBitmap(self, -1, png, (10, 5), (png.GetWidth(), png.GetHeight()))
	pngBitMap.Bind( wx.EVT_LEFT_DOWN, self.__OnLeftDown) 
	
	
	# Create menus...
        self.menubar = wx.MenuBar()
        self.fileMenu = wx.Menu()
        self.optionsMenu = wx.Menu()

        self.serialSubMenu = wx.Menu()
        self.openSerialItem = wx.MenuItem(self.serialSubMenu, ID_OPTIONS_OPENSERIAL, 'Discover and Connect', 'Open a connection to a FARKUS Array')
        #self.closeSerialItem = wx.MenuItem(self.serialSubMenu, ID_OPTIONS_CLOSESERIAL, '** Disconnected **', 'Disconnect from the FARKUS Array', kind=wx.ITEM_CHECK)

        self.serialSubMenu.AppendItem(self.openSerialItem)
       # self.serialSubMenu.AppendItem(self.closeSerialItem)
        
	self.optionsMenu.AppendMenu(ID_OPTIONS_CAROUSEL, 'Modules', self.serialSubMenu)

        self.editSettingsItem = wx.MenuItem(self.fileMenu, ID_OPTIONS_EDITSETTINGS, '&Edit Settings', "Modify Settings")
        self.startDumbMode = wx.MenuItem(self.fileMenu, ID_OPTIONS_STARTDUMB, '&Start Dumb Mode', "Get dumb.")

        self.optionsMenu.AppendItem(self.editSettingsItem)
        self.optionsMenu.AppendItem(self.startDumbMode)
		
        self.quitItem = wx.MenuItem(self.fileMenu, wx.ID_EXIT, '&Quit', 'Exit the Modular Robotics FARKUS Desk')
        self.fileMenu.AppendItem(self.quitItem)
        
        self.menubar.Append(self.fileMenu, '&File')
        self.menubar.Append(self.optionsMenu, '&Options')
        self.SetMenuBar(self.menubar)
	
	
	self.processGraphicManager = FarkusGUIProcessGraphicManager.FarkusGUIProcessGraphicManager(self, None)

	

	# Create E STOP Button!
	self.eStopButton = wx.Button(self, ID_ESTOP, "EMERGENCY STOP", pos=(721,10), size=(200,145))
	self.eStopButton.SetBackgroundColour('#CD0000')
	#self.eStopButton.Disable()
	
	# Create PAUSE Button!
	self.pauseButton = wx.Button(self, ID_PAUSE, "PAUSE", pos=(721, 165), size=(200,145))
	self.pauseButton.SetBackgroundColour('#FCD116')
	#self.pauseButton.Disable()
	
	# Create START Button!
	self.startButton = wx.Button(self, ID_START, "START", pos=(721, 320), size=(200,145))
	self.startButton.SetBackgroundColour('#009900')
	#self.startButton.Disable()
	
	# Add the Modrobotics Logo
	#logoPath = '/home/pi/FARKUS/inc/logo.jpg'
        #logoBitmap = wx.Image(logoPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        #wx.StaticBitmap(self, -1, logoBitmap, (80, 8), (logoBitmap.GetWidth(), logoBitmap.GetHeight()))
	
	
	# Create the logger output box
	wx.StaticText(self, -1, 'Messages (Newest at Top) ', pos=(10,360))
	self.logDisplay = wx.TextCtrl(self, id = -1, pos = (10, 375), size = (701, 100), style = wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_AUTO_URL)
	self.linesInLogBuffer = 0 # initialize a counter variable
	
        # Bind GUI events to their handlers
	self.Bind(wx.EVT_MENU, self.OnOpenSerial, self.openSerialItem)
        #self.Bind(wx.EVT_MENU, self.OnCloseSerial, self.closeSerialItem)
        self.Bind(wx.EVT_MENU, self.OnStartDumbMode, self.startDumbMode)
        self.Bind(wx.EVT_MENU, self.OnEditSettings, self.editSettingsItem)
        self.Bind(wx.EVT_MENU, self.OnQuitApp, self.quitItem)
        	
        
        self.startButton.Bind(wx.EVT_BUTTON, self.onStart)
        self.pauseButton.Bind(wx.EVT_BUTTON, self.onPause)
        self.eStopButton.Bind(wx.EVT_BUTTON, self.onEstop)

	# Variable to hold worker threads
        self.serialWorkers = []
        self.serialWorkers.append(None)
        self.serialWorkers.append(None)  # TODO: These should be created and destroyed as needed
        self.serialWorkers.append(None)
        self.serialWorkers.append(None)
        self.serialWorkers.append(None)
        self.serialWorkers.append(None)
        self.serialWorkers.append(None)

	# Declare Thread events for inter-thread communication
	EVT_NEWSERIALDATA0(self,self.OnNewSerialData)
	EVT_CONFIGMODULE(self,self.ConfigModule)

        # Create a status bar and set the default status
	self.statusBar = self.CreateStatusBar()
	self.statusBar.SetStatusText('Offline - 0 Modules Connected.') # Eventually we'll need to set this dynamically from settings
	
	# Setup the serial submenu
	self.serialSubMenu.Check(ID_OPTIONS_CLOSESERIAL, True)
	self.serialSubMenu.Enable(ID_OPTIONS_CLOSESERIAL, False)
	
	# Icon Me!
        iconFile = "/home/pi/FARKUS/inc/icon.ico"
        icon1 = wx.Icon(iconFile, wx.BITMAP_TYPE_ICO)
        self.SetIcon(icon1)
	
	# Show the application center in the user's screen
	self.Centre()
	
	
	# Initialize FARKUS Manager Singletons
	self.farkusTable = FarkusTable.FarkusTable(self);
	
	self.processGraphicManager.setModuleManager(self.farkusTable.getModuleManager())
	
		
	# Redirect STDOUT, STDERR to our logger now that we've rendered
        sys.stdout=RedirectSTDOUT_STDERR(self)
	sys.stderr=RedirectSTDOUT_STDERR(self)
	
    def OnOpenSerial(self, event):
	
	# This function will remove all current devices and reconnect.
	# TODO: allows discovery/connection of NEW modules only.  Maybe that could run period 
	self.LogToGUI("Discovering Devices...")
			# Close existing connections
	for i in self.serialWorkers:
		try:
			if i.isAlive() and i.ser.isOpen():
				i.ser.close()
				i.abort()
		except:
			pass
		
	# Remove all of the modules currently "on the table"
	self.farkusTable.getModuleManager().removeAll()
	self.processGraphicManager.updateAll()

	# Windows
	#if os.name == 'nt':

	availablePorts = []
	availableModuleTypes = []
	availableModuleLocations = []
	availableModuleLongNames = []
	
	# Close existing connections
	for i in self.serialWorkers:
		try:
			if i.isAlive() and i.ser.isOpen():
				i.ser.close()
				i.abort()
		except:
			pass
	
	
	for port in list_ports.comports():  #unix/mac
	#for i in range(256):  #windows
		try:
			s = serial.Serial(port[0], timeout=2)
			# maybe wait a hot second here for the bootloader to take a chill pill.
			s.write("I")
			
			
			identity = s.read(5)
			
			if(len(identity) > 0):
				# We got something back
				
				# Search the module types DB for a matching ID string
				foundModuleType = self.farkusTable.getModuleTypeManager().getModuleTypeBySerialIDString(identity)
				
									
				if ( foundModuleType ):  # Did we find a standard module?
					availablePorts.append(port[0])
					availableModuleLongNames.append(foundModuleType.getName())
					availableModuleTypes.append(foundModuleType.getSerialIDString())
					availableModuleLocations.append(False)
					self.LogToGUI("Found " + foundModuleType.getName() + " at " + str(port[0]))
					self.farkusTable.getModuleManager().add(FarkusModule.FarkusModule(foundModuleType.getSerialIDString(), self.farkusTable.getModuleTypeManager(), port[0]) )
				elif identity == self.farkusTable.getConveyance().getExpectedSerialIDString(): # Did we find a conveyance?
					availablePorts.append(port[0])
					availableModuleLongNames.append(self.farkusTable.getConveyance().getName())
					availableModuleTypes.append(self.farkusTable.getConveyance().getExpectedSerialIDString())
					availableModuleLocations.append(False)
					self.LogToGUI("Found " + self.farkusTable.getConveyance().getName() + " at " + str(port[0]))
					self.farkusTable.getConveyance().setSerialPortIdentifier(port[0])
				else:
					self.LogToGUI("Unknown device found at " + str(port[0]))
			else:
				self.LogToGUI("The device at " + str(port[0]) +" failed to identify itself.")
			s.close()
		except serial.SerialException:
			pass
	
	if len(availablePorts) > 0:
		#self.LogToGUI('Attempting to connect to modules....')
		pass
	else:
		self.LogToGUI('No FARKUS-compatible modules were found.')
		
	# Close existing connections
	for i in self.serialWorkers:
		try:
			if i.isAlive() and i.ser.isOpen():
				i.ser.close()
				i.abort()
		except:
			pass
		
	# Open connections to the modules we found
	for i in range(len(availablePorts)):
		temp = None
		if(availableModuleTypes[i] == self.farkusTable.getConveyance().getExpectedSerialIDString()):
			#Conveyance is special
			self.serialWorkers[i] = SerialWorker.SerialWorkerThread0(self, availablePorts[i], availableModuleTypes[i], availableModuleLocations[i], availableModuleLongNames[i], EVT_NEWSERIALDATA0_ID)
			self.farkusTable.getConveyance().setSerialWorker(self.serialWorkers[i]) # TODO: I tried moving this into the Conveyance/module object, but had trouble with event handling.  No time now.
			
		else:
			self.serialWorkers[i] = SerialWorker.SerialWorkerThread0(self, availablePorts[i], availableModuleTypes[i], availableModuleLocations[i], availableModuleLongNames[i], EVT_NEWSERIALDATA0_ID)
			temp = self.farkusTable.getModuleManager().getModuleBySerialPort(availablePorts[i])
			temp.setSerialWorker(self.serialWorkers[i])
				
				
    def OnCloseSerial(self, event):
        if self.serialWorker and self.serialWorker.isAlive() and self.serialWorker.ser.isOpen():
            self.LogToGUI('Attempting to close Serial Connections')
            # Close existing connections
            for i in self.serialWorkers:
		try:
			if i.isAlive() and i.ser.isOpen():
				i.abort()
		except:
			pass
        else:
	    pass
	
    def OnNewSerialData(self, event): # This handler is shared by all of the serialworkers.  Retrieve details on which from the `event` variable
		#print(event)
		if event.data is None:
			pass
		elif event.data == "$$$OPEN$$$":
			self.LogToGUI("Connection Established: "+event.moduleLongName+" @ Location "+str(event.moduleLocation));
			pass
		elif event.data == "$$$CONNECTFAIL$$$":
			self.LogToGUI("Connection to "+event.moduleLongName+ " @ Location "+str(event.moduleLocation)+ " was lost.");
			pass
		elif event.data[:15] == "$$$COMMFAULT$$$":
			self.LogToGUI("Communication failure @ Location "+str(event.moduleLocation)+ " "+event.moduleLongName);
			pass
		else:
			#self.LogToGUI('Last Message: %s' % event.data)
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
        while 1==1:
		
            #self.serialWorkers[0].write("A");
	    self.module1CubeID.SetLabel("13491")
            #time.sleep(1)
            #self.serialWorkers[1].write("A");
            time.sleep(1)
	    self.module1CubeID.SetLabel("---------")
	    self.module2CubeID.SetLabel("13491")

		
	    break;
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
	#    print 'There is already an instance of this application running! Closing... '
	 #   sleep(2)
	  #  sys.exit(-1)
	#else:
	 #   //open("C:\modrobot\Bootloader\.LOCKFILE", 'w').write("1")
	    try:
                app = MainApp(0)
                app.MainLoop()
	    finally:
		#os.remove("C:\modrobot\Bootloader\.LOCKFILE")
		os._exit(0)
    
