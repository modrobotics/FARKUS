class FarkusModuleType():
	"Class to define the types of modules that can be used on a FARKUS Table"
	def __init__(self, id, serialIDString, name, shortName1, shortName2, typeS, serialEventHandler, programmerEventHandler, programmerPath1):
		self.id = id
		self.serialIDString = serialIDString
		self.name = name
		self.shortName1 = shortName1
		self.shortName2 = shortName2
		self.typeS = typeS
		self.programmerPath1 = programmerPath1
		self.serialEventHandler = serialEventHandler
		self.programmerEventHandler = programmerEventHandler
		#self.version1 = version1
		#self.version2 = version2  # Not implemented yet
		#self.version3 = version3

	def getProgrammerPath1(self):
		return self.programmerPath1;   # these type of application/module-specific parameters should get moved into a key-value store
	
	def setProgrammerPath1(self, path):
		self.programmerPath1 = path;
	
	def getName(self):
		return self.name
	
	def setName(self, name):
		self.name = name
	
	def getShortName1(self):
		return self.shortName1
	
	def setShortName1(self, name):
		self.shortName1 = name
		
	def getShortName2(self):
		return self.shortName2
	
	def setShortName2(self, name):
		self.shortName2 = name
		
	def getSerialIDString(self):
		return self.serialIDString
	
	def setSerialIDString(self, serialIDString):
		self.serialIDString = serialIDString
	
	def getID(self):
		return self.id
		
	def incomingSerialHandler(self, commandIn):
		pass
	
	def getTypeS(self):
		return self.typeS
	
	def setTypeS(self, typeS):
		self.typeS = typeS
	