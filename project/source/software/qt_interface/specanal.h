#ifndef SPECANAL_H
#define SPECANAL_H

#include <fftw3.h>

class Specanal
{

public:
	Specanal(); // constructor
	void getPlan();
	void executeFFT();
	int fft_in_size=2048;
	double *fft_in;
	fftw_complex *fft_out;
private:
	int fft_out_size = (fft_in_size/2)+1;
	fftw_plan p;
	int fft_full_flag;
	
};
#endif // SPECANAL_H
