#pragma once

#include <JuceHeader.h>
#include <array>

//============================================
struct ImageProcessingThread : juce::Thread
{
    using ImagePassingFunc = std::function<void(juce::Image)>;
    ImageProcessingThread(int w_, int h_, ImagePassingFunc f) : Thread("ImageProcessingThread"), updateRenderer(std::move(f)), w(w_), h(h_)
    {
        startThread();
    }
    ~ImageProcessingThread()
    {
        stopThread(500);
    }
    void run() override;

    //void setUpdateRendererFunc(std::function<void(juce::Image&&)> f) { updateRenderer = std::move(f); };

private:
    int w {0}, h {0};
    ImagePassingFunc updateRenderer;
    juce::Random r;
};
//============================================
struct LambdaTimer : juce::Timer
{
    LambdaTimer(int ms, std::function<void()>f);
    ~LambdaTimer();
    void timerCallback() override;

private:
    std::function<void()> lambda;
};
//============================================
template<int Max>
struct ImageBuffer
{
    juce::Image read()
    {
        const juce::ScopedReadLock srl(rwLock);
        return images[(index % Max)];
    }
    void push(juce::Image image)
    {
        const juce::ScopedWriteLock swl(rwLock);
        images[(++index % Max)] = image;
    }
private:
    juce::ReadWriteLock rwLock;
    size_t index = 0;
    std::array<juce::Image, Max> images;
};


struct Renderer : juce::Component, juce::Timer// juce::AsyncUpdater
{
    Renderer();
    ~Renderer();
    void paint(juce::Graphics& g) override;
    //void handleAsyncUpdate() override;
    void timerCallback() override;


private:
    std::unique_ptr<ImageProcessingThread> processingThread;
    std::unique_ptr<LambdaTimer> lambdaTimer;
    //juce::Atomic<bool> firstImage{ true };
    ImageBuffer<5> imageToRender;
};
//============================================
struct DualButton : public juce::Component
{
    DualButton();
    void resized() override;

    void setButton1Handler(std::function<void()> f);
    void setButton2Handler(std::function<void()> f);

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

struct AsyncHiResGui : juce::Component, juce::AsyncUpdater, juce::HighResolutionTimer
{
    void handleAsyncUpdate() override
    {
        paintColor = (++paintColor) % maxColors;
        repaint();
    }
    void hiResTimerCallback() override 
    { 
        triggerAsyncUpdate(); 
        //repaint();
    }
    void paint(juce::Graphics& g) override
    {
        switch (paintColor)
        {
            case 0:
                g.setColour(juce::Colours::red);
                break;
            case 1:
                g.setColour(juce::Colours::green);
                break;
            case 2:
                g.setColour(juce::Colours::orange);
                break;
        }
        g.fillAll();
    }

    AsyncHiResGui() 
    {
        startTimer(1000 / 5);
    }
    ~AsyncHiResGui() 
    { 
        stopTimer(); 
        cancelPendingUpdate();
    }

private:
    int paintColor = 0;
    const int maxColors{3};
};

struct RepeatingThing : juce::Component, juce::Timer
{
    void timerCallback() override
    {
        drawRed = !drawRed;
        repaint();
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(drawRed ? juce::Colours::red : juce::Colours::green);
    }

    RepeatingThing() { startTimerHz(2); }
    ~RepeatingThing() { stopTimer(); }
private:
    bool drawRed = true;
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
    RepeatingThing repeatingThing;
    DualButton dualButton;
    AsyncHiResGui asyncGui;
    Renderer renderer;
    
    //int counter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
