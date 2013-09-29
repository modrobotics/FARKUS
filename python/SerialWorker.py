from threading import *  # for threading support
import SerialResultEventHandler
import wx
from time import sleep
import serial
#from serial.tools import list_ports

class SerialWorkerThread0(Thread):
	"Class to provide a thread in which a serial process can run asyncronously"

	def __init__(self, notify_window, portNumber, moduleType, moduleLocation, moduleLongName, wxEventID):
		Thread.__init__(self)
		self._notify_window = notify_window
		self._portNumber = portNumber
		self._moduleType = moduleType
		self._moduleLocation = moduleLocation
		self._moduleLongName = moduleLongName
		#self.threadID = moduleLocation		# probably not needed
		#self.name = moduleLocation			# probably not needed
		self._want_abort = 0
		
		self.wxEventID = wxEventID
		try:
			self.ser = serial.Serial(self._portNumber, 9600, timeout=0)
			if self.ser.isOpen():
				wx.PostEvent(self._notify_window, SerialResultEventHandler.SerialResultEvent0("$$$OPEN$$$", self._moduleType, self._moduleLocation, self._moduleLongName, self.wxEventID))
				self.start()
			else:
				wx.PostEvent(self._notify_window, SerialResultEventHandler.SerialResultEvent0("$$$CONNECTFAIL$$$", self._moduleType, self._moduleLocation, self._moduleLongName, self.wxEventID) )
				return
		except Exception as e:
			wx.PostEvent(self._notify_window, SerialResultEventHandler.SerialResultEvent0(e, self._moduleType, self._moduleLocation, self._moduleLongName, self.wxEventID) )
			return

	def run(self):
		while True:
			try:
				if self._want_abort:
					self.ser.close()
					wx.PostEvent(self._notify_window, SerialResultEventHandler.SerialResultEvent0(None, self._moduleType, self._moduleLocation, self._moduleLongName, self.wxEventID))
					return
				data = self.ser.read(1) #read 1 byte
				if len(data) > 0:
					wx.PostEvent(self._notify_window, SerialResultEventHandler.SerialResultEvent0(data, self._moduleType, self._moduleLocation, self._moduleLongName, self.wxEventID))
				sleep(0.1)
				#while 1==1:
				#	self.ser.write('A')
			except Exception as e:
				wx.PostEvent(self._notify_window, SerialResultEventHandler.SerialResultEvent0("$$$COMMFAULT$$$"+str(e), self._moduleType, self._moduleLocation, self._moduleLongName, self.wxEventID))
				return
        
	def write(self, stringToWrite):
		self.ser.write(stringToWrite) 
    
	def abort(self):
		self._want_abort = 1