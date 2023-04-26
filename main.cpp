#include "HSPD.hpp"

HSPD::console console;

class win : public HSPD::window
{
    HSPD::gfx2D g;
    HSPD::object image;
    void setup() override
    {
        g.initGfx2D(getScreenWidth(), getScreenHeight(), getScreenHWND(), &console);
        addDebugCommand("echo");
        image.initializeObject(g.getRenderTarget());
        image.loadObjectSprite(L"imageDir");
    }

    void winMain(float elapsedTime) override
    {
        HSPD::debugCommand cmd = getCommandOutput("echo");
        if (cmd.isTriggered == 1 && cmd.argv[0] != "")
        {
            console.writeAscii(cmd.argv[0], HSPD::console::FORE_GREEN, 0, 0, true);
        }
        
        //use begin draw to draw object sprites
        g.beginDraw();
        //THIS BOTH CLEARS THE DIRECT2D BUFFER AND THE GFX2D BUFFER
        //IF YOU ONLY WANT TO CLEAR THE GFX2D BUFFER USE FILL
        g.cls();

        g.fill({ 0, 0 }, { 120, 40 }, HSPD::blue);

        //gfx2D::render() is used to render anything other than object sprites
        g.render();

        //render the object sprites after you call the gfx2D::render() function so it doesnt get overwritten
        image.renderSprite();
        g.endDraw();
    }
};

int main()
{
    win w;
    console.initializeConsole();
    w.loadConsole(&console);
    w.constructWindow(1000, 700, L"HSPD test Win", true, 120);
}