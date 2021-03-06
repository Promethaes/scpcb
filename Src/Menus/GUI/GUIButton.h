#ifndef GUIBUTTON_H_INCLUDED
#define GUIBUTTON_H_INCLUDED

#include "GUIComponent.h"
#include "../../Graphics/Font.h"

class GUIFrame;
class GUIText;
class LocalizationManager;

// Ported.
class GUIButton : public GUIComponent {
    private:
        GUIFrame* frame;
        GUIText* text;

    protected:
        // MouseHit.
        bool active;
        // Activated and MouseUP.
        bool clicked;

        void updateInternal(PGE::Vector2f mousePos) override;
        void renderInternal() override;

    public:
        // Whether this button can be clicked.
        bool locked;

        GUIButton(UIMesh* um, Font* fnt, KeyBinds* kb, Config* con, LocalizationManager* lm, float x, float y, float width, float height, const PGE::String& txt, Alignment alignment = Alignment::CenterXY);
        ~GUIButton();

        bool isClicked() const;
};

#endif // GUIBUTTON_H_INCLUDED
