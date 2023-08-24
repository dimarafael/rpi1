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

	*gpio = (unsigned)0; // clear GPIO 0-9 funclion register (to input mode)
	*gpio |= 1<<3|1; // Set GPIO Pin 0 and 1 to output mode


	// main loop
	while(true){

		if(!st){ // switch On LED
			*(gpio+7) = 1; // switc ON Pin 0
			sleep(1);
			*(gpio+7) = 2; // switc ON Pin 1
		} else { // switch OFF LED
			*(gpio+10) = 1; // switc OFF Pin 0
			sleep(1);
			*(gpio+10) = 2; // switc ON Pin 1
		}
	}


	return 0;
}

