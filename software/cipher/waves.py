import matplotlib
import numpy as np
import wave
import math

from matplotlib import pyplot as plt

# Graphing helper function


def setup_graph(title='', x_label='', y_label='', fig_size=None):
    fig = plt.figure()
    if fig_size is not None:
        fig.set_size_inches(fig_size[0], fig_size[1])
    ax = fig.add_subplot(111)
    ax.set_title(title)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)


def tone_adder(freq1: int, freq2: int, amp=10):
    matplotlib.use('TkAgg')

    # Variables
    s_rate = 100000  # sample rate in Hertz
    amplitude1 = amp  # amplitude of first wave
    amplitude2 = amp  # amplitude of second wave
    num_periods = 2  # number of periods of the sine waves
    num_samples = s_rate * num_periods  # total number of samples
    x = np.linspace(0, num_periods, num_samples)
    f1 = lambda x: amplitude1*np.sin(freq1*2*np.pi*x)
    f2 = lambda x: amplitude2*np.sin(freq2*2*np.pi*x)
    sampled_fcomb = [(f1(i)+f2(i)) for i in x]

    plt.plot(x, sampled_fcomb)
    plt.xlabel("Time [sec]")
    plt.ylabel("Amplitude")
    plt.xlim(0, (1/1633)*20)
    plt.ylim(-25, 25)

    plt.show()


def graph_dtmf(dtmf_key: str):
    dtmf_lookup = {
        "1": [697, 1209],
        "2": [697, 1336],
        "3": [697, 1477],
        "a": [697, 1633],

        "4": [770, 1209],
        "5": [770, 1336],
        "6": [770, 1477],
        "b": [770, 1633],

        "7": [852, 1209],
        "8": [852, 1336],
        "9": [852, 1477],
        "c": [852, 1633],

        "*": [941, 1209],
        "0": [941, 1336],
        "#": [941, 1477],
        "d": [941, 1633],
    }
    tones = dtmf_lookup.get(dtmf_key.lower())
    if not tones:
        raise Exception(f"Invalid DTMF key {dtmf_key}")
    plt.title(f"DTMF Tone {dtmf_key}: {tones[0]}Hz + {tones[1]}Hz")
    tone_adder(tones[0], tones[1])


graph_dtmf("9")
tone_adder(852, 852, 5)
tone_adder(1477, 1477, 5)
