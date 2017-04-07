/*

This example reads from the default PCM device
and writes to standard output for 5 seconds of data.

pI2Salsa


*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <fftw3.h>

int main() {
	long loops;
	int rc;
	int alsa_in_size;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t frames;
	char *alsa_buffer;
	/* Open PCM device for recording (capture). */
	rc = snd_pcm_open(&handle, "default",
		              SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0) {
		fprintf(stderr,"unable to open pcm device: %s\n",
			snd_strerror(rc));
		exit(1);
	}
	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it in with default values. */
	snd_pcm_hw_params_any(handle, params);

	/* Set the desired hardware parameters. */

	/* Interleaved mode */
	snd_pcm_hw_params_set_access(handle, params,
					SND_PCM_ACCESS_RW_INTERLEAVED);
	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(handle, params,
					SND_PCM_FORMAT_S16_LE);

	/* 1 channel (mono) */
	snd_pcm_hw_params_set_channels(handle, params, 1);

	/* 44100 bits/second sampling rate (CD quality) */
	val = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params,	&val, &dir);
	/* Set period size to 32 frames. If 1 frame then 1 sample */
	frames = 64;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(handle, params);
	if (rc < 0) {
		fprintf(stderr,
			"unable to set hw parameters: %s\n",
		snd_strerror(rc));
		exit(1);
	}

	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	alsa_in_size = frames * 2; /* 2 bytes/sample, 2 channels */
	alsa_buffer = (char *) malloc(alsa_in_size);
	printf("Input buffer size = %d\n", alsa_in_size);
	/* We want to loop for 5 seconds */
	snd_pcm_hw_params_get_period_time(params,&val, &dir);

	/* get csv file */
	FILE *fp;
	char filename[100]="levels.csv";
	fp=fopen(filename,"w+");
	fprintf(fp,"t, val, ,real, imag");

	fftw_complex *fft_out;
	fftw_plan p;

	/* allocate input and output buffers */
	int fft_in_size = 2048;
	int fft_out_size = (fft_in_size/2)+1;
	double *fft_in = (double*)fftw_malloc(sizeof(double) * fft_in_size);
	fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (fft_out_size));

	/* currently takes 64 samples in, calculates fft from those 64 samples
	*  need it to load those 64 samples into the fft buffer
	*  number of samples captured and written till full buffer can be calculated
	*   from fft buffer size / samples per buffer. then loop for that.
	*  when buffer is full, calculate the fft of the buffer.
	*  write the real and imaginary parts to the csv file.
	*/
	printf("fft input size = %d\nfft output size %d\n", fft_in_size, fft_out_size);
	int fft_full_flag = fft_in_size / alsa_in_size;
	printf("fft full flag: %d\n", fft_full_flag);
	int place = 0, filled = 0;
	while (1) {
		rc = snd_pcm_readi(handle, alsa_buffer, frames);
		if (rc != (int)frames) {
			if (rc == -EPIPE) {
				/* EPIPE means overrun */
				fprintf(stderr, "overrun occurred\n");
				snd_pcm_prepare(handle);
			} else if (rc < 0) {
				fprintf(stderr,
				"error from read: %s\n",
				snd_strerror(rc));
			} else {
				fprintf(stderr, "short read, read %d frames\n", rc);
			}
		}
		/* builds up to alsa input buffer size */
		for(int i=0; i<alsa_in_size; i++){
			fft_in[place]=alsa_buffer[i];
			place++;
			if(place == fft_in_size){
				place = 0;
				filled= 1;
				break;
			}
		}
		if(filled){
			p = fftw_plan_dft_r2c_1d(fft_out_size, fft_in, fft_out, FFTW_ESTIMATE);
			fftw_execute(p);
			for(int k=0; k<fft_out_size; k++){
				fprintf(fp, "\n,,,%f,%f", fft_out[k][0], fft_out[k][1]);
			}
			filled=0;
		}
	}
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(alsa_buffer);
	return 0;
}
