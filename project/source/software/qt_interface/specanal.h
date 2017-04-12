#ifndef SPECANAL_H
#define SPECANAL_H

#include <fftw3.h>

class Specanal
{

public:
	Specanal(); // constructor
	void getPlan();
	void executeFFT();

private:
	int fft_in_size = 2048;
	int fft_out_size = (fft_in_size/2)+1;
	double *fft_in = (double*)fftw_malloc(sizeof(double) * fft_in_size);
	fftw_complex *fft_out;
	fftw_plan p;
	fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (fft_out_size));
	int fft_full_flag;
	
};
#endif // SPECANAL_H
