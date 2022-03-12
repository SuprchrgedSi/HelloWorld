#include "MainComponent.h"


//===============================================

void ImageProcessingThread::run()
{
    while (true)
    {
        if (threadShouldExit())
            break;

        auto canvas = juce::Image(juce::Image::PixelFormat::RGB, w, h, true);

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                canvas.setPixelAt(x, y, juce::Colour(r.nextFloat(), 
                                                     r.nextFloat(), 
                                                     r.nextFloat(), 
                                                     1.0f));
            }
        }

        if (updateRenderer)
            updateRenderer(canvas);

        wait(-1);
    }
}
//================================================
LambdaTimer::LambdaTimer(int ms, std::function<void()> f) : lambda(std::move(f))
{
    startTimer(ms);
}
LambdaTimer::~LambdaTimer()
{
    stopTimer();
}
void LambdaTimer::timerCallback()
{
    stopTimer();
    if ( lambda ) lambda();
}
//================================================
Renderer::Renderer()
{
    startTimerHz(20);
    lambdaTimer = std::make_unique<LambdaTimer>(10, [this]()
    {
        processingThread = std::make_unique<ImageProcessingThread>(getWidth(), 
                                                                   getHeight(), 
                                                                   [this](juce::Image image)
        {
            
            imageToRender.push(image);

            if (!processingThread->threadShouldExit())
            {
                lambdaTimer = std::make_unique<LambdaTimer>(1000, [this]()
                {
                    processingThread->notify();
                });
            }
        });
    });
}
Renderer::~Renderer()
{
    stopTimer();
    lambdaTimer.reset();
    processingThread.reset();
}
void Renderer::paint(juce::Graphics& g)
{
    g.drawImage(imageToRender.read() ,
                getLocalBounds().toFloat());
}

void Renderer::timerCallback()
{
    repaint();
}

//================================================
Renderer2::Renderer2()
{
    juce::Timer::callAfterDelay(10, [this]()
    {
        SafePointer<Renderer2> safePtr(this);
        if (safePtr.getComponent()) 
            safePtr->loop();
    });
        
}

void Renderer2::paint(juce::Graphics& g)
{
    g.drawImage(imageToRender.read(),
        getLocalBounds().toFloat());
}
    
void Renderer2::loop()
{
    auto w = getWidth();
    auto h = getHeight();

    juce::Thread::launch([this, w, h]()
    {
        juce::Random r;

        auto canvas = juce::Image(juce::Image::PixelFormat::RGB, w, h, true);

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                canvas.setPixelAt(x, y, juce::Colour(r.nextFloat(),
                    r.nextFloat(),
                    r.nextFloat(),
                    1.0f));
            }
        }

        SafePointer<Renderer2> safePtr(this);
        if (safePtr.getComponent())
            safePtr->imageToRender.push(canvas);

        juce::Timer::callAfterDelay(10, [this]() 
        { 
            SafePointer<Renderer2> safePtr(this);
            if (safePtr.getComponent())
                safePtr->repaint(); 
        });
        juce::Timer::callAfterDelay(1000, [this]() 
        { 
            SafePointer<Renderer2> safePtr(this);
            if (safePtr.getComponent())
                safePtr->loop(); 
        });

    });

}

//================================================
DualButton::DualButton()
{
    addAndMakeVisible(button1);
    addAndMakeVisible(button2);
    
}

void DualButton::resized()
{
    auto bounds = getLocalBounds();
    button1.setBounds(bounds.removeFromLeft(30));
    button2.setBounds(bounds);
    
}

void DualButton::setButton1Handler(std::function<void()> f)
{
    button1.onClick = f;
}

void DualButton::setButton2Handler(std::function<void()> f)
{
    button2.onClick = f;
}

//=================================================
OwnedArrayComponent::OwnedArrayComponent()
{
    for (int i = 0; i < 10; i++)
    {
        auto* button = buttons.add(new juce::TextButton(juce::String(i)));
        addAndMakeVisible(button);
        button->addListener(this);
    }
}

OwnedArrayComponent::~OwnedArrayComponent()
{
    for (auto* button : buttons)
    {
        button->removeListener(this);
    }
}

void OwnedArrayComponent::resized()
{
    auto width = getWidth() / static_cast<float>(buttons.size());
    int x = 0;
    auto h = getHeight();
    for (auto* widget : buttons)
    {
        widget->setBounds(x, 0, width, h);
        x += width;
    }
}

void OwnedArrayComponent::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == buttons.getFirst())
    {
        DBG("You clicked the first button");
    }
    else if ((clickedButton == buttons.getLast()))
    {
        DBG("You clicked the last button");
    }
    else
    {
        DBG("You clicked some other button");
    }
}

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(myComp);
    
    addAndMakeVisible(ownedArrayComp);
    ownedArrayComp.addMouseListener(this, true);

    addAndMakeVisible(dualButton);
    dualButton.setButton1Handler([this]()
    {
        repeatingThing.startTimerHz(1);
    });

    dualButton.setButton2Handler([this]()
    {
        repeatingThing.startTimerHz(4);
    });

    addAndMakeVisible(repeatingThing);

    addAndMakeVisible(asyncGui);

    addAndMakeVisible(renderer);
    
    addAndMakeVisible(renderer2);

    setSize (800, 400);
}

MainComponent::~MainComponent()
{
    myComp.removeMouseListener(this);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    myComp.setBounds(30, 30, 100, 100);
    ownedArrayComp.setBounds(myComp.getX(), myComp.getBottom() + 5, getWidth() - myComp.getX(), getHeight() - myComp.getBottom());
    //dualButton.setBounds(myComp.getRight() + 5, myComp.getY(), myComp.getWidth(), myComp.getHeight());

    dualButton.setBounds(myComp.getBounds().withX(myComp.getRight() + 5));

    repeatingThing.setBounds(dualButton.getBounds().withX(dualButton.getRight() + 5));

    asyncGui.setBounds(repeatingThing.getBounds().withX(repeatingThing.getRight() + 5));

    renderer.setBounds(asyncGui.getBounds().withX(asyncGui.getRight() + 5));
    
    renderer2.setBounds(renderer.getBounds().withX(renderer.getRight() + 5));
}
