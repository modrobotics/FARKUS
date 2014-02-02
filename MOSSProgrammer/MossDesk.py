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

#import SerialWorker
#import FarkusModuleType
#import FarkusModuleTypeManager
#import FarkusModule
#import FarkusModuleManager
#import FarkusConveyance
#import FarkusConfigureModuleWindow
#import FarkusGUIProcessGraphicManager

#import FarkusTestbedManager

BASE_PATH = "C:\Users\jmoyes\Desktop\ModRobotics\GitHub\FARKUS\MOSSProgrammer"
#BASE_PATH = "/home/pi/FARKUS/python"
SETTINGS_DATA_FILE = BASE_PATH + "\Settings.dat"

# Menu Button Definitions
ID_OPTIONS_EDITSETTINGS = wx.NewId()
ID_OPTIONS_STARTDUMB = wx.NewId()
ID_EXIT = wx.NewId
ID_OPTIONS_OPENSERIAL = wx.NewId()
ID_OPTIONS_CLOSESERIAL = wx.NewId()
ID_OPTIONS_CAROUSEL = wx.NewId()
ID_OPTIONS_CONNECT_TO_TESTBED = wx.NewId()
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

# GUI Frame class that spins off the worker thread
class MainFrame(wx.Frame):
	def onPause( self, event ):
	
		self.LogToGUI("System entering paused state")
		self.farkusTable.pause()

		return True
	
	def __init__(self, parent, id):
		wx.Frame.__init__(self, parent, id, 'Modular Robotics FARKUS Desk v0.1', size=(935,535), style= wx.SYSTEM_MENU | wx.CAPTION | wx.CLOSE_BOX)
		
		
		# Initialize FARKUS Manager Singletons
		#self.testbedManager = FarkusTestbedManager(self);
		
		# Set window BG Color to match BG of logo image
		self.SetBackgroundColour((203,226,244))
		
		# Create menus...
		self.menubar = wx.MenuBar()
		self.fileMenu = wx.Menu()
		self.optionsMenu = wx.Menu()
		self.connectMenu = wx.Menu()
	
		self.connectToTestbedSubMenu = wx.Menu()
		self.optionsMenu.AppendMenu(ID_OPTIONS_CONNECT_TO_TESTBED, 'Connect to Testbed', self.connectToTestbedSubMenu)
		self.disconnectAll = wx.MenuItem(self.optionsMenu, ID_OPTIONS_CLOSESERIAL, 'Disconnect All', 'Disconnect from the FARKUS Array')
		self.optionsMenu.AppendItem(self.disconnectAll)

		self.connectToTestbedCubeletsSubMenu = wx.Menu()
		self.connectToTestbedSubMenu.AppendMenu(ID_OPTIONS_CONNECT_TO_TESTBED_CUBELETS, 'Cubelets', self.connectToTestbedCubeletsSubMenu)

		self.connectToTestbedMossSubMenu = wx.Menu()
		self.connectToTestbedSubMenu.AppendMenu(ID_OPTIONS_CONNECT_TO_TESTBED_MOSS, 'MOSS', self.connectToTestbedMossSubMenu)
		
		self.moduleMossAnglePot = wx.MenuItem(self.connectToTestbedMossSubMenu, ID_OPTIONS_SELECT_MODULE_ANGLEPOT, 'MOSS-Angle  (Angle-Pot)', '')
		self.connectToTestbedMossSubMenu.AppendItem(self.moduleMossAnglePot)
		
		self.moduleMossBTMain = wx.MenuItem(self.connectToTestbedMossSubMenu, ID_OPTIONS_SELECT_MODULE_BTMAIN, 'MOSS-Bluetooth-Main  (Bluetooth-Main)', '')
		self.connectToTestbedMossSubMenu.AppendItem(self.moduleMossBTMain)

		self.moduleMossDistance = wx.MenuItem(self.connectToTestbedMossSubMenu, ID_OPTIONS_SELECT_MODULE_DISTANCE, 'MOSS-Distance  (Distance)', '')
		self.connectToTestbedMossSubMenu.AppendItem(self.moduleMossDistance)
		
		self.moduleMossFlashlight = wx.MenuItem(self.connectToTestbedMossSubMenu, ID_OPTIONS_SELECT_MODULE_FLASHLIGHT, 'MOSS-Flashlight  (Flashlight)', '')
		self.connectToTestbedMossSubMenu.AppendItem(self.moduleMossFlashlight)
		
		self.moduleMossMicrophone = wx.MenuItem(self.connectToTestbedMossSubMenu, ID_OPTIONS_SELECT_MODULE_MICROPHONE, 'MOSS-Microphone  (Microphone)', '')
		self.connectToTestbedMossSubMenu.AppendItem(self.moduleMossMicrophone)
		
		self.moduleMossSpinMain = wx.MenuItem(self.connectToTestbedMossSubMenu, ID_OPTIONS_SELECT_MODULE_SPINMAIN, 'MOSS-Spin  (Spin-Main)', '')
		self.connectToTestbedMossSubMenu.AppendItem(self.moduleMossSpinMain)
		
		# Create the logger output box
		wx.StaticText(self, -1, 'Messages (Newest at Top) ', pos=(10,360))
		self.logDisplay = wx.TextCtrl(self, id = -1, pos = (10, 375), size = (701, 100), style = wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_AUTO_URL)
		self.linesInLogBuffer = 0 # initialize a counter variable
		
		# Bind GUI events to their handlers
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.moduleMossAnglePot)
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.moduleMossBTMain)
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.moduleMossDistance)
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.moduleMossFlashlight)
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.moduleMossMicrophone)
		self.Bind(wx.EVT_MENU, self.OnSelectModule, self.moduleMossSpinMain)
		
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

		# Show the application center in the user's screen
		self.Centre()
		
		# Redirect STDOUT, STDERR to our logger now that we've rendered
		sys.stdout=RedirectSTDOUT_STDERR(self)
		sys.stderr=RedirectSTDOUT_STDERR(self)
		
	def OnSelectModule(self, event):
		self.LogToGUI("Changing Module")
		
		eventSource = event.GetId()
		
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
		else:
			self.LogToGUI("Unknown Module")
				
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
			
		# Remove all of the modules currently "on the table"
		#self.farkusTable.getModuleManager().removeAll()
		#self.processGraphicManager.updateAll()

		#self.statusBar.SetStatusText('Searching for FARKUS-Compatible Modules') 
		self.LogToGUI("Searching for FARKUS-Compatible Modules")
		
		# Windows
		#if os.name == 'nt':

		availablePorts = []
		availableModuleTypes = []
		availableModuleLocations = []
		availableModuleLongNames = []
		
		# Close existing connections
		self.LogToGUI("Closing existing connections")
		for i in self.serialWorkers:
			try:
				if i.isAlive() and i.ser.isOpen():
					i.ser.close()
					i.abort()
			except:
				pass
		
		foundDevices = 0
		
		#for port in list_ports.comports():  #unix/mac
		for portname in enumerate_serial_ports():
			try:
				print "Trying to Connect to: " + portname
				ser = serial.Serial(portname, 9600, timeout=0.5)
				print "Established"
				ser.write("I")
				print "sent"
				s = ser.read(4)
				print s
				ser.close()
			except:
				print "Failed"
				continue;
			
		
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
			pass

		# Everything is connected, update our UI	
		#self.processGraphicManager.updateAll()
		pass

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
		pass
		if event.module is not None:
			# we have somewhere to route this event
			if event.module.isConveyance is False:  # TODO: remove the isConveyance thing and check the object's type instead
				# it's a FarkusModule, pass it along
				event.module.onNewMessageFromSerial(event.data)
			else:
				# This is the FarkusConveyance
				pass
		else:
			self.LogToGUI("Received a message from an orphaned SerialWorker")
		
		if event.data is None:
			pass
		elif event.data == "$$$OPEN$$$":
			self.LogToGUI("Connection Established: "+event.moduleLongName+" @ Location "+str(event.moduleLocation));
			pass
		elif event.data == "$$$CONNECTFAIL$$$":
			self.LogToGUI("Connection to "+event.moduleLongName+ " @ Location "+str(event.moduleLocation)+ " was lost.");
			
			# Remove the module from the table
			
			# Check WTF we need to do now that guy is gone..
			
			# Update GUI
			self.processGraphicManager.updateAll()
			pass
		elif event.data[:15] == "$$$COMMFAULT$$$":
			self.LogToGUI("Communication failure @ Location "+str(event.moduleLocation)+ " "+event.moduleLongName);
			# Remove the module from the table
			
			# Check WTF we need to do now that guy is gone..
			
			# Update GUI
			self.processGraphicManager.updateAll()
			pass
		else:
			#self.LogToGUI('Message from Location ' + str(1) + ': ' + event.data)
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
	try:
		app = MainApp(0)
		app.MainLoop()
	finally:
		#os.remove("C:\modrobot\Bootloader\.LOCKFILE")
		os._exit(0)
	
