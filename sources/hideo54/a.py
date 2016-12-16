import wave
import numpy
import pylab

def dataFromWAV(filename):
    f = wave.open(filename, 'r')
    buf = f.readframes(f.getnframes())
    return numpy.frombuffer(buf, dtype='int16')

fig, (l, r) = pylab.subplots(ncols=2, figsize=(15, 5))
l.plot(dataFromWAV('maki.wav'))
l.set_title('MAKI')
l.set_xlim(15000, 16000)
r.plot(dataFromWAV('hanayo.wav'))
r.set_title('HANAYO')
r.set_xlim(15000, 16000)
fig.savefig('a.png')
