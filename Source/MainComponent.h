#pragma once

#include <JuceHeader.h>

struct DualButton : public juce::Component
{
    DualButton();
    void resized() override;

private:
    juce::TextButton button1{ "button1" }, button2{ "button2" };

};

struct Widget : public  juce::Component
{
    Widget(int i) : num(i) {}
    void paint(juce::Graphics& g) override 
        { 
            g.fillAll(juce::Colours::red); 
            g.setColour(juce::Colours::black);
            g.drawRect(getLocalBounds().reduced(2));

            g.drawFittedText(juce::String(num),
                getLocalBounds(),
                juce::Justification::centred,
                1);
        }
//private:
    int num = 0;
};

struct OwnedArrayComponent : juce::Component, juce::Button::Listener
{
    OwnedArrayComponent();
    ~OwnedArrayComponent();
    void resized() override;
    void buttonClicked(juce::Button* clickedButton) override;

private:
    juce::OwnedArray<juce::TextButton> buttons;
};

struct MyComp : juce::Component
{
    void resized() override {}
    void paint(juce::Graphics& g) override 
        { g.fillAll(juce::Colours::green); }

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
        //DBG("MainComp Mouse Enter " << counter);
        //counter++;
    }
    void mouseExit(const juce::MouseEvent& e) override
    {
        //DBG("MainComp Mouse Exit " << counter);
        //counter++;
    }
    void mouseMove(const juce::MouseEvent& e) override
    {
        //DBG( "MainComp Mouse Move " << counter);
        //counter++;
    }
    void mouseDown(const juce::MouseEvent& e) override
    {
        DBG( "You clicked the mouse" );
    }

private:
    //==============================================================================
    // Your private member variables go here...
    MyComp myComp;
    OwnedArrayComponent ownedArrayComp;
    DualButton dualButton;
    //int counter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
