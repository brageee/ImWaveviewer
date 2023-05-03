## Intro 
OFDM usually uses a cylic prefix (CP) that introduces repeated samples in a signal. This creates strong auto-correlation in the signal and we can use this property in different ways to extract key parameters of a OFDM signal.

What we need to know about a OFDM signal:
- FFT size
- CP length
- System sample rate
- Symbol start

## Key features of OFDM
- Nu = FFT size = number of samples in OFDM symbol without CP
- Tu = Nu/Sample rate = OFDM symbol time without CP
- 1/Tu = carrier spacing
- Ng = CP (guard) length = number of samples in cyclic prefix
- Tg = Ng/Sample rate = CP samples time
- Ns = Nu+Ng = number of samples in OFDM symbol with CP
- Ts = Tu+Tg = OFDM symbol time with CP

## Analysis steps
The steps to find key parameters of the OFDM signal is as follows:
- Perform autocorrelation to find Nu. As we may or may not be at the correct sample rate, the autocorrelation peak will not immediately tell us the FFT size, but it will tell us the carrier spacing -> Sample rate / autocorrelation peak = carrier spacing.
- Measure bandwidth with simple power spectrum.
- Estimate number of carriers: BW/carrier spacing.
- Likely FFT size is the 2^N closest to BW/carrier spacing. E.g. if BW/carrier spacing = 56.4, likely FFT size is 64.
- FFT sample rate estimate: Sample rate * FFT size / autocorrelation peak.

We use a transform called DelayAndCorrelate to find the CP length and therefore the OFDM symbol length. This transform first multiplies the signal with itself delayed by the FFT size and performs autocorrelation on the result. This results is periodic autocorrelation peaks which reveals the OFDM symbol length (and sample rate error). In Waveviewer you set the FFT length to a factor two or four more than your estimated OFDM FFT size and the delay equal to the OFDM FFT size.

We use a transform called DelayAndIntegrate to find the OFDM symbol start. This transform first multiplies the signal with itself delayed by the FFT size. Then it divides the result into blocks of OFDM symbol length and adds these magnitudes together and filters the result. In Waveviewer you set the FFT length equal to the OFDM FFT size and the delay equal to the CP length.

## Demodulation
When we have found all necessary parameters we can try to demodulate the OFDM signal (in essence remove the CP and perform a FFT). In Waveviewer you specify FFT size, CP length and symbol start. Waveviewer assumes a sample reference that is your current sample index. It will add this sample reference to the symbol start and use this index as a OFDM symbol start reference throughout the signal. If your signal does not keep this OFDM symbol timing throughout the file, for instance if you have a burst that is off the air a period not equal to a multiple of the OFDM symbol duration, you will loose OFDM symbol timing.

Waveviewer can try to estimate the carrier frequency offset (CFO) based on an average over OFDM symbols. It can also perform CFO correction on a per OFDM symbol basis based on the CP of the symbol.

In almost all cases there will be a small CFO in the signal after these corrections. Most OFDM signals contain pilot carriers with typically BPSK (or a single tone) modulation with known sequence. If your signal is unknown you only know the modulation. The small CFO left in you signal will create a individual rotation in all carriers. It can be difficult to tell how much a carrier should be rotated depending on the modulation used. For instance QPSK has a pi/2 ambiguity. 

Waveviewer can use the pilot carriers to estimate a phase rotation trajectory throughout the carriers. It means that if the estimated rotation on the pilots are -0.5, -0.1, 0.5, 1, 1.5, -1.1, we assume that the last pilot has a rotation really of 1.97 (1.5 + (pi/2-1.1)). It uses interpolation and extrapolation to find a rotation for each individual carrier. The goal is to make all carriers coherent for analysis when absolute mapping is used.





