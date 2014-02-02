import FarkusPartType
import random

class FarkusPart():
	"Class to define the ACTUAL parts on a FARKUS Table"
	def __init__(self, partType):
		self.id = 1  #TODO: Generate UUID
		self.partType = partType
		self.testResults = []
		self.serialNumber = random.randrange(46000, 56000) # Use for Cubelet ID
		self.status = "UNKNOWN" # We don't know what this part's status is yet.
		
	def getStatus(self):
		return self.status
	
	def setStatus(self, status):
		self.status = status
	
	def getPartType(self):
		return self.partType
	
	def setSerialNumber(self, sn):
		self.serialNumber = sn
	
	def getSerialNumber(self):
		return self.serialNumber
	
	def addTestResult(self, result):
		self.testResults.append(result)
	
	