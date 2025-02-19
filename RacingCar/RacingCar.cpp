#include <iostream>
#include <string>
#include "olcConsoleGameEngine.h"
using namespace std;


class OneLoneCoder_FormulaOLC : public olcConsoleGameEngine {
public:
    OneLoneCoder_FormulaOLC() {
        m_sAppName = L"Formula OLC";
    }

private:
    float fCarPos = .0f;
    float fDistance = .0f;
    float fSpeed = .0f;

    float fCurvature = .0f;
    float fTrackCurvature = .0f;
    float fPlayerCurvature = .0f;
    float fTrackDistance = .0f;

    float fCurrentLapTime = .0f;

    list<int> listLapTimes = { 0,0,0,0,0 };
    vector<pair<float, float>> vecTrack; //curvature, distance

protected:
    virtual bool OnUserCreate() {
        // Track sections
        vecTrack.push_back(make_pair(0.0f, 10.0f)); //Start line
        vecTrack.push_back(make_pair(0.0f, 200.0f));
        vecTrack.push_back(make_pair(1.0f, 200.0f));
        vecTrack.push_back(make_pair(0.0f, 400.0f));
        vecTrack.push_back(make_pair(-1.0f, 100.0f));
        vecTrack.push_back(make_pair(0.0f, 200.0f));
        vecTrack.push_back(make_pair(-1.0f, 200.0f));
        vecTrack.push_back(make_pair(1.0f, 200.0f));
        vecTrack.push_back(make_pair(0.0f, 200.0f));
        vecTrack.push_back(make_pair(0.2f, 500.0f));
        vecTrack.push_back(make_pair(0.0f, 200.0f));

        // iteare through tracks
        for (auto t : vecTrack)
            fTrackDistance += t.second;

        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {

        // debbuging
        if (m_keys[VK_UP].bHeld)
            fSpeed += 2.0f * fElapsedTime;
        else
            fSpeed -= 1.0f * fElapsedTime;

        if (m_keys[VK_LEFT].bHeld)
            fPlayerCurvature -= .7f * fElapsedTime;

        if (m_keys[VK_RIGHT].bHeld)
            fPlayerCurvature += .7f * fElapsedTime;

        if (fabs(fPlayerCurvature - fTrackCurvature) >= .8f)
            fSpeed = 5.0f * fElapsedTime;

        // Clamp speed
        if (fSpeed < 0.0f) fSpeed = 0.0f;
        if (fSpeed > 1.0f) fSpeed = 1.0f;

        // Moving car
        fDistance += (70.0f * fSpeed) * fElapsedTime;

        // Get point on track
        float fOffset = 0;
        int nTrackSection = 0;

        fCurrentLapTime += fElapsedTime;
        if (fDistance >= fTrackDistance) {
            fDistance -= fTrackDistance;
            listLapTimes.push_front(fCurrentLapTime);
            listLapTimes.pop_back();
            fCurrentLapTime = .0f;
        }

        // Find pos on track
        while (nTrackSection < vecTrack.size() && fOffset <= fDistance) {
            fOffset += vecTrack[nTrackSection].second;
            nTrackSection++;
        }

        float fTargetCurvature = vecTrack[nTrackSection - 1].first;
        float fTrackCurveDiff = (fTargetCurvature - fCurvature) * fElapsedTime *fSpeed;
        fCurvature += fTrackCurveDiff;

        fTrackCurvature += (fCurvature)*fElapsedTime * fSpeed;

        // Drawing empty spaces in the background
        Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

        // ************** DRAWING BOTTOM HALF OF TRACK ***************
        for (int y = 0; y < ScreenHeight() / 2; y++) { //column
            for (int x = 0; x < ScreenWidth(); x++) { //row

                // perspective obvs
                float fPerspective = (float)y / (ScreenHeight() / 2.0f);

                float fMiddlePoint = .5f + fCurvature * powf((1.0f - fPerspective), 3); // screen mid w/ curve and persp
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
        fCarPos = fPlayerCurvature - fTrackCurvature;
        int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0f) - 7; 

        DrawStringAlpha(nCarPos, 79, L"   || ## ||   ");
        DrawStringAlpha(nCarPos, 80, L"   ||####||   ");
        DrawStringAlpha(nCarPos, 81, L"   || ## ||   ");
        DrawStringAlpha(nCarPos, 82, L"     ####     ");
        DrawStringAlpha(nCarPos, 83, L"     ####     ");
        DrawStringAlpha(nCarPos, 84, L"|||  ####  |||");
        DrawStringAlpha(nCarPos, 85, L"|||########|||");
        DrawStringAlpha(nCarPos, 86, L"|||  ####  |||");

        // Draw lap 
        auto disp_time = [](float t) {
            int nMinutes = t / 60.0f;
            int nSeconds = t - (nMinutes * 60.0f);
            int nMilliSeconds = (t - (float)nSeconds) * 1000.0f;
            return to_wstring(nMinutes) + L"." + to_wstring(nSeconds) + L":" + to_wstring(nMilliSeconds);
            };

        DrawString(10, 8, disp_time(fCurrentLapTime));
        // Display 5 lap times
        int j = 10;
        for (auto time : listLapTimes) {
            DrawString(10, j, disp_time(1));
            j++;

        }

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
