import FarkusModuleType
import random
from time import sleep

class FarkusModuleTypeManager():
	"Class to define helper functions for FarkusModuleTypes"
	
	
	#  THIS SHOULD GET MOVED OUT INTO ANOTHER FILE  / CLASS
	def serialEventHandlerStandardTestbed(self, event):
		# TODO: Determine system/module actions split
		
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
			print "Connection to " + event.moduleLongName + "(Serial#" + event.module.GetSerialNumber() + ") HAS BEEN LOST"
		
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
			
		pass
	

##################################
##################################



	def __init__(self):
		self.moduleTypes = []
		
		# TODO: Load these from a shelf.  For now we'll statically define them
	# CUBELETS FARKUS Modules
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823887,  "`0001", "***Cubelets Flashlight/Brightness Test Module", "Flash/Bright", "Test Module", "Module", False, False))
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823888,  "`0003", "***Cubelets Communication Test Module", "Cube Comm", "Test Module", "Module", False, False))
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823889,  "`0007", "***Cubelets Flashing Shield", "Cubelets", "Flasher", "Module", False, False))
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(1823890,
									  "`0002",
									  "***Cubelets Power Test Module",
									  "Power Test",
									  "Module",
									  "Module",
									  False, False)
					)
	
		
		
		
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(5444444,
									  "1000",
									  "MOSS - Distance",
									  "Distance prgm/test",
									  "Testbed",
									  "Standalone",
									  self.serialEventHandlerStandardTestbed,
									  "PathTO the PROGRAMMER! for distance"))
		
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(54444445,
									  "1001",
									  "MOSS - Flashlight",
									  "Flashlight prgm/test",
									  "Testbed",
									  "Standalone",
									  self.serialEventHandlerStandardTestbed,
									  "PathTO the PROGRAMMER! for flashlight"))
		
		self.moduleTypes.append(FarkusModuleType.FarkusModuleType(5444446,
									  "1002",
									  "MOSS - BT Main",
									  "Bluetooth Main prgm/test",
									  "Testbed",
									  "Standalone",
									  self.serialEventHandlerStandardTestbed,
									  "PathTO the PROGRAMMER! for BT Main"))

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
		
   
