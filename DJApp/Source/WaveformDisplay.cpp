#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                juce::AudioThumbnailCache& cacheToUse) :
                                audioThumb(1000, formatManagerToUse, cacheToUse), 
                                fileLoaded(false), 
                                position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::goldenrod);
    if (fileLoaded)
    {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0, 
            audioThumb.getTotalLength(), 
            0, 
            1.0f);
        g.setColour(juce::Colours::darkred);
        g.drawRect(position * getWidth(), 0, getWidth()/60, getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.setColour(juce::Colours::white);
        g.drawText("LOAD A FILE", getLocalBounds(),
            juce::Justification::centred, true);
    }

}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "wfd: loaded!" << std::endl;
        repaint();
    }
    else
    {
        std::cout << "wfd: not loaded!" << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    std::cout << "wfd: change received!" << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}