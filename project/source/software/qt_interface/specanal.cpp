#include <specanal.h>

// constructor
Specanal::Specanal()
{
// possible malloc for sizes of the fft?
	*fft_in = (double*)fftw_malloc(sizeof(double) * fft_in_size);
	fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (fft_out_size));
}


void Specanal::getPlan(){
	p = fftw_plan_dft_r2c_1d(fft_out_size, fft_in,
		fft_out, FFTW_ESTIMATE);
}

void Specanal::executeFFT(){
	fftw_execute(p);
}

