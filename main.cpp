#include <iostream>
#include <sys/mman.h> // mmap
#include <fcntl.h>  //open file
#include <cstdlib>  // exit 
#include <unistd.h> // close file


using namespace std;

int main(){

	bool st = false; // state of LED

	int gpioFd;
	void* gpioMap;
	volatile unsigned* gpio;
// create pointer to GPIO	
	gpioFd = open("/dev/gpiomem", O_RDWR|O_SYNC);
	if(gpioFd < 0){
		cout << "Can not open /dev/gpiomem" << endl;
		exit(-1);
	}

	gpioMap = mmap(
		NULL,
		4*1024,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		gpioFd,
		0
	);

	close(gpioFd);

	if(gpioMap == MAP_FAILED){
		cout << "MAP_FAILED" << endl;
		exit(-1);
	}

	gpio = (volatile unsigned*)gpioMap;
//-------------

	*gpio &= ~(7<<15|7<<18); // Clear settings for pin 5 and 6

	*gpio |= 1<<15|1<<18; // Set GPIO Pin 5 and 6 to output mode


	// main loop
	while(true){

		if(!st){ // switch On LED
			*(gpio+7) = 1<<5; // switc ON Pin 5
			sleep(1);
			*(gpio+7) = 1<<6; // switc ON Pin 6
		} else { // switch OFF LED
			*(gpio+10) = 1<<5; // switc OFF Pin 5
			sleep(1);
			*(gpio+10) = 1<<6; // switc ON Pin 6
		}
	}


	return 0;
}

