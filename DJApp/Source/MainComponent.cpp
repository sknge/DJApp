#include "MainComponent.h"


MainComponent::MainComponent()
{
    setSize (800, 600);

    // request permissions to open input channels
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // function called when audio device is started, or when its settings (i.e. sample rate, block size, etc) are changed
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // mixes blocks from player 1 and 2
    mixerSource.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources()
{
    // called when the audio device stops, or when it is being restarted due to a setting change
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint (juce::Graphics& g)
{
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::darkred);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::goldenrod);
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    getLookAndFeel().setColour(juce::ComboBox::outlineColourId, juce::Colours::black);
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth() / 2, (getHeight() / 3)*2);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, (getHeight() / 3)*2);
    playlistComponent.setBounds(0, (getHeight() / 3)*2, getWidth(), getHeight() / 3);
}