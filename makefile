platform=arduino:avr:nano:cpu=atmega328old
path=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
port=/dev/ttyUSB0

build: init
	arduino-cli compile --fqbn $(platform) $(path)

flash: init
	arduino-cli upload -p $(port) --fqbn $(platform) $(path)

init:
	#sudo usermod -a -G dialout $(USER)
	#sudo chmod a+rw /dev/ttyUSB0
	#echo $(path)

clean:
	echo Cleanup ...

