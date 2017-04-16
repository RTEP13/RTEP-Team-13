#ifndef ADCreader_H
#define ADCreader_H


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "gz_clk.h"
#include "gpio-sysfs.h"

class ADCreader
{

public:
	ADCreader(); // constructor with initialisation
	void quit();
	void run();
	
	static void pabort(const char *s);
	static void writeReset(int fd);
	static void writeReg(int fd, uint8_t v);
	static uint8_t readReg(int fd);
	static int readData(int fd);



	/*device points to the string "/dev/spidev0.0" which represents
	device 0, chip select 0*/
	static const char *device = "/dev/spidev0.0";
	
	// Set the SPI Mode to ClockPhase == 1 and Block Polarity == inverted(1)?
	static uint8_t mode = SPI_CPHA | SPI_CPOL;;
	static uint8_t bits = 8;
	static int drdy_GPIO = 22;


private:
	bool running;
};
#endif // ADCreader_H
