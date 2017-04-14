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
	// either valueChanged or onClick?
	connect ( &playBtn, SIGNAL(clicked()), SLOT(play()) );
	connect ( &stopBtn, SIGNAL(clicked()), SLOT(stop()) );
	connect ( &quitBtn, SIGNAL(clicked()), SLOT(quit()) );
	
	bin_arr_pos = 4; 	
	spinner.setRange(8,1024);
	spinner.setSuffix(" bins");
	spinner.setValue(bin_arr[bin_arr_pos]);
	connect(&spinner, SIGNAL(valueChanged(int)), SLOT(setBins(int)) ); 	
	
	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
//	connect( &knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	// set up the thermometer
	thermo.setFillBrush( QBrush(Qt::green) );
	thermo.setRange(0, 10);
	thermo.show();

	double xData[tBuff.dataSize];
	// set up the initial plot data
	for( int index=0; index<tBuff.dataSize; ++index )
	{
		xData[index] = index;
		tBuff.yData[index] = gain * sin( M_PI * index/15 );
	}
	
	// make a plot curve from the data and attach it to the plot
	amp_curve.setSamples(xData, tBuff.yData, tBuff.dataSize);
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
//	spec_plot.setAxisScale(spec_plot.yLeft,0, 100,0);

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
	// implement ring buffer here
	
	// remove N old samples, add N new samples
	// keep yData plot
	
	tBuff.add(inVal);
//	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );		
//	yData[plotDataSize-1] = inVal;

	amp_curve.setSamples(xData, tBuff.yData, tBuff.dataSize);
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
		//	fft.fft_in[i]=yData[plotDataSize-fft.fft_in_size+i];
			fft.fft_in[i]=tBuff.yData[i];
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
	printf("quitting.. still to implement\n");
}

void Window::play()
{
// start data aquisition, can only be used if !isPlay
	printf("playing.. still to implement\n");
}

void Window::stop(){
// stop data aquisition, can only be used if isPlay
	printf("stopping.. still to implement\n");
}
