#ifndef SPECANAL_H
#define SPECANAL_H

#include <fftw3.h>

class Specanal
{

public:
	Specanal(); // constructor

	void setInSize(int fft_in_size);
	void setOutSize(int fft_out_size);
	void setPlan();
	void executeFFT();

	static const int fft_in_size=64;
	static const int sampRate = 500;
//	static const int fft_out_size = sampRate/2;
	static const int fft_out_size = (fft_in_size/2)+1;

	double *fft_in;
	fftw_complex *fft_out;

	double fft_x[sampRate/2];
	double fft_y[fft_out_size];

private:
	fftw_plan p;
	int fft_full_flag;
	
};
#endif // SPECANAL_H
