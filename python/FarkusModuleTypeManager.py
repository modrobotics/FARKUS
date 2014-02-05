import FarkusModuleType
import random
from time import sleep

class FarkusModuleTypeManager():
	"Class to define helper functions for FarkusModuleTypes"
	
	#  THIS SHOULD GET MOVED OUT INTO ANOTHER FILE  / CLASS
	def programmerEventHandlerStandardTestbed(self, event):
		print "HERE"
		if event.returnCode == 0:
			self.LogToGUI('Programming SUCCESS')
			# Mark ID as used.
			if(event.useID):
			    #CIMSClientManager.onBootloadSuccess(self)
			    pass
			# If we're in automated mode, send the PASS command
			if self.isAutoMode:
				try:
				#self.serialWorker.write("P")
					pass
				except Exception:
					self.LogToGUI("Failed to send command P to Carousel")
			else:
			    pass
			
		elif event.returnCode == 1:
		    # Failed to compile
		    self.LogToGUI('Programming FAILED: Failed to Compile (E#%s)' % event.returnCode)
		elif event.returnCode == 2:
		    # Failed to burn fuses (AVR only)
		    self.LogToGUI('Programming FAILED: Failed to Burn Fuses (E#%s)' % event.returnCode)
		elif event.returnCode == 3:
		    # Failed to flash
		    self.LogToGUI('Programming FAILED: Failed to Flash (E#%s)' % event.returnCode)
		elif event.returnCode == 4:
		    # ID Database Fault
		    self.LogToGUI('Programming FAILED: ID Database Fault (E#%s)' % event.returnCode)
		elif event.returnCode == 5:
		    # Failed to update Emergency ID Datastore (EIDDS?)
		    self.LogToGUI('Programming FAILED: Failed to update EIDDS (E#%s)' % event.returnCode)
		elif event.returnCode == 6:
		    # Unknown Failure
		    self.LogToGUI('Programming FAILED: An Unknown Failure Occurred (E#%s)' % event.returnCode)
		elif event.returnCode == 7:
		    # Unknown Failure
		    print "Invalid Programmer Action Module"
		    #self.LogToGUI('Programming FAILED: Invalid Programmer Action Module Specified !!!(E#%s)' % event.returnCode)
		elif event.returnCode == 8:
		    # Unknown Failure
		    self.LogToGUI('Programming FAILED: No Programmer Action Module (PAM) has been selected (E#%s)' % event.returnCode)
		# Close the CIMS thread
		#wx.PostEvent(self, CIMSResultEvent("$$$CIMSDONE$$$"))
		    
		
		# General Failure handler
		if (event.returnCode > 0):
			event.module.serialWorker.write("F")
			
	#  THIS SHOULD GET MOVED OUT INTO ANOTHER FILE  / CLASS
	def serialEventHandlerStandardTestbed(self, event):
		# TODO: Determine system/module actions split
		try:
			if event.data == "$$$OPEN$$$":
				self.LogToGUI("Connection Established: "+event.moduleLongName+" @ Location "+str(event.moduleLocation));
				
				pass
			elif event.data == "$$$CONNECTFAIL$$$":
				self.LogToGUI("Connection to "+event.moduleLongName+ " @ Location "+str(event.moduleLocation)+ " was lost.");
				
				# Remove the module from the table?  Or at least mark it inactive
				
				# Update GUI
				self.processGraphicManager.updateAll()
				pass
			elif event.data[:15] == "$$$COMMFAULT$$$":
				print "Connection to " + event.moduleLongName + "(Serial#" + event.module.GetSerialNumber() + ") HAS BEEN LOST11111"
			
			#****************************
			# Commands below  ****************
			#***************************************
			
			elif event.data == "G":
				print "Programmer Starting..."
				event.module.getProgrammerWorker().program(True)
			
			'''
				if( random.choice([True, False]) ):
					# Pass
					print "Programming Successful"
					event.module.serialWorker.write("P");
				else:
					# Failure
					print "Programming Failed!"
					event.module.serialWorker.write("F");
			'''	
				
		except:
			print "Connection to " + event.moduleLongName + "(Serial#" + event.module.GetSerialNumber() + ") HAS BEEN LOST2222"

		#pass
	

##################################
##################################



	def __init__(self):
		self.moduleTypes = []
		
		# TODO: Load these from a shelf.  For now we'll statically define them
	# CUBELETS FARKUS Modules
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823887,  "`0001", "***Cubelets Flashlight/Brightness Test Module", "Flash/Bright", "Test Module", "Module", False, False, False))
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823888,  "`0003", "***Cubelets Communication Test Module", "Cube Comm", "Test Module", "Module", False, False, False))
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823889,  "`0007", "***Cubelets Flashing Shield", "Cubelets", "Flasher", "Module", False, False, False))
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823890,
									  "`0002",
									  "***Cubelets Power Test Module",
									  "Power Test",
									  "Module",
									  "Module",
									  False, False, False)
					)
	
		
		
		
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(5444444,
									  "1001",
									  "MOSS - Distance",
									  "Distance prgm/test",
									  "Testbed",
									  "Standalone",
									  self.serialEventHandlerStandardTestbed,
									  self.programmerEventHandlerStandardTestbed,
									  "/src/PAMs/programmer_pic.bat"))
		
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(5444445,
									  "0123",
									  "MOSS - Flashlight",
									  "Flashlight prgm/test",
									  "Testbed",
									  "Standalone",
									  self.serialEventHandlerStandardTestbed,
									  self.programmerEventHandlerStandardTestbed,
									  "src/hex/MOSS-Flashlight-v1.0.0.hex"))
		
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(5444446,
									  "1002",
									  "MOSS - BT Main",
									  "Bluetooth Main prgm/test",
									  "Testbed",
									  "Standalone",
									  self.serialEventHandlerStandardTestbed,
									  self.programmerEventHandlerStandardTestbed,
									  "src/hex/MOSS-BTMain-v1.0.0.hex"))

	def getModuleByLocation(self, location):
		pass
	
	def add(self, module):
		pass
		
	def remove(self, module):
		pass
		
	def removeByLocation(self, location):
		pass
		
	def removeAll(self):
		pass
	
	# returns the moduleType object if found, False if not.
	def getModuleTypeBySerialIDString(self, identifier):
		for moduleType in self.moduleTypes:
			if( moduleType.getSerialIDString() == identifier):
				return moduleType
		return False;
		
   
