#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)

wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int Rotate(int px, int py, int r)
{
    switch (r % 4)
    {
        case 0: return py * 4 + px;              // 0 degrees
        case 1: return 12 + py - (px * 4);       // 90 degrees
        case 2: return 15 - (py * 4) - px;       // 180 degrees
        case 3: return 3 - py + (px * 4);        // 270 degrees
    }

    return 0;
}
// Collision Detection
// nPosx and nPosY is the top left of the piece
bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            // Get index into piece
            int pi = Rotate(px, py, nRotation);

            // Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                        return false; // fail on first hit
                }
            }
        }
    }

    return true;
}

int main() {

    // Create assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");

    // Create Playing Field
    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
    {
        for (int y = 0; y < nFieldHeight; y++)
        {
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
        }
    }

    // Keep the tutorial's screen buffer, but use narrow characters because this
    // field renderer only draws ASCII symbols in the Ubuntu terminal.
    char* screen = new char[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = ' ';

    // Game Logic Stuff
    bool bGameover = false;
    bool bFirstFrame = true;

    int nCurrentPiece = 0;
    int nCurrentRotation = 0;




    while (!bGameover)
    {
        // GAME TIMING ========================================================


        // INPUT ==============================================================


        // GAME LOGIC =========================================================


        // RENDER OUTPUT ======================================================

        // Draw Playing Field into the Screen Buffer
        int offset = 2;
        const string assets = " ABCDEFG=#";
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                // A field coordinate maps to y * field width + x in the 1D array.
                screen[(y + offset) * nScreenWidth + (x + offset)] = assets[pField[y * nFieldWidth + x]];


        // \x1b[2J is an ANSI escape sequence used by programmers to clear the terminal screen
        // \x1b: This represents the Escape character (ESC, ASCII value 27 in decimal or 1B in hexadecimal).
        // It signals to the terminal emulator that the characters following it are instructions to manipulate
        // the display rather than plain text to be printed.
        // [: This is the Control Sequence Introducer (CSI). It marks the beginning of most multi-character
        // ANSI commands.
        // 2: A parameter specifying the scope of the action. In the context of the clear screen command,
        // 2 means "erase the entire visible screen". (Using a 0 clears from the cursor to the end of the screen,
        // and a 1 clears from the beginning of the screen to the cursor).
        // The \x1b[H part tells the terminal to move the cursor back to the "home" position (the top-left corner
        // of the screen). Without it, your terminal screen would be wiped clean, but your cursor would remain
        // floating down in the middle of the blank space!
        if (bFirstFrame)
        {
            std::cout << "\x1b[2J"; // Clear old terminal contents once.
            bFirstFrame = false;
        }
        std::cout << "\x1b[H";
        for (int y = 0; y < nScreenHeight; y++)
        {
            std::cout.write(screen + y * nScreenWidth, nScreenWidth);
            std::cout << '\n';
        }
        std::cout.flush();

        // Replace Windows timing APIs with the standard C++ sleep mechanism.
        // This field-only tutorial stage has no input yet; Ctrl-C stops the loop.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    delete[] screen;
    delete[] pField;
    return 0;
}
