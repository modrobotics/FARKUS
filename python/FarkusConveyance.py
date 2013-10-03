import SerialWorker
import SerialResultEventHandler

import FarkusPart
import FarkusPartType
import FarkusPartTypeManager

from collections import deque
import time

class FarkusConveyance():
	"Class to define the Conveyance on a FARKUS Table"
	def __init__(self, serialPortIdentifier, partTypeManager, gui):
        
		self.name = "***FARKUS-Conveyance"
		self.serialIDString = "`0000"
		self.partHolderCount = 10  # one less than actual so that pop()'s occur on the exit ramp
		self.partHolderPitch = None
		self.allowPartsInIntermediatePositions = False  # Force conveyance to move 2 increments per part
		self.isConveyance = True  #FarkusConveyance should get wrapped into an extension of FarkusModule eventually, lets start that now
		
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
		if self.allowPartsInIntermediatePositions is False:
			self.skipIntermediatePartHolder()
		
		self.attachedParts.appendleft(FarkusPart.FarkusPart( self.partTypeManager.getPartTypeById(partTypeId) ) )
		self.removePartOnExit()
		self.advanceForward()
		self.partsInProcess+=1 #increment attached part counter
		self.gui.processGraphicManager.updatePartInformation()
		self.gui.processGraphicManager.updateStatusBar()
		
		# Wait here for a second to allow the conveyance to actually move before telling modules to go
		# TODO: Query for completion command.  I thought about putting this in self.advanceForward()
		# but I want the GUI to update quickly
		time.sleep(1)
		return True
	
		pass
		
	def getPartsInProcess(self):
		return self.partsInProcess;
	
	def skipIntermediatePartHolder(self):
		self.attachedParts.appendleft(None)
		self.advanceForward()
		self.removePartOnExit()
		self.gui.processGraphicManager.updatePartInformation()
	
	def insertEmptyPartHolder(self):
		try:
			if self.allowPartsInIntermediatePositions is False:
				self.skipIntermediatePartHolder()
			
			self.attachedParts.appendleft(None)
			self.advanceForward()
			self.removePartOnExit()
			self.gui.processGraphicManager.updatePartInformation()
			self.gui.processGraphicManager.updateStatusBar()
			
			# Wait here for a second to allow the conveyance to actually move before telling modules to go
			# TODO: Query for completion command. I thought about putting this in self.advanceForward()
			# but I want the GUI to update quickly
			time.sleep(1)
			
			return True
		
		except Exception:
			pass
	
	def removePartOnExit(self):
		removedPart = self.attachedParts.pop()
		if removedPart is not None:
			# There's actually a part on this holder
			# TODO: Log this somewhere intelligent
			if removedPart.getStatus() == "PASSED":  #TODO: use EMUMS for status'
				status = " passed all tests."
			elif removedPart.getStatus() == "FAILURE":
				status = " FAILED one or more tests."
			else:
				status = " finished with indeterminate status."
					
			self.gui.LogToGUI("Part on Exit Ramp (6+) with Serial Number # " + str(removedPart.getSerialNumber()) + status)
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
		self.setConfigState(1)  # Go forward TODO: constants for commands
		time.sleep(0.1)
		self.go()
		
	def advanceBackward(self):
		self.setConfigState(0)  # Go backward TODO: constants for commands
		time.sleep(0.1)
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
	
