#include <specanal.h>

// constructor
Specanal::Specanal()
{
// possible malloc for sizes of the fft?
}


void Specanal::getPlan(){
	p = fftw_plan_dft_r2c_1d(fft_out_size, fft_in,
		fft_out, FFTW_ESTIMATE);
}

void Specanal::executeFFT(){
	fftw_execute(p);
}
