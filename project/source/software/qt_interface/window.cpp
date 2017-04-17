#include "window.h"
#include <cmath>  // for sine stuff

Window::Window() : plot( QString("Spectrum Analyer") ), gain(5), count(0)
{
	help_text.setReadOnly(1);
	help_text.setText(infoStr);

	playBtn.setText("Play");
	stopBtn.setText("Stop");
	quitBtn.setText("Quit");

	connect ( &playBtn, SIGNAL(clicked()), SLOT(play()) );
	connect ( &stopBtn, SIGNAL(clicked()), SLOT(stop()) );
	connect ( &quitBtn, SIGNAL(clicked()), SLOT(quit()) );
	
	bin_arr_pos = 4; 	
	spinner.setRange(8,1024);
	spinner.setSuffix(" bins");
	spinner.setValue(bin_arr[bin_arr_pos]);
	connect(&spinner, SIGNAL(valueChanged(int)), SLOT(setBins(int)) ); 	
	
	// set up the volume meter
	thermo.setFillBrush( QBrush(Qt::green) );
	thermo.setRange(0, 10);
	thermo.show();




	/****** Initialise plot data *******/
	ringIndex=0;	
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = 0;
	}
	
	// make a plot curve from the data and attach it to the plot
	amp_curve.setSamples(xData, yData, plotDataSize);
	amp_curve.attach(&plot);
	
	// don't need to fill up pre samples, can all be 0
	spec_curve.setSamples(fft.fft_x, fft.fft_y, fft.sampRate/2);
	spec_curve.attach(&spec_plot);

	plot.replot();
	spec_plot.replot();

	plot.setAxisTitle(plot.xBottom,time_x);
	plot.setAxisTitle(plot.yLeft, time_y);
	spec_plot.setAxisTitle(spec_plot.xBottom,spec_x);
	spec_plot.setAxisTitle(spec_plot.yLeft,spec_y);

	plot.show();
	spec_plot.show();

	/************ Creating layouts ***********/

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

	/********** begin adc thread ***********/
	adc.run();
}


void Window::timerEvent( QTimerEvent * )
{

//	double inVal = gain * sin( M_PI * count/15.0 );
//	++count;

	// get sample value from adc thread, add to ring buffer.
	// comment out add and uncomment memmove to for without
	// ring buffer.
	add(adc.value);
//	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );		
//	yData[plotDataSize-1] = inVal;

	amp_curve.setSamples(xData, yData, plotDataSize);
	plot.replot();

	// set the thermometer value
	thermo.setValue( adc.value +5);
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
		for(int i=0; i<fft.fft_in_size-1;i++){
		//	fft.fft_in[i]=yData[plotDataSize-fft.fft_in_size+i];
			fft.fft_in[i]=yData[i];
		}
		fft.setPlan();
		fft.executeFFT();
		for(int k=0; k<fft.fft_out_size; k++){
			fft.fft_y[k]=fft.fft_out[k][0];
		}
		spec_curve.setSamples(fft.fft_x,fft.fft_y,fft.sampRate/2);
		spec_plot.replot();
	}
	
}

void Window::add(double val)
{
	yData[ringIndex]=val;;
	ringIndex++;
	if(ringIndex>=plotDataSize){
		ringIndex=0;
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
	printf("input bins = %d\tpbins = %d", bins, pbins);
	if(bins>pbins){
		bin_arr_pos++;
	} else {
		bin_arr_pos--;
	}
	pbins = bin_arr[bin_arr_pos];
	spinner.setSpecialValueText(QString::number(pbins));
	this->bins = pbins;
	printf("setting output bins = %d\tthis bins = %d\n", pbins, this->bins);
}

void Window::quit()
{
// quits the program, destructor called
	printf("quitting");
	adc.quit();	
}

void Window::play()
{
// start data aquisition, can only be used if !isPlay
	printf("playing.. still to implement\n");
	adc.run();	
}

void Window::stop(){
// stop data aquisition, can only be used if isPlay
	printf("stopping.. still to implement\n");
}
