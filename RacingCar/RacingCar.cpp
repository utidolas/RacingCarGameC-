#include <iostream>
using namespace std;
#include "olcConsoleGameEngine.h"

class OneLoneCoder_FormulaOLC : public olcConsoleGameEngine {
public:
    OneLoneCoder_FormulaOLC() {
        m_sAppName = L"Formula OLC";
    }

private:
    float fCarPos = .0f;
    float fDistance = .0f;

protected:
    virtual bool OnUserCreate() {
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {

        // debbuging
        if (m_keys[VK_UP].bHeld)
            fDistance += 100.0f * fElapsedTime;

        // Drawing empty spaces in the background
        Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

        // ************** DRAWING BOTTOM HALF OF TRACK ***************
        for (int y = 0; y < ScreenHeight() / 2; y++) { //column
            for (int x = 0; x < ScreenWidth(); x++) { //row

                // perspective obvs
                float fPerspective = (float)y / (ScreenHeight() / 2.0f);
                    
                float fMiddlePoint = .5f; // screen mid
                float fRoadWidth = .1f + fPerspective *.8f; // adjusting to fit screen
                float fClipWidth = fRoadWidth * .15f;

                fRoadWidth *= .5f;

                int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * ScreenWidth();
                int nLeftClip = (fMiddlePoint - fRoadWidth) * ScreenWidth();
                int nRightClip = (fMiddlePoint + fRoadWidth) * ScreenWidth();
                int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * ScreenWidth();

                // Set to bottom of screen
                int nRow = ScreenHeight() / 2 + y;

                // Perspectives colors 
                int nGrassColor = sinf(20.0f * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f ? FG_GREEN : FG_DARK_GREEN;
                int nClipColor = sinf(50.0f * powf(1.0f - fPerspective, 2) + fDistance) > 0.0f ? FG_RED : FG_WHITE;

                if (x >= 0 && x < nLeftGrass) 
                    Draw(x, nRow, PIXEL_SOLID, nGrassColor);
                if (x >= nLeftGrass && x < nLeftClip)
                    Draw(x, nRow, PIXEL_SOLID, nClipColor);
                if (x >= nLeftClip && x < nRightClip) // ROAD
                    Draw(x, nRow, PIXEL_SOLID, FG_GREY);
                if (x >= nRightClip && x < nRightGrass)
                    Draw(x, nRow, PIXEL_SOLID, nClipColor);
                if (x >= nRightGrass && x < ScreenWidth())
                    Draw(x, nRow, PIXEL_SOLID, nGrassColor);
            }
        }

        // ***************** DRAWING CAR *****************
        // 
         //drawing car in the mid and subtract offset
        int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0f) - 7; 

        DrawStringAlpha(nCarPos, 79, L"   || ## ||   ");
        DrawStringAlpha(nCarPos, 80, L"   ||####||   ");
        DrawStringAlpha(nCarPos, 81, L"   || ## ||   ");
        DrawStringAlpha(nCarPos, 82, L"     ####     ");
        DrawStringAlpha(nCarPos, 83, L"     ####     ");
        DrawStringAlpha(nCarPos, 84, L"|||  ####  |||");
        DrawStringAlpha(nCarPos, 85, L"|||########|||");
        DrawStringAlpha(nCarPos, 86, L"|||  ####  |||");
        return true;
    }
};

    

int main()
{
    OneLoneCoder_FormulaOLC game;
    game.ConstructConsole(160, 100, 8, 8); //Setting screen size and pixel
    game.Start();
    return 0;
}
