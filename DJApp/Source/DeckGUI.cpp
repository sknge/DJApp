#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player, 
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse) : player(_player), 
    waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(rotaryFadeLabel);

    addAndMakeVisible(fadeSlider);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(unmuteButton);
  
    addAndMakeVisible(waveformDisplay);

    // button and slider listeners allow event triggers based on user interaction
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    // custom components fadeout and mute slider and button listeners
    fadeSlider.addListener(this);
    muteButton.addListener(this);
    unmuteButton.addListener(this);

    // volume slider style changed from default to vertical layout/ slider ranges and initial values set
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setValue(0.70);
    speedSlider.setRange(0.0, 10.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setValue(1.0);
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    fadeSlider.setRange(0.0, 1.0);
    fadeSlider.setValue(1);
    fadeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    fadeSlider.setNumDecimalPlacesToDisplay(3);
    fadeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);

    // setting appearance of sliders
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::white);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::goldenrod);

    // slider labels added and justified
    volLabel.setText("VOLUME", juce::dontSendNotification);
    speedLabel.setText("SPEED", juce::dontSendNotification);
    posLabel.setText("POSITION", juce::dontSendNotification);
    rotaryFadeLabel.setText("SLOW FADEOUT", juce::dontSendNotification);
    rotaryFadeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    rotaryFadeLabel.setJustificationType(juce::Justification::centred);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    // drawing code, clearing background and adding an outline 
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
}

void DeckGUI::resized()
{
    // method setting bounds and positioning/layout of child components to maintain layout when window is resized
    double rowH = getHeight() / 13;
    double colW = getWidth() / 8;
    double buttoncolW = getWidth() / 3;
    // waveform display, buttons and sliders
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 4);
    playButton.setBounds(0, rowH * 4, buttoncolW, rowH);
    stopButton.setBounds(buttoncolW, rowH * 4, buttoncolW, rowH);
    loadButton.setBounds(buttoncolW*2, rowH * 4, buttoncolW, rowH);
    volSlider.setBounds(20, rowH * 6, colW/2, rowH*6);
    posSlider.setBounds((colW*3)-20, rowH * 6, colW*5, rowH);
    speedSlider.setBounds((colW*3)-20, (rowH * 8)-20, colW*5, rowH);
    fadeSlider.setBounds(colW, rowH * 9, getWidth() / 2, rowH * 3);
    muteButton.setBounds(colW * 5, (rowH * 9)+20, colW, rowH * 2);
    unmuteButton.setBounds(colW * 6, (rowH * 9)+20, colW, rowH * 2);
    // labels positioning
    volLabel.setBounds(5, rowH * 12, 80, rowH);
    posLabel.setBounds((colW)+20, rowH * 6, 80, rowH);
    speedLabel.setBounds((colW)+20, (rowH * 8)-20, 80, rowH);
    rotaryFadeLabel.setBounds(colW, rowH * 12, getWidth() / 2, rowH);
}

// function recieves a pointer to the button, code executed when button click recieved
void DeckGUI::buttonClicked(juce::Button* button)
{
    
    if (button == &playButton)
    {
        player->start();
    }

    if (button == &stopButton)
    {
        player->stop();
    }

    if (button == &loadButton)
    {
        juce::FileChooser chooser{"LOAD A TRACK"};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
        }
    }

    // buttons to mute and unmute by setting specified gain levels instantly on click
    if (button == &muteButton)
    {
        player->setGain(0);
    }
    if (button == &unmuteButton)
    {
        player->setGain(0.7);
    }
}
// function recieves pointer to slider. executed when slider value change is registered
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
 
    // audio slow fade out takes slidervalues for speed and gain, combines them to one slider for fadeout effect
    if (slider == &fadeSlider)
    {
        player->setGain(slider->getValue());
        player->setSpeed(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag " << std::endl;
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}