#include "MainComponent.h"

template<typename ButtonType>
auto makeButton(ButtonType* buttonToMakeUnique) -> std::unique_ptr<ButtonType>
{
    return std::unique_ptr<ButtonType>(buttonToMakeUnique);
}

//==============================================================================
MainComponent::MainComponent()
{
    //addAndMakeVisible(toggleButton);

    //ButtonWrapper<juce::TextButton> button(nullptr);
    /*ButtonWrapper<juce::DrawableButton> button(std::function<void()> onClickHandler,
                                               const juce::String & name,
                                               const juce::DrawableButton::ButtonStyle buttonStyle);*/
    heapButton.reset(new HeapButtonWrapper<juce::TextButton>([](){ DBG("You clicked the heap button"); },
                                                             new juce::TextButton("HeapButton")
                                                            )
                    );
    addAndMakeVisible(textButton);
    addAndMakeVisible((*heapButton));
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (44.0f));
    g.setColour (juce::Colours::red);
    g.drawText ("Hello new PFM::C++ Student!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    //toggleButton.setBounds(10, 10, 30, 30);
    textButton->setBounds(JUCE_LIVE_CONSTANT(498),
                          JUCE_LIVE_CONSTANT(0),
                          100, 
                          30);
    //heapButton->operator->()->setBounds(10, 50, 100, 30);
    (*heapButton)->setBounds(JUCE_LIVE_CONSTANT(498),
                            JUCE_LIVE_CONSTANT(368),
                            100,
                            30);
}
