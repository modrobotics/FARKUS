class FarkusPartType():
	"Class to define the part types that can be processed by a FARKUS Table"
	def __init__(self, partTypeId, name, requiredTests):
		self.id = partTypeId
		self.name = name
		self.requiredTests = requiredTests
		
	def getId(self):
		return self.id
	
	def getName(self):
		return self.name
	
	def setName(self, name):
		self.name = name
		
	def getRequiredTests(self):
		return self.requiredTests
	
	
	