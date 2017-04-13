#include "window.h"

#include <cmath>  // for sine stuff


Window::Window() : plot( QString("Spectrum Analyer") ), gain(5), count(0) // <-- 'c++ initialisation list' - google it!
{
	// set up the gain knob
//	knob.setValue(gain);

	help_text.setReadOnly(1);
	help_text.setText(infoStr);
	playBtn.setText("Play");
	stopBtn.setText("Stop");
	quitBtn.setText("Quit");

	
	bin_arr_pos = 4; 
	spinner.setValue(bin_arr[bin_arr_pos]);	
	spinner.setRange(8,1024);
	spinner.setSuffix(" bins");
	connect(&spinner, SIGNAL(valueChanged(int)), SLOT(setBins(int)) ); 	
	
	
	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
	connect( &knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	// set up the thermometer
	thermo.setFillBrush( QBrush(Qt::green) );
	//thermo.setRange(0, 10);
	thermo.show();


	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = gain * sin( M_PI * index/15 );
	}
	for( int iindex=0; iindex<(sampRate/2); ++iindex ) {
		fft_x[iindex]=iindex;
		fft_y[iindex]=iindex;
	}
	
	// make a plot curve from the data and attach it to the plot
	amp_curve.setSamples(xData, yData, plotDataSize);
	amp_curve.attach(&plot);
	
	// don't need to fill up pre samples, can all be 0
	spec_curve.setSamples(fft_x, fft_y, sampRate/2);
	spec_curve.attach(&spec_plot);

	plot.replot();
	spec_plot.replot();

	plot.setAxisTitle(plot.xBottom,time_x);
	plot.setAxisTitle(plot.yLeft, time_y);
	spec_plot.setAxisTitle(spec_plot.xBottom,spec_x);
	spec_plot.setAxisTitle(spec_plot.yLeft,spec_y);

	plot.show();
	spec_plot.show();


	hTimeL.addWidget(&thermo);
	hTimeL.addWidget(&plot);
	hSpecL.addLayout(&vSpecOptionsL);

	hSpecL.addWidget(&spec_plot);
	vSpecOptionsL.addWidget(&spinner);
	vSpecOptionsL.addWidget(&help_text);
	vSpecOptionsL.addLayout(&vControlsL);
	
	hPlayPauseL.addWidget(&playBtn);
	hPlayPauseL.addWidget(&stopBtn);

	vControlsL.addLayout(&hPlayPauseL);
	vControlsL.addWidget(&quitBtn);

	vMainL.addLayout(&hTimeL);
	vMainL.addLayout(&hSpecL);

	setLayout(&vMainL);
}


void Window::timerEvent( QTimerEvent * )
{
	// generate an sine wave input for example purposes - you must get yours from the A/D!
	double inVal = gain * sin( M_PI * count/15.0 );
	++count;

	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = inVal;
	
	amp_curve.setSamples(xData, yData, plotDataSize);
	plot.replot();


	// set the thermometer value
	thermo.setValue( inVal +5);
	if(thermo.value()>0){
		thermo.setFillBrush ( QBrush(Qt::green));
	}
	if(thermo.value()>7){
		thermo.setFillBrush ( QBrush(Qt::yellow));
	}
	if(thermo.value()>9){
		thermo.setFillBrush ( QBrush(Qt::red));
	}
	
	// could be method, could be threaded
	if(count > fft.fft_in_size){
		for(int i=0; i<fft.fft_in_size;i++){
			fft.fft_in[i]=	yData[i];
		}
		fft.setPlan();
		fft.executeFFT();
		for(int k=0; k<fft.fft_out_size; k++){
			fft_y[k]=fft.fft_out[k][0];
		}
		spec_curve.setSamples(fft_x,fft_y,sampRate/2);
		spec_plot.replot();
	}
	
}


// this function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// for example purposes just change the amplitude of the generated input
	this->gain = gain;
}

void Window::setBins(int bins)
{
	if(bins>pbins){
		pbins = bin_arr[bin_arr_pos++];
	} else {
		pbins = bin_arr[bin_arr_pos--];		
	}
	this->bins = pbins;
}
