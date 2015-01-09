#include "AudioSamepleGame.h"

// Declare our game instance
AudioSamepleGame game;

AudioSamepleGame::AudioSamepleGame()
    : _scene(NULL), _wireframe(false), _form(NULL), _audioSource(NULL)
{
}

void AudioSamepleGame::initialize()
{
    // Load game scene from file
    _scene = Scene::load("res/demo.scene");

    // Get the box model and initialize its material parameter values and bindings
    Node* boxNode = _scene->findNode("box");
    Model* boxModel = boxNode->getModel();
    Material* boxMaterial = boxModel->getMaterial();

    // Set the aspect ratio for the scene's camera to match the current resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

    _form = Form::create("res/audio.form");

    const char * buttons[] = { "playButton", "pauseButton", "resumeButton", "rewindButton", "stopButton" };
    for (int i = 0; i < sizeof(buttons) / sizeof(uintptr_t); i++) {
        Button* button = static_cast<Button*>(_form->getControl(buttons[i]));
        button->addListener(this, Control::Listener::RELEASE);
    }   

    // Create the audio source here, and feed the values into the UI controls.
    _audioSource = AudioSource::create("res/sample.audio#backgroundTrack");

    CheckBox* checkBox = static_cast<CheckBox*>(_form->getControl("loopCheckBox"));
    checkBox->setChecked(_audioSource->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    Slider* slider = static_cast<Slider*>(_form->getControl("gainSlider"));
    slider->setValue(_audioSource->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_form->getControl("pitchSlider"));
    slider->setValue(_audioSource->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);
}

void AudioSamepleGame::finalize()
{
    SAFE_RELEASE(_audioSource);
    SAFE_RELEASE(_form);
    SAFE_RELEASE(_scene);
}

void AudioSamepleGame::update(float elapsedTime)
{
    // Rotate model
    _scene->findNode("box")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 180.0f));
    _form->update(elapsedTime);
}

void AudioSamepleGame::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &AudioSamepleGame::drawScene);
    _form->draw();
}

bool AudioSamepleGame::drawScene(Node* node)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw(_wireframe);
    }
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
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        _wireframe = !_wireframe;
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}

void AudioSamepleGame::controlEvent(Control* control, EventType eventType)
{
    switch (eventType)
    {
    case Control::Listener::RELEASE:
        if (strcmp("playButton", control->getId()) == 0) {
            _audioSource->play();
        } else if (strcmp("pauseButton", control->getId()) == 0) {
            _audioSource->pause();
        } else if (strcmp("resumeButton", control->getId()) == 0) {
            _audioSource->resume();
        } else if (strcmp("rewindButton", control->getId()) == 0) {
            _audioSource->rewind();
        } else if (strcmp("stopButton", control->getId()) == 0) {
            _audioSource->stop();
        }
        break;
    case Control::Listener::VALUE_CHANGED:
        if (strcmp("loopCheckBox", control->getId()) == 0) {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioSource->setLooped(loopCheckBox->isChecked());
        } else if (strcmp("gainSlider", control->getId()) == 0) {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioSource->setGain(float(gainSlider->getValue()));
        } else if (strcmp("pitchSlider", control->getId()) == 0) {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f) {
                _audioSource->setPitch(pitchValue);
            }
        }
        break;
    };
}
