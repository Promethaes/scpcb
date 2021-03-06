#include "KeyBinds.h"

KeyBinds::KeyBinds(PGE::IO* inIo) {
    downInputs = Input::None;
    hitInputs = Input::None;

    io = inIo;

    mouse1 = new PGE::MouseInput(PGE::MouseInput::BUTTON::LEFT);
    mouse2 = new PGE::MouseInput(PGE::MouseInput::BUTTON::RIGHT);

    escape = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::ESCAPE);
    leftArrow = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::LEFT);
    rightArrow = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::RIGHT);
    upArrow = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::UP);
    downArrow = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::DOWN);
    leftShift = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::LSHIFT);
    rightShift = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::RSHIFT);
    backspace = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::BACKSPACE);
    del = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::DELETE);
    enter = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::RETURN);

#ifdef __APPLE__
    leftShortcutKey = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::LGUI);
    rightShortcutKey = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::RGUI);
#else
    leftShortcutKey = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::LCTRL);
    rightShortcutKey = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::RCTRL);
#endif
    keyA = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::A);
    keyC = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::C);
    keyX = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::X);
    keyV = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::V);
    keyZ = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::Z);
#ifndef __APPLE__
    keyY = new PGE::KeyboardInput(PGE::KeyboardInput::KEYCODE::Y);
#endif

    io->trackInput(mouse1);
    io->trackInput(mouse2);
    io->trackInput(escape);
    io->trackInput(leftArrow);
    io->trackInput(rightArrow);
    io->trackInput(upArrow);
    io->trackInput(downArrow);
    io->trackInput(leftShift);
    io->trackInput(rightShift);
    io->trackInput(backspace);
    io->trackInput(del);
    io->trackInput(enter);
    io->trackInput(leftShortcutKey);
    io->trackInput(rightShortcutKey);
    io->trackInput(keyA);
    io->trackInput(keyC);
    io->trackInput(keyX);
    io->trackInput(keyV);
    io->trackInput(keyZ);
#ifndef __APPLE__
    io->trackInput(keyY);
#endif
}

KeyBinds::~KeyBinds() {
    io->untrackInput(mouse1);
    io->untrackInput(mouse2);
    io->untrackInput(escape);
    io->untrackInput(leftArrow);
    io->untrackInput(rightArrow);
    io->untrackInput(upArrow);
    io->untrackInput(downArrow);
    io->untrackInput(leftShift);
    io->untrackInput(rightShift);
    io->untrackInput(backspace);
    io->untrackInput(del);
    io->untrackInput(enter);
    io->untrackInput(leftShortcutKey);
    io->untrackInput(rightShortcutKey);
    io->untrackInput(keyA);
    io->untrackInput(keyC);
    io->untrackInput(keyX);
    io->untrackInput(keyV);
    io->untrackInput(keyZ);
#ifndef __APPLE__
    io->untrackInput(keyY);
#endif

    delete mouse1; delete mouse2; delete escape;
    delete leftArrow; delete rightArrow; delete upArrow; delete downArrow; delete leftShift; delete rightShift;
    delete backspace; delete del;
    delete leftShortcutKey; delete rightShortcutKey;
    delete keyA; delete keyC; delete keyX; delete keyV; delete keyZ;
#ifndef __APPLE__
    delete keyY;
#endif

    // Untrack and delete bindings map.
    UserInputMap::iterator it;
    for (it = bindings.begin(); it != bindings.end(); it++) {
        io->untrackInput(it->second.input);
        delete it->second.input;
    }
}

bool KeyBinds::anyShiftDown() const {
    return leftShift->isDown() || rightShift->isDown();
}

bool KeyBinds::anyShortcutDown() const {
    return leftShortcutKey->isDown() || rightShortcutKey->isDown();
}

bool KeyBinds::copyIsHit() const {
    return keyC->isHit() && anyShortcutDown();
}
bool KeyBinds::cutIsHit() const {
    return keyX->isHit() && anyShortcutDown();
}
bool KeyBinds::pasteIsHit() const {
    return keyV->isHit() && anyShortcutDown();
}
bool KeyBinds::undoIsHit() const {
#ifdef __APPLE__
    return keyZ->isHit() && anyShortcutDown() && !anyShiftDown();
#else
    return keyZ->isHit() && anyShortcutDown();
#endif
}
bool KeyBinds::redoIsHit() const {
#ifdef __APPLE__
    return keyZ->isHit() && anyShortcutDown() && anyShiftDown();
#else
    return keyY->isHit() && anyShortcutDown();
#endif
}
bool KeyBinds::selectAllIsHit() const {
    return keyA->isHit() && anyShortcutDown();
}

void KeyBinds::update() {
    downInputs = Input::None;
    hitInputs = Input::None;

    UserInputMap::const_iterator it;
    for (it = bindings.begin(); it != bindings.end(); it++) {
        // Check if any of the assigned inputs are down.
        // If one of them is down/hit then the input is active.
        if (it->second.input->isDown()) {
            downInputs = downInputs | it->first;
        }
            
        if (it->second.input->isHit()) {
            hitInputs = hitInputs | it->first;
        }
    }
}

Input KeyBinds::getDownInputs() const {
    return downInputs;
}

Input KeyBinds::getHitInputs() const {
    return hitInputs;
}

void KeyBinds::bindInput(Input input, UserInput key) {
    io->trackInput(key.input);

    bindings.emplace(input, key);
}

void KeyBinds::bindInput(Input input, PGE::MouseInput::BUTTON key) {
    UserInput wrapKey;
    wrapKey.input = new PGE::MouseInput(key);
    wrapKey.code = (int)key;

    bindInput(input, wrapKey);
}

void KeyBinds::bindInput(Input input, PGE::KeyboardInput::KEYCODE key) {
    UserInput wrapKey;
    wrapKey.input = new PGE::KeyboardInput(key);
    wrapKey.code = (int)key;

    bindInput(input, wrapKey);
}

void KeyBinds::bindInput(Input input, PGE::ControllerInput::BUTTON key) {
    UserInput wrapKey;
    wrapKey.input = new PGE::ControllerInput(io->getController(0), key);
    wrapKey.code = (int)key;

    bindInput(input, wrapKey);
}

void KeyBinds::unbindInput(Input input, PGE::UserInput::DEVICE device, int key) {
    // Iterate over all values of the given key
    std::pair<UserInputMap::iterator, UserInputMap::iterator> bindingsRange = bindings.equal_range(input);
    for (UserInputMap::iterator it = bindingsRange.first; it != bindingsRange.second; it++) {
        if (it->second.input->getDevice() == device && it->second.code == key) {
            io->untrackInput(it->second.input);
            delete it->second.input;
            bindings.erase(it);
            return;
        }
    }
}

void KeyBinds::unbindInput(Input input, PGE::MouseInput::BUTTON key) {
    unbindInput(input, PGE::UserInput::DEVICE::MOUSE, (int)key);
}

void KeyBinds::unbindInput(Input input, PGE::KeyboardInput::KEYCODE key) {
    unbindInput(input, PGE::UserInput::DEVICE::KEYBOARD, (int)key);
}

void KeyBinds::unbindInput(Input input, PGE::ControllerInput::BUTTON key) {
    unbindInput(input, PGE::UserInput::DEVICE::CONTROLLER, (int)key);
}
