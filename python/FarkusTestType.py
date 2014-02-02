class FarkusTestType():
	"Class to define the tests that can be performed by FarkusModules on FarkusParts on a FarkusTable fark. Farking FarkFarker.  Fark us."
	# Eventually this class should be expanded and generalized to extend a "FarkusActions" class.  But that, my friends, is for another day.
	def __init__(self, ident, name, runBefore, runAfter, commandToStart, specificResponse, runTimeEstimate):
		self.id = ident  #TODO: Generate UUID
		self.name = name
		self.runBefore = []
		self.runAfter = []
		self.commandToStart = ""
		self.specificResponse = True  # Assume we need a specific response until told otherwise False if return value, (String) if specific response
		self.runTimeEstimate = 1 # Assume the test takes very little time 