from threading import *
from wx import PostEvent
from wx import PyEvent
import subprocess

class ProgrammerResultEvent(PyEvent):
    def __init__(self, returnCode, useID, eventTypeId, module):
        PyEvent.__init__(self)
        self.SetEventType(eventTypeId)
        self.returnCode = returnCode
        self.useID = useID
	self.module = module
	
class ProgrammerWorkerThread(Thread):
    def __init__(self, notify_window, CubeletID, eventId, basePath ):
	self.eventId = eventId
        Thread.__init__(self)
        self._notify_window = notify_window
        self.CubeletID = CubeletID
        self.start()
	self.module = None
	self.basePath = basePath
	
    def run(self):
	
        while True:
            # This is here just to keep the thread running
            pass
       
    def program(self, useID):
	    self.pamPath = self.basePath + self.getModule().moduleType.getProgrammerPath1()

	    child = subprocess.Popen(self.pamPath)
	    streamdata = child.communicate()[0]
	    returnCode = child.returncode
	
	    PostEvent(self._notify_window, ProgrammerResultEvent(returnCode, False, self.eventId, self.module))
    
    def setModule(self, module):
	self.module = module
	    
    def getModule(self):
	return self.module
		
    def abort(self):
        self._notify_window.programmerWorker = None
