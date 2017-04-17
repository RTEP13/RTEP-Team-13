# Real Time Spectrum Analyser

Audio Level Indicator and Spectrum Analyser for the Raspberry Pi (Zero)

## Prerequisites
Install BCM 2835 Library
* [Latest version can be found from here](http://www.airspayce.com/mikem/bcm2835/)</br>
* [Version used for this project](http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz) 
   After downloading to the pi, simply do the following from the pi's terminal
   ```
   tar zxvf bcm2835-1.52.tar.gz
   cd bcm2835-1.52
   ./configure
   make
   sudo make check
   sudo make install
   ```
* Ensure to install qt, qwt and other dependencies by entering the following commands into a terminal on the Pi:
```
$ sudo apt-get install qt4-dev-tools libqt4-dev qt4-qmake g++ make
$ sudo apt-get install libqwt-dev libqwt-doc libqwt6
$ sudo apt-get install xorg
```

## Introduction

** Still a work in progress **

The goal of this project is to provide a hardware and software solution for a real-time audio level indicator and spectrum analyser for the Raspberry Pi Zero.

The Raspberry Pi Zero can be configured as a USB virtual network gadget can can communicate to a PC through an SSH network connection. The user interface is provided making use of X forwarding using the X Windows System with the Pi Zero hosting the application.

## Hardware requirements

* Raspberry Pi Zero configured as a USB virtual network gadget. See [this](http://blog.gbaman.info/?p=699) extremely helpful walkthrough.
 
## Software requirements

* [KiCad](http://kicad-pcb.org/) - Open source electronics design suite
* [Alsa](https://www.alsa-project.org/) - Provides audio functionality to the Pi
* [FFTW3](http://www.fftw.org/) - Software library for computing DFTs
* [QT](https://www.qt.io/) - Provides user experience


## Software build

To build:
```
$ git clone git@github.com:RTEP13/rtep-spectrum-analyser.git
$ cd rtep-spectrum-analyser/project/source/software/qt_interface
$ qmake
$ make
```
To run software from directory qt\_interface:
```
$ ./QwtExample
```
![Alt text](https://raw.githubusercontent.com/RTEP13/rtep-spectrum-analyser/master/project/source/hardware/SPH0645LM4H-B_Mic_Board/_plot/png/rtep-audio-spectrum-analyser-ray-traced.png)

