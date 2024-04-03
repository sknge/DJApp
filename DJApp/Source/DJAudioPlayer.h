#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        // fetch audio file path
        void loadURL(juce::URL audioURL);
        // control of volume
        void setGain(double gain);
        // control of sound speed/frequency of audio sample flow
        void setSpeed(double ratio);
        //control playback position and relating value to waveform display
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);
       
        // control of sound play
        void start();
        void stop();

        // get the relative position of the playhead
        double getPositionRelative();

    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};
