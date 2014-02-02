import _winreg as winreg
import itertools

def enumerate_serial_ports():
	""" Uses the Win32 registry to return an
		iterator of serial (COM) ports
		existing on this computer.
	"""
	path = 'HARDWARE\\DEVICEMAP\\SERIALCOMM'
	try:
		key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, path)
	except WindowsError:
		raise IterationError

	for i in itertools.count():
		try:
			val = winreg.EnumValue(key, i)
			if ( "\\Device\BthModem" not in str(val[0])):
				yield str(val[1])
				
		except EnvironmentError:
			break
	
def full_port_name(portname):
	""" Given a port-name (of the form COM7,
		COM12, CNCA0, etc.) returns a full
		name suitable for opening with the
		Serial class.
	"""
	m = re.match('^COM(\d+)$', portname)
	if m and int(m.group(1)) < 10:
		return portname
	return '\\\\.\\' + portname
	
def FARKUS_get_serial_ID_string(port):
	try:
		#self.statusBar.SetStatusText('Trying port ' + str(port)) 
		#s = serial.Serial(port[0], timeout=2)
		s = serial.Serial(port, timeout=2)
		
		#self.statusBar.SetStatusText('found something on' + port) 
		# maybe wait a hot second here for the bootloader to take a chill pill.
		s.write("I")
		
		
		identity = s.read(5)
		
		if(len(identity) > 0):
			# We got something back
			
			# Search the module types DB for a matching ID string
			foundModuleType = self.farkusTable.getModuleTypeManager().getModuleTypeBySerialIDString(identity)
			
								
			if ( foundModuleType ):  # Did we find a standard module?
				availablePorts.append(port[0])
				availableModuleLongNames.append(foundModuleType.getName())
				availableModuleTypes.append(foundModuleType.getSerialIDString())
				availableModuleLocations.append(False)
				#self.LogToGUI("Found " + foundModuleType.getName() + " at " + str(port[0]))
				self.farkusTable.getModuleManager().add(FarkusModule.FarkusModule(foundModuleType.getSerialIDString(), self.farkusTable.getModuleTypeManager(), port[0], self) )
				foundDevices+=1
				#Fself.statusBar.SetStatusText('Searching for FARKUS-Compatible Devices - Found ' + str(foundDevices) + ' Devices.') 
			#elif identity == self.farkusTable.getConveyance().getExpectedSerialIDString(): # Did we find a conveyance?
			#	availablePorts.append(port[0])
			#	availableModuleLongNames.append(self.farkusTable.getConveyance().getName())
			#	availableModuleTypes.append(self.farkusTable.getConveyance().getExpectedSerialIDString())
			#	availableModuleLocations.append(False)
			#	#self.LogToGUI("Found " + self.farkusTable.getConveyance().getName() + " at " + str(port[0]))
			#	self.farkusTable.getConveyance().setSerialPortIdentifier(port[0])
			#	foundDevices+=1
			#	self.statusBar.SetStatusText('Searching for FARKUS-Compatible Devices - Found ' + str(foundDevices) + ' Devices.') 
			else:
				self.LogToGUI("Unknown device found at " + str(port))
		else:
			self.LogToGUI("The device at " + str(port) +" failed to identify itself.")
		s.close()
	except serial.SerialException:
		return False
