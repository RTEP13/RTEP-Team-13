#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <qwt/qwt_knob.h>
#include <qwt/qwt_slider.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

#include <QBoxLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QString>
#include <QPushButton>

#include <specanal.h>
#include <ADCreader.h>

class Window : public QWidget
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window();
	void timerEvent( QTimerEvent * );

// the functions called by signal events
public slots:
	void setGain(double gain);
	void setBins(int bins);
	void add(double val);
	void stop();
	void play();
	void quit();

private:

	// FFT and ADC classes 	
	Specanal     fft;
	ADCreader    adc;

	// Qwt objects
	QwtKnob      knob;
	QwtThermo    thermo;
	QwtPlot      plot,spec_plot;
	QwtPlotCurve amp_curve, spec_curve;

	// Some QT objects
	QSpinBox     spinner;
	QTextEdit    help_text;
	QPushButton  playBtn, stopBtn, quitBtn;	

	// layout elements from Qt
	QVBoxLayout  vMainL, vSpecOptionsL, vControlsL;  // vertical layout
	QHBoxLayout  hTimeL, hSpecL, hPlayPauseL;  // horizontal layout

	// data arrays for the plot
	static const int plotDataSize = 64;
	double xData[plotDataSize];
	double yData[plotDataSize];
	
	int ringIndex;
	double gain;
	static const int bin_arr_size = 10;
	int bin_arr[bin_arr_size]={8,16,32,64,
				128,256,512,
				1024,2048};
	int bins,pbins, bin_arr_pos;
	int count;
	
	// strings
	QString infoStr = "Hi there! <Insert \
			information here about how \
			 to use the software>";
	QString spec_x  = "Frequency (Hz)";
	QString spec_y  = "Amplitude";
	QString time_x  = "Samples";
	QString time_y  = "Amplitude of signal";
};

#endif // WINDOW_H
