#include <ADCreader.h>

// constructor
ADCreader::ADCreader()
{
	int ret = 0;
	int fd;
	int sysfs_fd;
	int no_tty = !isatty( fileno(stdout) );

  //Open /dev/spidev0.0 for reading and writing.
	fd = open(device, O_RDWR);

	//If FileDescriptor has failed to open correctly, Abort
	if (fd < 0)
		pabort("can't open device");
	//Set the SPI mode of "/dev/spidev0.0"
	//Send "/dev/spidev0.0" the data of &mode with instruction write mode
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");
	//Send "/dev/spidev0.0" the data of &mode with instruction read mode
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	//Set the number of bits per word for the spi device and chip "/dev/spidev0.0"
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	//Print the SPI mode and number of bits per word
	fprintf(stderr, "spi mode: %d\n", mode);
	fprintf(stderr, "bits per word: %d\n", bits);

	// enable master clock for the AD
	// divisor results in roughly 4.9MHz
	// this also inits the general purpose IO
	gz_clock_ena(GZ_CLK_5MHz,5);

	// enables sysfs entry for the GPIO pin
	gpio_export(drdy_GPIO);
	// set to input(0)
	gpio_set_dir(drdy_GPIO,0);
	// set interrupt detection to falling edge
	//When pin !drdy on the AD7705 is low,a new word of data is ready to be read
	// from the data register of the ADC. Hence, when falling edge is detected,
	//the AD7705 is ready to send a new word of data to the microprocessor
	//responsible for handle the data.
	gpio_set_edge(drdy_GPIO,"falling");
	// get a file descriptor for the GPIO pin
	sysfs_fd = gpio_fd_open(drdy_GPIO);

	// resets the AD7705 so that it expects a write to the communication register
        printf("sending reset\n");
	writeReset(fd);

	// tell the AD7705 that the next write will be to the clock register
	writeReg(fd,0x20);
	// write 00001100 : CLOCKDIV=1,CLK=1,expects 4.9152MHz input clock
	writeReg(fd,0x0C);

	// tell the AD7705 that the next write will be the setup register
	writeReg(fd,0x10);
	// intiates a self calibration and then after that starts converting
	writeReg(fd,0x40);


}
void ADCreader::run(){
	running = true;
	while(running){
		// let's wait for data for max one second
		ret = gpio_poll(sysfs_fd,1000);
		if (ret<1) {
			fprintf(stderr,"Poll error %d\n",ret);
		}
		// tell the AD7705 to read the data register (16 bits)
		writeReg(fd,0x38);
		// read the data register by performing two 8 bit reads
		int value = readData(fd)-0x8000;
		fprintf(stderr,"data = %d       \r",value);
		// if stdout is redirected to a file or pipe, output the data
		if( no_tty ){
			printf("%d\n", value);
			fflush(stdout);
		}
	}
}

void ADCreader::quit(){
	running = false;
	exit(0);
}


//print the error message stored in the string 's' and abort
static void pabort(const char *s)
{
	perror(s);
	abort();
}

static void writeReset(int fd){
	int ret;
	uint8_t tx1[5] = {0xff,0xff,0xff,0xff,0xff};
	uint8_t rx1[5] = {0};
	struct spi_ioc_transfer tr;

	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = sizeof(tx1);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		printf("\nerr=%d when trying to reset. \n",ret);
		pabort("Can't send spi message");
	}
}

static void writeReg(int fd, uint8_t v){
	int ret;
	uint8_t tx1[1];
	tx1[0] = v;
	uint8_t rx1[1] = {0};
	struct spi_ioc_transfer tr;
	//Sets sizeof(spi_ioc_transfer) number of bytes, in the address pointed to by
	// tr to the value of zero
	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = sizeof(tx1);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1){pabort("can't send spi message");}
}

static uint8_t readReg(int fd){
	int ret;
	uint8_t tx1[1];
	tx1[0] = 0;
	uint8_t rx1[1] = {0};
	struct spi_ioc_transfer tr;

	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = sizeof(tx1);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	  pabort("can't send spi message");

	return rx1[0];
}

static int readData(int fd)
{
	int ret;
	uint8_t tx1[2] = {0,0};
	uint8_t rx1[2] = {0,0};
	struct spi_ioc_transfer tr;

	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = sizeof(tx1);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
          {
	  printf("\n can't send spi message, ret = %d\n",ret);
          exit(1);
          }

	return (rx1[0]<<8)|(rx1[1]);
}
