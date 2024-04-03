#pragma once

#include <JuceHeader.h>

class WaveformDisplay  : public juce::Component, 
                        public juce::ChangeListener
{
public:
    // receives audio data from audioformatmanager
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    // from the component class: implementing drawing code for components
    void paint (juce::Graphics&) override;
    void resized() override;
    // links to listener for waveform display to  be redrawn
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    // path to audio files that will be loaded
    void loadURL(juce::URL audioURL);

    /** set the relative position f the playhead */
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
