/*

This example reads from the default PCM device
and writes to standard output for 5 seconds of data.

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
  int in_size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
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
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);
  /* Set period size to 32 frames. If 1 frame then 1 sample */
  frames = 64;
  snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params,
                                      &frames, &dir);
  in_size = frames * 2; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(in_size);
  printf("Input buffer size = %d\n", in_size);
  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params,
                                         &val, &dir);

  /* While loops and --loops gives recording time */
  /*loops = 5000000 / val; */
  printf("Starting\n");
  FILE *fp;
  char filename[100]="levels.csv";
  fp=fopen(filename,"w+");
  fprintf(fp,"t, val, ,real, imag");

  fftw_complex *out;
  fftw_plan p;
  int fft_in_size = 2048;
  int n_out = (fft_in_size/2)+1;
  double *in = (double*)fftw_malloc(sizeof(double) * fft_in_size);
  out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (n_out));

  /* currently takes 64 samples in, calculates fft from those 64 samples
  *  need it to load those 64 samples into the fft buffer
  *  number of samples captured and written till full buffer can be calculated
  *   from fft buffer size / samples per buffer. then loop for that.
  *  when buffer is full, calculate the fft of the buffer.
  *  write the real and imaginary parts to the csv file.
  */
  printf("fft input size = %d\nfft output size %d\n", fft_in_size, n_out);
  int fft_full_flag = fft_in_size / in_size;
  printf("fft full flag: %d\n", fft_full_flag);
  int one = 1;
  int place = 0;
  while (1) {
    rc = snd_pcm_readi(handle, buffer, frames);
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
    int filled=0;
    /* builds up to alsa input buffer size */
    for(int i=0; i<in_size; i++){
	in[place]=buffer[i];
	place++;
	if(place == fft_in_size){
		place = 0;
		filled= 1;
		break;
	}
    }
    if(filled){
        p = fftw_plan_dft_r2c_1d(n_out, in, out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int k=0; k<n_out; k++){
            fprintf(fp, "\n,,,%f,%f", out[k][0], out[k][1]);
        }
        filled=0;
	
    }
  }
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
  return 0;
}
