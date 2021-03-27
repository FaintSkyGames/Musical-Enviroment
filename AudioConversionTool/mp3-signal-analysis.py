######################################################################################################################################
# A script to analyse the frequencies and amplitudes of the sounds within an mp3 file across time
# Takes an mp3 file as input and outputs a csv file
# csv contains = frequencies in the columns (Hz), a row for each time increment (Secs) with amplitude (dB) values
# A spectrogram plot will also be shown if requested
######################################################################################################################################

# imports from the pydub library
from pydub import AudioSegment
from pydub.utils import get_array_type

# imports signal analysis function from scipy library
from scipy import signal

# imports data handling types - arrays, numpy arrays and pandas data frames
import array
import numpy as np
import pandas as pd

# imports the pyplot
import matplotlib.pyplot as plt

# imports the argument parser library
import argparse

def main():

    show_spectrogram = False
    
    # configure the argument parser, the -i and -o parameters are mandatory
    cliparser = argparse.ArgumentParser(description="mp3-signal-analysis")
    cliparser.add_argument("--i",help="the filepath/filename of the mp3 file to analyse", required='store_true')
    cliparser.add_argument("--o",help="the filepath/filename of the csv output file", required='store_true')
    cliparser.add_argument("--show", help="shows the analysis as a spectrogram", action='store_true')

    # parse the arguments that have passed in
    args = cliparser.parse_args()
 
 	# set the input file location using the -i parameter that was passed in on script start
    input_file = args.i

    # set the output csv file location using the -o parameter that was passed in on script start
    output_file = args.o

    # set whether the spectrogram will be shown(1) or not shown(0) - default is not shown
    if args.show:
        show_spectrogram = True
    
    # load the mp3 file into an AudioSegment
    # then split to mono and obtain the left audio channel
    audio = AudioSegment.from_file(input_file)
    left_channel = audio.split_to_mono()[0]

    # perform some array conversions 
    # first copy the original left channel data into a new array
    # using the sample width of the audio * 8 bits as the array's data type
    # then convert to a numpy array so that it can be used with the scipy spectrogram
    bit_depth = left_channel.sample_width * 8
    array_type = get_array_type(bit_depth)
    signal_array = array.array(array_type, left_channel._data)
    signal_np_array = np.array(signal_array)  

    # use a scipy signal spectrogram to compute consecutive fourier transforms
    # for the whole audio track using the frame rate of the audio as the sampling frequency
    # leave the FFT windowing settings as default
    freqs, times, amplitudes = signal.spectrogram(signal_np_array,left_channel.frame_rate)

    # convert the amplitudes into decibels
    amplitudes = 20 * np.log10(amplitudes)

    # create a new pandas data frame to hold the output using
    # time for the rows, frequency in Hz as the columns, and amplitudes as the cell values    # 
    # note: the amplitude array needs to be transposed inorder to swap the rows and columns around
    # as the scipy spectrogram arranges data with rows for each frequency rather than each time increment 
    df = pd.DataFrame(amplitudes.transpose(),
                    index=times,
                    columns=freqs)

    # insert a new column at the beginning to hold the values
    # of the time increments outputed by the spectrogram
    df.insert(loc = 0, 
            column = 'Time in Sec', 
            value = times)

    # save the data frame into the csv file
    df.to_csv(output_file, index=False)

    # optionally show the spectrogram if requested
    if (show_spectrogram == True):
        plt.pcolormesh(times, freqs, amplitudes)
        plt.xlabel("Time in Seconds")
        plt.ylabel("Frequency in Hz")
        plt.show()

# run the main program
main()