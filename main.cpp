#include <stdio.h>
#include <math.h>
#include <cstring>
#include <bitset>

#include "MidiParser.h"
#include "ADSR.h"
#include "portaudio.h"
#include "rtmidi/RtMidi.h"

#include "WavetableOscillator.h"
#define NUM_SECONDS (4)
#define SAMPLE_RATE (48000)

typedef struct
{
    float left_phase;
    float right_phase;
    WavetableOscillator oscillator1;
}
paTestData;
static paTestData data;

ADSR osc1ADSR(SAMPLE_RATE);

static void checkErr(PaError err)
{
    if (err != paNoError)
    {
        printf("PortAUdio error: %s\n", Pa_GetErrorText(err));
        exit(EXIT_FAILURE);
    }
}

// Midi note array
const int maxNotes = 16;
int activeNotes[maxNotes];
int activeNoteCount;

// Midi Call Back Function
void midiCallBack(double deltatime, std::vector< unsigned char > *message, void *userData)
{
    std::vector<unsigned char> midiMessage = *message;

    // Code to print binary info of midimessage
    // std::bitset<8> x(midiMessage.front());
    // std::cout << x << ' ';

    // Check message type
    // Move these functions to Midi parser
    // Note On
    if (midiMessage.front() == 0x90)
    {
        // std::cout << "Note On\n";
        // osc1ADSR.setState(0);

        if(activeNoteCount < maxNotes)
        {
            activeNotes[activeNoteCount] = midiMessage[1];
            activeNoteCount++;

            float frequency = noteOn(midiMessage.at(1));
            data.oscillator1.setFrequency(frequency, SAMPLE_RATE);
            data.oscillator1.setState(true);
        }
    }
    // Note Off
    if (midiMessage.front() == 0x80)
    {
        //osc1ADSR.setState(3);
        bool activeNoteChanged = false;

        for (int i = activeNoteCount - 1; i >= 0; i--)
        {
            if (activeNotes[i] == midiMessage[1])
            {
                if (i == activeNoteCount - 1)
                {
                    activeNoteChanged = true;
                }
                for (int j = i; j < activeNoteCount - 1; j++)
                {
                    activeNotes[j] = activeNotes[j+1];
                }
                activeNoteCount--;
            }
        }
        if (activeNoteCount == 0)
        {
            data.oscillator1.setState(false);
        } else if (activeNoteChanged)
        {
            float frequency = noteOn(activeNotes[activeNoteCount - 1]);
            data.oscillator1.setFrequency(frequency, SAMPLE_RATE);
        }
    }

    // Other Midi Messages

}

// Audio Call Back Function
static int patestCallBack(  const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlas,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */
    float pitch = 0.0f;
    float volumeEnv = 0.0f;

    // Update ADSR
    //if (osc1ADSR.isOn()){volumeEnv = osc1ADSR.getADSRAmp();}

    // Generate audio
    for( i=0; i<framesPerBuffer; i++ )
    {
        pitch = data->oscillator1.getNextSample();
        *out++ = pitch;  /* left */
        *out++ = pitch;  /* right */
    }
    return paContinue;
};

void listAudioDevices()
{
    const int numDevices = Pa_GetDeviceCount();
    printf("Number of devices %d\n", numDevices);
    if ( numDevices < 0 )
    {
        printf("Error getting device count.\n");
        exit(EXIT_FAILURE);
    } if ( numDevices == 0 )
    {
        printf("There are no devices on this machine. \n");
        exit(EXIT_SUCCESS);
    }

    const PaDeviceInfo* deviceInfo;
    for ( int i=0; i < numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo(i);
        printf("Device %d:\n", i);
        printf("    name: %s\n", deviceInfo->name);
        printf("    maxInputChannels: %d\n", deviceInfo->maxInputChannels);
        printf("    maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
        printf("    defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
    }
}

int main()
{
    // Initialise Wavetable
    data.oscillator1.setWavetable(0); // Set wavetable

    // Initialise RtMidi
    RtMidiIn *midiIn = new RtMidiIn();
    unsigned int nPorts = midiIn->getPortCount();
    if ( nPorts == 0 ) {
        std::cout << "No ports available!\n";
    }

    midiIn->openPort( 0 );
    midiIn->setCallback(&midiCallBack);
    midiIn->ignoreTypes( false, false, false );

    // Init PortAudio Instance
    PaStream *stream;
    PaError err = Pa_Initialize();
    checkErr(err);

    listAudioDevices();

    data.left_phase = data.right_phase = 0.0;
    PaStreamParameters outputParameters;

    memset(&outputParameters, 0, sizeof(outputParameters));
    outputParameters.channelCount = 2;
    outputParameters.device = 2; // Set Device thing
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(2)->defaultLowInputLatency;

    // Open audio stream
    err = Pa_OpenStream(&stream,
        0,
        &outputParameters,
        SAMPLE_RATE,
        256,
        paNoFlag,
        patestCallBack,
        &data);
    checkErr(err);

    // Start audio stream
    err = Pa_StartStream(stream);
    checkErr(err);

    // Keep program running
    std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
    char input;
    std::cin.get(input);

    // Stop stream
    err = Pa_StopStream(stream);
    checkErr(err);
    err = Pa_CloseStream(stream);
    checkErr(err);
    err = Pa_Terminate();
    checkErr(err);
    delete midiIn;

    return EXIT_SUCCESS;
}
