#pragma once

#include <JuceHeader.h>

struct MyComp : juce::Component
{
    void resized() override {}
    void paint(juce::Graphics& g) override { g.fillAll(juce::Colours::green); }

    void mouseEnter(const juce::MouseEvent& e) override 
    {
        DBG("MyComp Mouse Enter " << counter);
        counter++;
    }
    void mouseExit(const juce::MouseEvent& e) override
    {
        DBG("MyComp Mouse Exit " << counter);
        counter++;
    }
    void mouseMove(const juce::MouseEvent& e) override
    {
        DBG("MyComp Mouse Move " << counter);
        counter++;
    }
private:
    int counter = 0;
    
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& e) override
    {
        DBG("MainComp Mouse Enter " << counter);
        counter++;
    }
    void mouseExit(const juce::MouseEvent& e) override
    {
        DBG("MainComp Mouse Exit " << counter);
        counter++;
    }
    void mouseMove(const juce::MouseEvent& e) override
    {
        DBG( " MainComp Mouse Move " << counter);
        counter++;
    }


private:
    //==============================================================================
    // Your private member variables go here...
    MyComp myComp;
    int counter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
