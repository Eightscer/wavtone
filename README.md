# wavtone #

## Simple program that generates a tone in the form of a .wav file ##

### Build ###

Change to the root directory and run `make`.

Running `make clean` will remove object files, the compiled program, and `test.wav` if it exists.

### Usage ###
`-d` or `--demo`: Creates the default 5 second 440 Hz sine wave

`-f` or `--frequency`: integer frequency of the tone in Hz.

`-t` or `--time`: floating point duration of the tone in seconds.

`-w` or `--waveform`: specifies waveform of the tone. Waveform options (must be in lowercase):
- `sine`
- `square`
- `sawtooth`
- `triangle`

The output file will be named `output.wav`. If running the demo, the file will be named `test.wav`. If the file already exists, the program will not run as to not overwrite anything.

### Purpose ###

This is a simple program meant as an exercise of understanding file formats, file operations, and version control. At the moment, it is not intended to serve any serious purpose or application.
