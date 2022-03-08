#pragma once

#include <JuceHeader.h>

struct Widget : juce::Component
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

struct OwnedArrayComponent : juce::Component
{
    OwnedArrayComponent()
    {
        for (int i = 0; i < 10; i++)
        {
            auto* widget = widgets.add(new Widget(i));
            addAndMakeVisible(widget);
        }
    }

    void resized() override
    {
        auto width = getWidth() / static_cast<float>(widgets.size());
        int x = 0;
        auto h = getHeight();
        for (auto* widget : widgets)
        {
            //widget->setBounds(x, 0, 0, static_cast<int>(width));
            widget->setBounds(x, 0, width, h);
            x += width;
        }
    }
    juce::OwnedArray<Widget> widgets;
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
    OwnedArrayComponent ownedArrayComp;
    int counter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
