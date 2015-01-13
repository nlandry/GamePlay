#include "AudioSamepleGame.h"

// Declare our game instance
AudioSamepleGame game;

AudioSamepleGame::AudioSamepleGame()
    :  _formBackground(NULL), _formBraking(NULL),  _audioBackgroundSource(NULL), _audioBrakingSource(NULL)
{
}

void AudioSamepleGame::initialize()
{
    _formBackground = Form::create("res/background.form");

    const char * buttons[] = { "playButton", "pauseButton", "resumeButton", "rewindButton", "stopButton" };
    for (int i = 0; i < sizeof(buttons) / sizeof(uintptr_t); i++) {
        Button* button = static_cast<Button*>(_formBackground->getControl(buttons[i]));
        button->addListener(this, Control::Listener::RELEASE);
    }   

    // Create the audio source here, and feed the values into the UI controls.
    _audioBackgroundSource = AudioSource::create("res/sample.audio#backgroundTrack");

    CheckBox* checkBox = static_cast<CheckBox*>(_formBackground->getControl("loopCheckBox"));
    checkBox->setChecked(_audioBackgroundSource->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    Slider* slider = static_cast<Slider*>(_formBackground->getControl("gainSlider"));
    slider->setValue(_audioBackgroundSource->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBackground->getControl("pitchSlider"));
    slider->setValue(_audioBackgroundSource->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    _formBraking = Form::create("res/braking.form");

    Button* button = static_cast<Button*>(_formBraking->getControl("playBrakingButton"));
    button->addListener(this, Control::Listener::RELEASE);

    _audioBrakingSource = AudioSource::create("res/sample.audio#braking");

    checkBox = static_cast<CheckBox*>(_formBraking->getControl("loopBrakingCheckBox"));
    checkBox->setChecked(_audioBrakingSource->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBraking->getControl("gainBrakingSlider"));
    slider->setValue(_audioBrakingSource->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBraking->getControl("pitchBrakingSlider"));
    slider->setValue(_audioBrakingSource->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);
}

void AudioSamepleGame::finalize()
{
    SAFE_RELEASE(_audioBackgroundSource);
    SAFE_RELEASE(_formBraking);
    SAFE_RELEASE(_audioBackgroundSource);
    SAFE_RELEASE(_formBackground);
}

void AudioSamepleGame::update(float elapsedTime)
{
    _formBackground->update(elapsedTime);
    _formBraking->update(elapsedTime);
}

void AudioSamepleGame::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _formBackground->draw();
    _formBraking->draw();
}

bool AudioSamepleGame::drawScene(Node* node)
{
    return true;
}

void AudioSamepleGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void AudioSamepleGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

void AudioSamepleGame::controlEvent(Control* control, EventType eventType)
{
    switch (eventType)
    {
    case Control::Listener::RELEASE:
        if (strcmp("playButton", control->getId()) == 0) {
            _audioBackgroundSource->play();
        } else if (strcmp("pauseButton", control->getId()) == 0) {
            _audioBackgroundSource->pause();
        } else if (strcmp("resumeButton", control->getId()) == 0) {
            _audioBackgroundSource->resume();
        } else if (strcmp("rewindButton", control->getId()) == 0) {
            _audioBackgroundSource->rewind();
        } else if (strcmp("stopButton", control->getId()) == 0) {
            _audioBackgroundSource->stop();
        } else if (strcmp("playBrakingButton", control->getId()) == 0) {
            _audioBrakingSource->play();
        }
        break;
    case Control::Listener::VALUE_CHANGED:
        if (strcmp("loopCheckBox", control->getId()) == 0) {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioBackgroundSource->setLooped(loopCheckBox->isChecked());
        } else if (strcmp("gainSlider", control->getId()) == 0) {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioBackgroundSource->setGain(float(gainSlider->getValue()));
        } else if (strcmp("pitchSlider", control->getId()) == 0) {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f) {
                _audioBackgroundSource->setPitch(pitchValue);
            }
        } else if (strcmp("loopBrakingCheckBox", control->getId()) == 0) {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioBrakingSource->setLooped(loopCheckBox->isChecked());
        } else if (strcmp("gainBrakingSlider", control->getId()) == 0) {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioBrakingSource->setGain(float(gainSlider->getValue()));
        } else if (strcmp("pitchBrakingSlider", control->getId()) == 0) {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f) {
                _audioBrakingSource->setPitch(pitchValue);
            }
        }
        break;
    };
}
