# Real Time Spectrum Analyser

Audio Level Indicator and Spectrum Analyser for the Raspberry Pi (Zero)

## Prerequisites
Ensure to install qt, qwt and other dependencies by entering the following commands into a terminal on the Pi: <br />
`sudo apt-get install qt4-dev-tools libqt4-dev qt4-qmake g++ make qtcreator` <br />
`sudo apt-get install libqwt-dev libqwt-doc libqwt6`

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

![Alt text](/project/source/hardware/kicad/_plot/png/rtep-audio-spectrum-analyser-ray-traced.png?raw=true)

