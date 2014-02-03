import ProgrammerWorkerThread

class FarkusModule():
	import ProgrammerWorkerThread
	"Class to define the ACTUAL Testbeds"
	def __init__(self, moduleType, moduleTypeManager, serialPortIdentifier, gui):
		self.isReady=False
		self.tablePosition = None
		self.isConveyance = False
		self.moduleTypeManager = moduleTypeManager
		self.moduleType = self.moduleTypeManager.getModuleTypeBySerialIDString(moduleType)
		
		self.configState = None
		
		self.serialWorker = None
		self.programmerWorker = None
		
		self.serialPortIdentifier = serialPortIdentifier
		self.serialBuffer = ""
		self.lastCommand = ""
		self.isWaitingForSerialResponse = False
		self.expectingPassFail = False

		self.gui = gui
		self.isBusy = False
		
		self.serialNumber = False;
		# Setup Serial, connect, set isConnected via function
		
		# Wait for RDY, set is_ready, is_ready event
		pass
	
	def QuerySerialNumber(self):
	    self.ser.write("S")
	    self.serialNumber = self.ser.read(4)
	    return self.serialNumber;
	
	def GetSerialNumber(self):
	    if(self.serialNumber ):
			return self.serialNumber
	    else:
			try:
				self.QuerySerialNumber()
				return self.serialNumber
			except:
				return "Unknown";
	
    
	def onNewMessageFromSerial(self, event):
	    
		if self.moduleType.getTypeS() is "Standalone":
		    # Testbeds, and what not define their own event handlers in this version!
		    self.moduleType.serialEventHandler(event)
		    
		    # Current version of testbed comm spec doesn't /r/n terminate lines, but probably should. 
		else:
		    print "Got message from a module!"
		    # FARKUS Modules will eventually, but we'll need to refactor a but TODO
		    message = event.data
		    
		    if message is not None:
			    if self.isWaitingForSerialResponse is False:
				    # UNEXPECTED, put it onto the buffer
				    self.serialBuffer += message
				    
				    #are the last 2 characters on the buffer a newline pair?
				    if self.serialBuffer[-2:] == "\r\n":
					    self.serialBuffer = self.serialBuffer[:-2] #remove the newline
					    self.gui.LogToGUI("Unexpected From Module " + str(self.tablePosition) + ": " + self.serialBuffer)
					    self.lastCommand = self.serialBuffer
					    self.serialBuffer = ""
				    pass
			    else:
				    # Expected, put it onto the buffer
				    self.serialBuffer += message
				    
				    #are the last 2 characters on the buffer a newline pair?
				    if self.serialBuffer[-2:] == "\r\n":
					    self.serialBuffer = self.serialBuffer[:-2] #remove the newline
					    self.gui.LogToGUI("From Module " + str(self.tablePosition) + ": " + self.serialBuffer)
					    self.lastCommand = self.serialBuffer
					    self.serialBuffer = ""
				    #if so, we have a complete message, act on it.
				    
				    if(True):
					    if self.lastCommand == "PASS":
						    # Set test result to pass
						    # TODO: Math! But this works for now
						    index = self.tablePosition
						    if (index == 0): # Modules are on the odd part holders, and they are 1 indexed
							    moduleIndex = 1
						    elif (index == 2):
							    moduleIndex = 2
						    elif (index == 4):
							    moduleIndex = 3
						    elif (index == 6):
							    moduleIndex = 4
						    elif (index == 8):
							    moduleIndex = 5
						    elif (index == 10):
							    moduleIndex = 6
						    else:
							    moduleIndex = False
						    self.gui.LogToGUI("PASSED")
						    if(self.gui.farkusTable.getConveyance().attachedParts[moduleIndex].getStatus() is not "FAILED" ): #don't let failed cubes pass...workaround for not having testresult objects working yet
							    self.gui.farkusTable.getConveyance().attachedParts[moduleIndex].setStatus("PASSED")
						    self.gui.processGraphicManager.updateAll()
						    self.gui.isBusy = False
						    pass
					    elif self.serialBuffer == "FAIL":
						    # TODO: Math! But this works for now
						    index = self.tablePosition
						    if (index == 0): # Modules are on the odd part holders, and they are 1 indexed
							    moduleIndex = 1
						    elif (index == 2):
							    moduleIndex = 2
						    elif (index == 4):
							    moduleIndex = 3
						    elif (index == 6):
							    moduleIndex = 4
						    elif (index == 8):
							    moduleIndex = 5
						    elif (index == 10):
							    moduleIndex = 6
						    else:
							    moduleIndex = False
						    
						    self.gui.LogToGUI("FAIL")
						    self.gui.farkusTable.getConveyance().attachedParts[moduleIndex].setStatus("FAILED")
						    self.gui.processGraphicManager.updateAll()
						    self.gui.isBusy = False
    
					    #self.isWaitingForSerialResponse = False
		pass
	
	def isConnected(self):
		if( self.serialWorker.ser.isOpen() ):
			return True
		return False
	
	def connect(self ):
		# Attempt serial connection, wait for RDY, set is ready, is_ready event
		pass
	
	def remove(self ):
		try:
			# Estop module
			self.eStop()
		
			# Disconnect Serial
			self.serialWorker.abort()
		
			# Set self to false
			self = False
		except:
			pass;
		
	#  MODULE COMMANDS ARE HERE ##################
	#	Overall TODO: check for echo of command, boolean return
	#			TODO: constants for commands

	def sendCommandWithResponse(self, command, response ):
		self.isWaitingForSerialResponse = True
		self.nextExpectedResponse = str(response) # this is a dumb way to do this. It assumes that responses are 1-to-1, and limits the flexibility of these responses.  Likely to be revised soon.
										# If response === True, we're expecting a soft response to be processed outside of the normal event handler (anonymous function, perhaps?)
		self.serialWorker.write(command)
		# Maybe set a timer here to perform timeouts?

	
	def sendCommand(self, command):
		self.serialWorker.write(command) # this is more like a message, but whatever
		self.isWaitingForSerialResponse = False
		self.nextExpectedResponse = None # this is a dumb way to do this. It assumes that responses are 1-to-1, and limits the flexibility of these responses.  Likely to be revised soon.
		
	def go(self):
		#self.serialWorker.write("GO")
		self.sendCommandWithResponse("GO", "GO")
		self.expectingPassFail = True
		self.isWaitingForSerialResponse = True
		self.isBusy = True
		# Maybe set a timer here to perform timeouts?
		
		return True
	
	def eStop(self):
		self.serialWorker.write("GO")
		self.sendCommandWithResponse("ESTOP", "ESTOP")
		self.expectingPassFail = False
		self.isWaitingForSerialResponse = True
		# Maybe set a timer here to perform timeouts?
		
		return True
	
	def blinkForID(self):
		self.sendCommandWithResponse("BLINK", "BLINK")
		self.expectingPassFail = False
		self.isWaitingForSerialResponse = True
		# Maybe set a timer here to perform timeouts?
		return True
	
	def getActualModuleId(self):
		self.sendCommandWithResponse("I", True)  # [1]=True means soft response expected
		self.expectingPassFail = False
		self.isWaitingForSerialResponse = True
		# Maybe set a timer here to perform timeouts?
		return True
	
	def configModule(self, state):
		self.sendCommandWithResponse("C" + str(state), True)  # [1]=True means soft response expected
		self.expectingPassFail = False
		self.isWaitingForSerialResponse = True
		# Maybe set a timer here to perform timeouts?
		return True
	
	# #############################################
	
	# Housekeeping setters/getters below
	def getTablePosition(self):
		return self.tablePosition
		
	def setTablePosition(self, position):
		self.tablePosition = position
	
	def getName(self):
		if( self.moduleType is not None ):
			return self.moduleType.getName()
		else:
			return False
	
	def setShortName1(self, name):
		if( self.moduleType is not None ):
			self.moduleType.setShortName1(name)
			return True
		else:
			return False
	
	def getShortName1(self):
		if( self.moduleType is not None ):
			return self.moduleType.getShortName1()
		else:
			return False
	
	def setShortName2(self, name):
		if( self.moduleType is not None ):
			self.moduleType.setShortName2(name)
			return True
		else:
			return False
	
	def getShortName2(self):
		if( self.moduleType is not None ):
			return self.moduleType.getShortName2()
		else:
			return False
		
	def getConfigState(self):
		return self.configState

	def setConfigState(self, configState):
		if self.configModule(self.configState) is True:
			# Set the local cache
			self.configState = configState
		else:
			return False
		
	def isBusy(self):
		return True
	
	def getIsReady(self):
		return self.isReady
	
	def setSerialWorker(self, serialWorker):
		self.serialWorker = serialWorker
	
	def setProgrammerWorker(self, programmerWorker):
		self.programmerWorker = programmerWorker
	    
	def getProgrammerWorker(self):
		return self.programmerWorker
		
	def getExpectedSerialIDString(self):
		return self.moduleType.getSerialIDString()

	def getSerialPortIdentifier(self):
		return self.serialPortIdentifier
	
	
	
	
	
