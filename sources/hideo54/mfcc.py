import librosa
import numpy

def getMfcc(filename):
    y, sr = librosa.load(filename)
    return librosa.feature.mfcc(y=y, sr=sr)

maki = getMfcc('maki.wav')
