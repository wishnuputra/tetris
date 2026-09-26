#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>

using namespace std;

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)

wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

// “For the cell (px, py) in the rotated 4×4 shape, which index should I read from the original 16-character Tetromino?”
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

    int nCurrentPiece = rand() % 7;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    //bool bKey[4];
    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;




    while (!bGameover)
    {
        // GAME TIMING ========================================================
        this_thread::sleep_for(50ms);
        nSpeedCounter++;
        bForceDown = (nSpeedCounter == nSpeed);

        // INPUT ==============================================================
        int key = getch();

        // GAME LOGIC =========================================================

        // movement handler
        if (key == KEY_RIGHT && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
        {
            nCurrentX = nCurrentX + 1;
        }

        if (key == KEY_LEFT && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
        {
            nCurrentX = nCurrentX - 1;
        }

        if (key == KEY_DOWN && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
        {
            nCurrentY = nCurrentY + 1;
        }

        if (key == 'z' || key == 'Z')
        {
            if (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY))
            {
                nCurrentRotation = nCurrentRotation + 1;
                bRotateHold = true;
            }
        }
        else
        {
            bRotateHold = false;
        }

        // game handler
        if (bForceDown)
        {
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
            {
                nCurrentY = nCurrentY + 1;
            }
            else
            {
                // Lock the current piece in the field
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                            pField[(nCurrentY + py)*nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;


                // Check have we got any lines
                for (int py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < nFieldHeight -1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                        {
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
                        }

                        if (bLine)
                        {
                            // Remove Line, set to
                            for (int px = 1; px < nFieldWidth - 1; px++)
                            {
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            }
                        }
                    }
                }

                // Choose next piece
                nCurrentPiece = rand() % 7;
                nCurrentRotation = 0;
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;

                // if piece does not fit -> GameOver
                bGameover = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }

            nSpeedCounter = 0;
        }

        // RENDER OUTPUT ======================================================

        // Clear screen buffer
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        {
            screen[i] = ' ';
        }

        // Draw Playing Field into the Screen Buffer
        int offset = 2;
        const string assets = " ABCDEFG=#";
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                // A field coordinate maps to y * field width + x in the 1D array.
                screen[(y + offset) * nScreenWidth + (x + offset)] = assets[pField[y * nFieldWidth + x]];

        // Draw Current Piece
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                    screen[(nCurrentY + py + offset)*nScreenWidth + (nCurrentX + px + offset)] = 'A' + nCurrentPiece;


        // Display Frame
        erase();
        for (int y = 0; y < nScreenHeight; y++)
        {
            mvaddnstr(y, 0, screen + y * nScreenWidth, nScreenWidth);
        }
        refresh();


        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    endwin();

    delete[] screen;
    delete[] pField;
    return 0;
}
