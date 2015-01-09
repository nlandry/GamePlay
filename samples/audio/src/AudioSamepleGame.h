#ifndef AudioSamepleGame_H_
#define AudioSamepleGame_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class AudioSamepleGame: public Game, public Control::Listener
{
public:

    /**
     * Constructor.
     */
    AudioSamepleGame();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    // Control::Listener override
    void controlEvent(Control* control, EventType eventType);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);

    Scene* _scene;
    bool _wireframe;
    Form* _form;
    AudioSource* _audioSource;
};

#endif
