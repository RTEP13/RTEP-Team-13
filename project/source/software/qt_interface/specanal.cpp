#include <specanal.h>

// constructor
Specanal::Specanal()
{
// possible malloc for sizes of the fft?
	fft_in = (double*)fftw_malloc(sizeof(double) * fft_in_size);
	fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* (fft_out_size));
	for (int index=0; index<(sampRate/2); ++index){
		fft_x[index]=index;
		fft_y[index]=0;
	}
}

void Specanal::setInSize(int fft_in_size){
	// deallocate fft_in then reallocate with new size
	fftw_free(this->fft_in);
	this->fft_in = (double*)fftw_malloc(sizeof(double) *fft_in_size);
}

void Specanal::setOutSize(int fft_out_size){
	// deallocate fft_out and reallocate with new size
	fftw_free(this->fft_out);
	this->fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(fft_out_size));
}

void Specanal::setPlan(){
	this->p = fftw_plan_dft_r2c_1d(fft_out_size, fft_in,
		fft_out, FFTW_ESTIMATE);
}

void Specanal::executeFFT(){
	fftw_execute(p);
}

