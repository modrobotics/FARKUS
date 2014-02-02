import FarkusStandaloneModuleType

class FarkusStandaloneModuleTypeManager():
	"Class to define helper functions for FarkusModuleTypes"
	def __init__(self):
		self.moduleTypes = []
		
		# TODO: Load these from a shelf.  For now we'll statically define them
	# MOSS Testbeds below
		self.moduleTypes.append(FarkusStandaloneModuleType.FarkusStandaloneModuleType(5000001,  "`1000", "***MOSS Distance Testbed", "MOSS-Distance", "Testbed"))

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
		
   
