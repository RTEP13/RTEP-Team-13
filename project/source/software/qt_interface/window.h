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
#include <buff.h>

// class definition 'Window'
class Window : public QWidget
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // default constructor - called when a Window is declared without arguments

	void timerEvent( QTimerEvent * );

public slots:
	void setGain(double gain);
	void setBins(int bins);
	void stop();
	void play();
	void quit();
// internal variables for the window class
private:
	// Give samples and num samples, get fft, 	
	Specanal     fft;
	Buff	     tBuff(int dataSize = 64);
	
  // graphical elements from the Qwt library - http://qwt.sourceforge.net/annotated.html
	QwtKnob      knob;
	QwtThermo    thermo;
	QwtPlot      plot,spec_plot;
	QwtPlotCurve amp_curve, spec_curve;

	QSpinBox     spinner;
	QTextEdit    help_text;
	QPushButton  playBtn, stopBtn, quitBtn;	
	// layout elements from Qt itself http://qt-project.org/doc/qt-4.8/classes.html
	
	QVBoxLayout  vMainL, vSpecOptionsL, vControlsL;  // vertical layout
	QHBoxLayout  hTimeL, hSpecL, hPlayPauseL;  // horizontal layout

//	static const int plotDataSize = 128;

	// data arrays for the plot
//	double xData[tBuff.dataSize];
//	double yData[plotDataSize];

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
	QString time_x  = "Time (s)";
	QString time_y  = "Amplitude of signal";
};

#endif // WINDOW_H
