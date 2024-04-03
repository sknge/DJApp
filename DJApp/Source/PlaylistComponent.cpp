#include <JuceHeader.h>
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent() 
{
    trackTitles.push_back("01-180813_1305");
    trackTitles.push_back("aon_inspired");
    trackTitles.push_back("bad_frog");
    trackTitles.push_back("bleep_2");
    trackTitles.push_back("bleep_10");
    trackTitles.push_back("c_major_theme");
    trackTitles.push_back("electro_smash");
    trackTitles.push_back("fast_melody_regular_drums");
    trackTitles.push_back("fast_melody_thing");
    trackTitles.push_back("hard");
    trackTitles.push_back("ms20_improvisation");
    trackTitles.push_back("selection1");
    tableComponent.getHeader().addColumn("TRACK TITLE", 1, 400);
    tableComponent.getHeader().addColumn("DECK 1", 2, 200);
    tableComponent.getHeader().addColumn("DECK 2", 2, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);

}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::goldenrod);
    }
    else
    {
        g.fillAll(juce::Colours::white);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        g.drawText(trackTitles[rowNumber],
            2, 0,
            width - 4,
            height,
            juce::Justification::centredLeft,
            true);
    }
}

// setting load buttons per deck per track on tracklist
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "LOAD" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "LOAD" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "playlistcomponent::button clicked" << trackTitles[id] << std::endl;
}