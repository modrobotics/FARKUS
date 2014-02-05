from threading import *
from wx import PostEvent
from wx import PyEvent


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
	print "We're calling " + self.pamPath

        # The CIMS manager should refuse to load if there is no Programmer Action Module selected, but just in case....
        #if None is None:
        #    # Invalid payload or none selected
        #    PostEvent(self._notify_window, ProgrammerResultEvent(7, False, self.eventId, self.module))
        #else:
	try:
	    #pass
	    # This startupinfo crazyness hides the command window from the user
	    startupinfo = None
	    startupinfo = subprocess.STARTUPINFO()
	    startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
	    returnCode = subprocess.call([self.pamPath, str(self.CubeletID)], startupinfo=startupinfo)
	    PostEvent(self._notify_window, ProgrammerResultEvent(returnCode, useID, self.eventId, self.module))
	except:
	    #pass
	    PostEvent(self._notify_window, ProgrammerResultEvent(6, False, self.eventId, self.module))
        #self.abort()
    
    def setModule(self, module):
	self.module = module
	    
    def getModule(self):
	return self.module
		
    def abort(self):
        self._notify_window.programmerWorker = None
