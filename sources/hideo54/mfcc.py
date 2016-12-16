import scipy.io.wavfile as wav
from python_speech_features import mfcc

def getMfcc(filename):
    (rate, sig) = wav.read(filename)
    return mfcc(sig, rate)

maki = getMfcc('maki.wav')
