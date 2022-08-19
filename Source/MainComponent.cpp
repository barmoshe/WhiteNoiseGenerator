#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    addAndMakeVisible(VolumeSlider);
    VolumeSlider.setRange(0, 100);
    VolumeSlider.setTextValueSuffix("%");
    addAndMakeVisible(volLable);
    volLable.setText("Volume", juce::NotificationType::dontSendNotification);
    volLable.attachToComponent(&VolumeSlider, true);
    VolumeSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 100, 20);
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    for (int ch1=0; ch1<bufferToFill.buffer->getNumChannels(); ch1++) {
        float * const buffer=bufferToFill.buffer->getWritePointer(ch1,bufferToFill.startSample);
        for (int sample=0; sample<bufferToFill.numSamples; sample++) {
            buffer[sample]=((rand.nextFloat()*2)-1)*(VolumeSlider.getValue()/100);
        }
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    juce::Rectangle<int> sliderBounds=getLocalBounds();
    sliderBounds.setY((getLocalBounds().getHeight()/4));
    sliderBounds.setWidth(getLocalBounds().getWidth()/3);
    sliderBounds.setX(getLocalBounds().getWidth()/2-(VolumeSlider.getWidth()/2)+volLable.getWidth()/2);
    VolumeSlider.setBounds(sliderBounds);
    g.fillAll (juce::Colours::black);
       g.setColour (juce::Colours::white);
       g.setFont (20.0f);
       g.drawText ("White Noise!!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
