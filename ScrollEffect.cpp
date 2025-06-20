// ScrollEffect.h
#ifndef SCROLL_EFFECT_H
#define SCROLL_EFFECT_H

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>  // for Sleep()

using namespace std;

vector<string> buildSudokuArt() {
    return {
        "  11111   11     11  111111      11111    11    11  11     11 ",    
        " 11       11     11  11   11    11   11   11   11   11     11 ",
        " 11       11     11  11    11  11     11  11  11    11     11 ",
        "  11111   11     11  11    11  11     11  1111      11     11 ",
        "      11  11     11  11    11  11     11  11  11    11     11 ",
        "      11  11     11  11   11    11   11   11   11   11     11 ",
        " 11111     1111111   111111      11111    11    11   1111111  "
    };
}

void scrollSudoku(int windowWidth = 50, int delayMillis = 100) 
{
    auto art = buildSudokuArt();
    int pad = windowWidth;
    for (auto &line : art)
    {
        line = string(pad, ' ') + line + string(pad, ' ');
    }
        
    int totalSteps = art[0].size() - windowWidth;

    for (int step = 0; step < totalSteps; ++step) 
    {
        system("cls"); // or "clear" on Linux/macOS
        for (auto &line : art)
            cout << line.substr(step, windowWidth) << "\n";
        Sleep(delayMillis);
    }
}

#endif // SCROLL_EFFECT_H
