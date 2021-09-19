TESTING - This is a testing project for the library.

To build the project, type:
	$ make

To upload the binary into the microcontroller, type:
	$ make program

Alternatively, to upload the binary, type:
	$ avrdude -c usbasp -p t85 -U flash:w:"main.hex":a

To clean up files left out from previous builds, type:
   $ make clean

