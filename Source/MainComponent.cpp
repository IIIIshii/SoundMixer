#include "MainComponent.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audiopolicy.h>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

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

    //COM
	HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
		DBG("COM initialization failed");
    }

	//Master Volume
	IAudioEndpointVolume* pEndPointVolume = NULL;
	IMMDevice* pMMDevice = NULL;
	IMMDeviceEnumerator* pMMDeviceEnumerator = NULL;

    //‚Ü‚¸MMDevice Enumerator
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator),
		(LPVOID*)&pMMDeviceEnumerator
    );
    if (FAILED(hr)) {
        DBG("MMDevice Enumerator instantiation failed");
    }

    //ŽŸ‚ÉMMDevice
	hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pMMDevice);
	pMMDeviceEnumerator->Release();
	pMMDeviceEnumerator = NULL;
    if (FAILED(hr)) {
        DBG("MMDevice instantiation failed");
    }

	//ÅŒã‚ÉEndpointVolume
	hr = pMMDevice->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_INPROC_SERVER,
        NULL,
        (LPVOID*)&pEndPointVolume
    );
    if (FAILED(hr)) {
        DBG("EndpointVolume instantiation failed");
    }
	pEndPointVolume->GetMasterVolumeLevelScalar(&endpointVolume);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    CoUninitialize();
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
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	g.fillAll(juce::Colours::black);

    // You can add your drawing code here!
    g.setColour(juce::Colours::yellow);
	g.setFont(40.0f);
	g.drawText("Master Volume: " + std::to_string(endpointVolume * 100.0f), getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
