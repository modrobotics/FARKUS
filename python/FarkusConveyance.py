import SerialWorker
import SerialResultEventHandler

import FarkusPart
import FarkusPartTypeManager
import FarkusPartType
from collections import deque

class FarkusConveyance():
	"Class to define the Conveyance on a FARKUS Table"
	def __init__(self, serialPortIdentifier, partTypeManager, gui):
        
		self.name = "***FARKUS-Conveyance"
		self.serialIDString = "`0000"
		self.partHolderCount = 10  # one less than actual so that pop()'s occur on the exit ramp
		self.partHolderPitch = None
		
		self.attachedParts = deque([]) # init deque with extra spot for shifting
		for i in range(0,self.partHolderCount+1):
			self.insertEmptyPartHolder()
		self.partsInProcess = 0;
		
		self.serialPortIdentifier = serialPortIdentifier
		self.serialWorker = None
		#self.isConnected = False
		self.isReady=False
		self.configState = None		
		self.partTypeManager = partTypeManager
		self.gui = gui
	
		# Setup Serial, connect, set isConnected via function
		
		# Wait for RDY, set is_ready, is_ready event
		pass
	
	def isConnected(self):
		try:
			if self.serialWorker.ser.isOpen() is True:
				return True
			else:
				return False
		except:
			return False
	def connect(self):
		# Attempt serial connection, wait for RDY, set is ready, is_ready event
		pass
	
	def remove(self ):
		# Estop module
		# Disconnect Serial
		# Set self to false
		pass
		
	# adds the part, shifts the parts in the attachedParts array, advances the conveyance
	def insertNewPart(self, partTypeId):
		self.attachedParts.appendleft(FarkusPart.FarkusPart( self.partTypeManager.getPartTypeById(partTypeId) ) )
		self.removePartOnExit()
		self.advanceForward()
		self.partsInProcess+=1 #increment attached part counter
		self.gui.processGraphicManager.updatePartInformation()
		self.gui.processGraphicManager.updateStatusBar()
		pass
		
	def getPartsInProcess(self):
		return self.partsInProcess;
	
	def insertEmptyPartHolder(self):
		try:
			self.attachedParts.appendleft(None)
			self.advanceForward()
			self.removePartOnExit()
			self.gui.processGraphicManager.updatePartInformation()
			self.gui.processGraphicManager.updateStatusBar()
		except Exception:
			pass
	
	def removePartOnExit(self):
		removedPart = self.attachedParts.pop()
		if removedPart is not None:
			# There's actually a part on this holder
			# TODO: Log this somewhere intelligent
			self.gui.LogToGUI("Part on Exit Module (6) with Serial Number # " + str(removedPart.getSerialNumber()) + " " + removedPart.getStatus())
			self.partsInProcess-=1 #decrement attached part counter
			self.gui.processGraphicManager.updateStatusBar()
		pass

	def getConnectedPartByPartHolder(self, holderIndex):
		try:
			foundPart = self.attachedParts[holderIndex]
		#if(foundPart is not None):
			return foundPart
		except:
			pass
		#else:
		#	return False
		
		
	def getConnectedParts(self):
		return self.attachedParts
	
	def go(self):
		#Command to serial, bool
		self.serialWorker.write("GO")
		
		#TODO: check for echo and bool return
		return True;
	
	def advanceForward(self):
		self.setConfigState(0)  # Go forward TODO: constants for commands
		self.go()
		
	def advanceBackward(self):
		self.setConfigState(1)  # Go backward TODO: constants for commands
		self.go()
	
	def eStop(self):
		#Command to serial, bool
		self.serialWorker.write("ESTOP")
		pass
	    
	def configModule(self, state):
		self.serialWorker.write("C" + str(self.configState))
		return True

	def blinkForID(self):
		#Command to serial, bool
		pass
	
	def getActualModuleId(self):
		#command to serial return
		self.serialWorker.write("I")
		pass
	
	def getName(self):
	    return self.name
	
	def setName(self, name):
	    self.name = name
	    
	def getConfigState(self):
	    return self.configState
	
	def setConfigState(self, configState):
		if(configState != self.configState):
			self.configState = configState
			self.configModule(configState)
			
	def getIsReady(self):
		return self.isReady
	
	def getExpectedSerialIDString(self):
		return self.serialIDString
	
	def setSerialWorker(self, serialWorker):
		self.serialWorker = serialWorker
	
	def getSerialPortIdentifier(self):
		return self.serialPortIdentifier

	def setSerialPortIdentifier(self, identifier):
		self.serialPortIdentifier = identifier
	