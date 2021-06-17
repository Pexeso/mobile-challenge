# Welcome to the hiring challenge for mobile engineers!

In this repository there is a small library written in C programming language that is using [FFmpeg](https://ffmpeg.org/) to read videos frame by frame. 

Your task is to write language bindings (Kotlin/Java for Android, Swift for iOS) that will use this library and build it for either Android or iOS so that it can be easily used in any iOS or Android app.

### Building and installing the library

Following building instructions are for Ubuntu 20.04, it is up to you to adapt the process for a mobile platform.

We're going to need [FFmpeg](https://ffmpeg.org/) and [CMake](https://cmake.org/) to build it:

    $ sudo apt install gcc cmake libavformat-dev libavcodec-dev libavutil-dev

With the dependencies installed you can start building.

    $ git clone git@github.com:Pexeso/mobile-challenge.git
    $ cd mobile-challenge
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=../install ..
    $ make install


### Usage example

Now that you have the library installed on your system, you can look at the [example code](example/main.c) to see how the library can be used.

The example program takes a video as an argument and outputs a timestamp of each keyframe together with an information whether the frame is too dark or not.

Example video: https://cdn.eso.org/videos/hd_and_apple/eso2109a.m4v

Example output:

    $ curl -LO https://cdn.eso.org/videos/hd_and_apple/eso2109a.m4v
    $ ./example eso2109a.m4v
    ts=0.000000 is_dark=1
    ts=3.000000 is_dark=0
    ts=6.000000 is_dark=0
    ts=9.000000 is_dark=1
    ts=12.000000 is_dark=0
    ts=15.000000 is_dark=0
    ts=18.000000 is_dark=0
    ts=21.000000 is_dark=0
    ts=24.000000 is_dark=1
    ts=27.000000 is_dark=1
    ts=30.000000 is_dark=1
    ts=33.000000 is_dark=1
    ts=36.000000 is_dark=0
    ts=39.000000 is_dark=0
    ts=42.000000 is_dark=0
    ts=45.000000 is_dark=0
    ts=45.639999 is_dark=0
    ts=48.639999 is_dark=0
    ts=51.639999 is_dark=0
    ts=54.639999 is_dark=0
    ts=57.639999 is_dark=0
    ts=58.279999 is_dark=0
    ts=61.279999 is_dark=0
    ts=64.279999 is_dark=0
    ts=67.279999 is_dark=0
    ts=70.279999 is_dark=0
    ts=73.279999 is_dark=0
    ts=76.279999 is_dark=1
    ts=79.279999 is_dark=1

You can find more testing videos here under Creative Commons Attribution here: https://www.eso.org/public/videos/
