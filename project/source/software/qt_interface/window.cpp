#include "window.h"

#include <cmath>  // for sine stuff


Window::Window() : plot( QString("Waveform input") ), gain(5), count(0) // <-- 'c++ initialisation list' - google it!
{
	// set up the gain knob
//	knob.setValue(gain);
	
	spinner.setValue(8);	
	spinner.setRange(8,1024);
	spinner.setSuffix(" bins");
	connect( &spinner, SIGNAL(valueChanged(int)), SLOT(setBins(int)) ); 	
	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
	connect( &knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	// set up the thermometer
	thermo.setFillBrush( QBrush(Qt::green) );
	thermo.setRange(0, 50);
	thermo.show();


	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = gain * sin( M_PI * index/25 );
	}

	// make a plot curve from the data and attach it to the plot
	amp_curve.setSamples(xData, yData, plotDataSize);
	amp_curve.attach(&plot);

	spec_curve.setSamples(xData, yData, plotDataSize);
	spec_curve.attach(&plot2);

	plot.replot();
	plot2.replot();
	plot.show();
	plot2.show();

	// set up the layout - knob above thermometer
	vLayout.addWidget(&knob);
	vLayout.addWidget(&thermo);
	vLayout.addWidget(&spinner);
	vPlots.addWidget(&plot);
	vPlots.addWidget(&plot2);

	// plot to the left of knob and thermometer
	hLayout.addLayout(&vLayout);
	hLayout.addLayout(&vPlots);

	setLayout(&hLayout);
}


void Window::timerEvent( QTimerEvent * )
{
	// generate an sine wave input for example purposes - you must get yours from the A/D!
	double inVal = gain * sin( M_PI * count/25.0 );
	++count;

	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = inVal;
	amp_curve.setSamples(xData, yData, plotDataSize);
	spec_curve.setSamples(xData, yData, plotDataSize);
	plot.replot();
	plot2.replot();

	// set the thermometer value
	thermo.setValue( inVal + 5 );
	spinner.setValue(bins);
}


// this function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// for example purposes just change the amplitude of the generated input
	this->gain = gain;
}

void Window::setBins(int bins)
{
	unsigned int r=0;
	if(bins<pbins){
		r=bins;
		r = r | (r >> 1);
		r = r | (r >> 2);
		r = r | (r >> 4);
		r = r | (r >> 8);
		r = r | (r >> 16);
		this->bins=r-(r>>1);	
	} else {	
	r = bins > 0 ? bins - 1 : 0;
	// rounds up to nearest power of 2
		r |= r >> 1;
		r |= r >> 2;
		r |= r >> 4;
		r |= r >> 8;
		r |= r >> 16;
		r++;
	pbins = r;	
	this->bins = r;}
}
