#ifndef ADCREADER
#define ADCREADER


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <gz_clk.h>
#include <gpio-sysfs.h>

#include <QThread>

class ADCreader : public QThread
{

public:
	ADCreader(); // constructor with initialisation
	void quit();
	void run();
	
	void pabort(const char *s);
	void writeReset(int fd);
	void writeReg(int fd, uint8_t v);
	uint8_t readReg(int fd);
	int readData(int fd);

	int value;	
	int ret;
	int fd;
	int sysfs_fd;
	int no_tty = !isatty( fileno(stdout) );

	/*device points to the string "/dev/spidev0.0" which represents
	device 0, chip select 0*/
//	static constexpr const char * const device = "/dev/spidev0.0";
	
//	static const int drdy_GPIO = 22;


private:
	bool running;
};

//ADCreader::mode = SPI_CPHA | SPI_CPOL;
//ADCreader::bits = 8;
#endif // ADCreader
