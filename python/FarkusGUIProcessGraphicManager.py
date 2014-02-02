import wx
from collections import deque


class FarkusGUIProcessGraphicManager():
	"Class to define the some helper functions to manage the process graphic"
	def __init__(self, gui, moduleManager, BASE_PATH ):
		self.gui = gui
		self.moduleManager = moduleManager

		self.gui.moduleCubeID = [None]*10
		self.gui.moduleName1 = [None]*10
		self.gui.moduleName2 = [None]*10
		self.gui.moduleConfigureText = [None]*10
		self.partHolderIndicators = []
	
		#self.partHolderIndicators = deque([]) # init deque with extra spot for shifting
		#for i in range(0,self.gui.farkusTable.getConveyance().getPartHolderCount()+1):
		#	self.insertEmptyPartHolder()
			   
		self.failedPartPath = BASE_PATH + '/inc/failedPart.png'
		self.testingPartPath = BASE_PATH + '/inc/testingPart.png'
		self.passedPartPath = BASE_PATH + '/inc/passedPart.png'	   #Eventually real part images could go here
		self.emptyPartPath = BASE_PATH + '/inc/emptyPart.png'
		self.unknownPartPath = BASE_PATH + '/inc/unknownPart.png'

		
		self.failedPartBitmap = wx.Image(self.failedPartPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		self.testingPartBitmap = wx.Image(self.testingPartPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		self.passedPartBitmap = wx.Image(self.passedPartPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		self.emptyPartBitmap = wx.Image(self.emptyPartPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		self.unknownPartBitmap = wx.Image(self.unknownPartPath, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		
		# Configure which icon the part holders get during init
		self.defaultPartBitmap = self.emptyPartBitmap
			
		# Setup!
		self.InitGUI()
	
	def setModuleManager(self, manager):
		self.moduleManager = manager
	
	# this is a graphical primitive...needs a managing function to do shifting:::: use deque
	def markPartHolderStatus(self, holderIndex, status): # TODO: ENUMs here
		try:
			if(status == "TESTING"):			
				# HELL YES this works.  The wx bitmap library is a little strange (can't simply assign bitmaps to an array?).
				# There's an hour of my life I'll never get back
				self.partHolderIndicators[holderIndex].SetBitmap(self.testingPartBitmap)
			elif ( status == "FAILED" ):
				self.partHolderIndicators[holderIndex].SetBitmap(self.failedPartBitmap)
			elif ( status == "PASSED" ):
				self.partHolderIndicators[holderIndex].SetBitmap(self.passedPartBitmap)
			elif ( status == "EMPTY" ):
				self.partHolderIndicators[holderIndex].SetBitmap(self.emptyPartBitmap)
			elif ( status == "UNKNOWN" ):
				self.partHolderIndicators[holderIndex].SetBitmap(self.unknownPartBitmap)
			pass
		except:
			pass
	def InitGUI(self):
		# Create the images to mark part holders
		# cells in image aren't spaced on the python grid...wing it!
		offset = 0
		for i in range(0,11):
			part = wx.StaticBitmap(self.gui, -1, self.defaultPartBitmap, ((119+(i*51)+offset), 296), (self.defaultPartBitmap.GetWidth(), self.defaultPartBitmap.GetHeight())) #+51
			self.partHolderIndicators.append(part)
			if( i%2 == 1 ):
				offset +=1
			
			
			# Create a status bar and set the default status
			self.gui.statusBar = self.gui.CreateStatusBar()
			self.gui.statusBar.SetStatusText('Offline')
			
			# Create textfields for Cubelet IDs
			self.gui.moduleCubeID[1] = wx.StaticText(self.gui, -1, '---------', pos=(105,248))
			self.gui.moduleCubeID[2] = wx.StaticText(self.gui, -1, '---------', pos=(208,248))
			self.gui.moduleCubeID[3] = wx.StaticText(self.gui, -1, '---------', pos=(313,248))
			self.gui.moduleCubeID[4] = wx.StaticText(self.gui, -1, '---------', pos=(418,248))
			self.gui.moduleCubeID[5] = wx.StaticText(self.gui, -1, '---------', pos=(518,248))
			self.gui.moduleCubeID[6] = wx.StaticText(self.gui, -1, '---------', pos=(623,248))
			
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
		self.updatePartInformation()
		self.updateStatusBar()
		self.updateMainControlButtons()
		pass
	
	def updateStatusBar(self):
		if(len(self.moduleManager.getConnectedModules()) > 0):
			if self.gui.farkusTable.getConveyance().isConnected() is True:
				conveyance = "Conveyance Connected - "
			else:
				conveyance = "Conveyance DISCONNECTED - "
			status = "Online - " + conveyance + str(len(self.gui.farkusTable.getModuleManager().getConnectedModules())) + " Modules Connected - " + str(self.gui.farkusTable.getConveyance().getPartsInProcess()) + " parts in process."
		else:
			status = "Offline" 
		self.gui.SetStatusText(status) # Eventually we'll need to set this dynamically from settings
	
	
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
				
			# Force the GUI to update right now...
			self.gui.Update()
	
	def updateMainControlButtons(self):
		if self.gui.farkusTable.getConveyance().isConnected():  #hacky...shouldn't be accessing the conveyance like this. TODO: self.getFarkusTable()
			# We're connected to the conveyance, allow add part and Advance buttons
			self.gui.advanceButton.Enable()
			self.gui.addPartButton.Enable()
			pass
		else:
			# disable buttons
			self.gui.advanceButton.Disable()
			self.gui.addPartButton.Disable()
			pass
	
	def updatePartInformation(self):
		try:
			for index in range(0,11):
				part = self.gui.farkusTable.getConveyance().getConnectedParts()[index]
				
				# TODO: Math! But this works for now
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
				   
				if (part is None):
					# No part here. Mark it.
					self.markPartHolderStatus(index, "EMPTY")
					
					# update the ID field if we're dealing with a part holder currently in a module
					if(moduleIndex):
						self.gui.moduleCubeID[moduleIndex].SetLabel('---------') #cubeIDfields are 1-indexed
				else:
					#we have a part, so it will have a status
					self.markPartHolderStatus(index, part.getStatus())
					
					# if were in a module, update the Serial Number field
					if(moduleIndex):
						# update the ID field
						if(part.getSerialNumber() is not None):
							self.gui.moduleCubeID[moduleIndex].SetLabel(str(part.getSerialNumber())) #cubeIDfields are 1-indexed
						else:
							self.gui.moduleCubeID[moduleIndex].SetLabel('Unknown') #cubeIDfields are 1-indexed
								
						# This doesn't belong here at all. THis is a GUI manager, and I'm about to write code to tell modules what to do here.
						# Please forgive me.  I'll clean it up later
						if moduleIndex:
							# We're in a module.  There's a part in the module.
							# Eventually we'll query this FarkusPartType's required tests, this FarkusModule's supportedTests,
							# Check this against this FarkusPart's testResults and, if all of these things suggest
							# this module can perform a test that this PartType requires and hasn't yet been
							# completed, then run the test.  For now....we're going to throw our hands in the air and run all of the tests
							tempModule = self.moduleManager.getModuleByTablePosition(moduleIndex)
							if tempModule:# and (tempModule.isBusy() == False):  # TODO: this isBusy check isn't required. updatePartInfo()
																				# is getting called 2x now, but making a ModuleManager or
																				# TestManager method handle this should alleviate the need
																				# for this code here.
								# Got handle on Module successfully
								if(tempModule.isBusy is True):
									pass
								else:
									self.gui.LogToGUI("Instructing Module @ Position " + str(moduleIndex) + " to GO")
									tempModule.go()
							else:
								#self.gui.LogToGUI("We have a part in Module Position " + str(moduleIndex) + " with no Module Present")
								pass
				# increment the index regardless
				index+=1
				
				# Force the GUI to update right now...
				self.gui.Update()
		except Exception:
			pass
		
				