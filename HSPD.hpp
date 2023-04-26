/*
*     High Speed Dirt Engine
*     Example main.cpp File Using HSPD Engine
* -------------------------------------------------
* #include "HSPD.hpp"
* 
* HSPD::console debugConsole;
* 
* class win : public HSPD::window
* {
*     gfx2D g;
*     void setup() override
*     {
*         g.initGfx2D(getScreenWidth(), getScreenHeight(), getScreenHWND(), &debugConsole);
*     }
* 
*     void winMain(float elapsedTime) override
*     {
*         g.beginDraw();
* 
*         //drawing to the screen
* 
*         g.endDraw();
*     }
* };
* 
* int main()
* {
*     win w;
*     debugConsole.initializeConsole();
*     w.loadConsole(&debugConsole);
*     w.constructWindow(1000, 700, L"HSPD", true, 120);
* }
*/

#pragma once
#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#include <stdint.h>
#include <algorithm>
#include <windows.h>
#include <psapi.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "D2D1.lib")

#include <iostream>
#include <string>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define HSPD_GAME_ENGINE

#define KEY_NOT_TOUCHED 0
#define KEY_DOWN 1
#define KEY_RELEASED 2


namespace HSPD
{
    struct point
    {
        int x, y;
        friend point operator / (point p, int divisor)
        {
            return { p.x / divisor, p.y / divisor };
        }
        friend point operator * (point p, int multiplier)
        {
            return { p.x * multiplier, p.y * multiplier };
        }
        friend point operator + (point p, int addend)
        {
            return { p.x + addend, p.y + addend };
        }
        friend point operator + (point p, HSPD::point addend)
        {
            return { p.x + addend.x, p.y + addend.y };
        }
        friend point operator - (point p, int subtrahend)
        {
            return { p.x - subtrahend, p.y - subtrahend };
        }

        void operator += (point addend)
        {
            x += addend.x;
            y += addend.y;
        }
        void operator -= (point subtrahend)
        {
            x -= subtrahend.x;
            y -= subtrahend.y;
        }
    };

    /////////////////////////////////////////////
    /// COLOUR RELEATED /////////////////////////
    /////////////////////////////////////////////

    struct colour
    {
        uint8_t r, g, b, a;
        friend colour operator / (colour c, uint8_t divisor)
        {
            colour o = { c.r / divisor, c.g / divisor, c.b / divisor, c.a / divisor };
            return o;
        }
        friend colour operator * (colour c, uint8_t multiplier)
        {
            colour o = { c.r * multiplier, c.g * multiplier, c.b * multiplier, c.a * multiplier };
            return o;
        }
        friend colour operator + (colour c, uint8_t addend)
        {
            colour o = { c.r + addend, c.g + addend, c.b + addend, c.a * addend };
            return o;
        }
        friend colour operator - (colour c, uint8_t subtrahend)
        {          
            colour o = { c.r - subtrahend, c.g - subtrahend, c.b - subtrahend, c.a - subtrahend };
            return o;
        }

        friend colour operator | (colour c, colour mask)
        {
            colour o = { c.r | mask.r, c.g | mask.g, c.b | mask.b, c.a | mask.a };
            return o;
        }
        friend colour operator & (colour c, colour mask)
        {
            colour o = { c.r & mask.r, c.g & mask.g, c.b & mask.b, c.a & mask.a };
            return o;
        }
    };

    static const colour black = { 0, 0, 0, 255 };
    static const colour red = { 255, 0, 0, 255 };
    static const colour darkRed = { 128, 0, 0, 255 };
    static const colour green = { 0, 255, 0, 255 };
    static const colour darkGreen = { 0, 128, 0, 255 };
    static const colour blue = { 0, 0, 255, 255 };
    static const colour cyan = { 0, 255, 255, 255 };
    static const colour magenta = { 255, 0, 255, 255 };
    static const colour yellow = { 255, 255, 0, 255 };
    static const colour orange = { 128, 128, 0, 255 };
    static const colour white = { 255, 255, 255, 255 };
    static const colour gray = { 128, 128, 128, 255 };
    static const colour grey = { 192, 192, 192, 255 };
    static const colour maroon = { 128, 0, 0, 255 };
    static const colour navy = { 0, 0, 128, 255 };
    static const colour teal = { 0, 128, 128, 255 };
    static const colour purple = { 128, 0, 128, 255 };

    /////////////////////////////////////////////
    /// RESOURCE MANAGER ////////////////////////
    /////////////////////////////////////////////

    class resourceManager
    {
        std::string headerFile;
        std::string rcFile = "#include \"resourceHeader.h\"\n";;

        int currentDefine = 1;
    public:

        int addResource(std::string path)
        {
            std::string name;
            int folder = 0;
            int startChar = 0;
            bool searchType = false;
            std::string type;
            for (int i = 0; i < path.length(); i++)
            {
                if (path[i] == '/')
                {
                    folder++;
                    startChar = folder;
                }
                if (path[i] == '.')
                {
                    searchType = true;
                }
                if (searchType)
                    type += path[i];
            }
            for (int i = startChar; i < path.length(); i++)
            {
                if (path[i] == '.')
                    break;
                name += path[i];
            }

            headerFile += "#define " + name + ' ' + std::to_string(currentDefine) + " // " + type + '\n';
            currentDefine++;

            rcFile += name + ' ';
            if (type == ".png")
                rcFile += "RCDATA";
            if (type == ".jpg")
                rcFile += "RCDATA";
            if (type == ".jpeg")
                rcFile += "RCDATA";
            if (type == ".bmp")
                rcFile += "BITMAP";
            if (type == ".ico")
                rcFile += "ICON";
            if (type == ".cur")
                rcFile += "CURSOR";
            rcFile += ' ';
            rcFile += '\"' + path + '\"' + '\n';

            return 0;
        }

        int createResourceFiles()
        {
            std::ofstream header("resourceHeader.h");
            header << headerFile;
            std::ofstream resourceFile("resources.rc");
            resourceFile << rcFile;
            return 0;
        }
    };

    /////////////////////////////////////////////
    /// CONSOLE RELEATED ////////////////////////
    /////////////////////////////////////////////

    class console
    {
        int consoleWidth = 120, consoleHeight = 30;
        CHAR_INFO* buffer;
        HANDLE consoleHandle;
        HANDLE consoleStdHandle = GetStdHandle(STD_INPUT_HANDLE);
        int currentXY;
    public:

        ///I grabbed this one from https://stackoverflow.com/questions/17125440/c-win32-console-color
        enum COMMAND_LINE_COLOURS
        {
            FORE_BLACK = 0,
            FORE_DARKBLUE = FOREGROUND_BLUE,
            FORE_DARKGREEN = FOREGROUND_GREEN,
            FORE_DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
            FORE_DARKRED = FOREGROUND_RED,
            FORE_DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
            FORE_DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
            FORE_DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            FORE_GRAY = FOREGROUND_INTENSITY,
            FORE_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
            FORE_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
            FORE_CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
            FORE_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
            FORE_MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
            FORE_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
            FORE_WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

            BACK_BLACK = 0,
            BACK_DARKBLUE = BACKGROUND_BLUE,
            BACK_DARKGREEN = BACKGROUND_GREEN,
            BACK_DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
            BACK_DARKRED = BACKGROUND_RED,
            BACK_DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
            BACK_DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
            BACK_DARKGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
            BACK_GRAY = BACKGROUND_INTENSITY,
            BACK_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
            BACK_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
            BACK_CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
            BACK_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
            BACK_MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
            BACK_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
            BACK_WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        };

        int writeToConsoleBuffer()
        {
            SMALL_RECT r = { 0, 0, consoleWidth - 1, consoleHeight - 1 };
            if (WriteConsoleOutputW(consoleHandle, buffer, { (short)consoleWidth, (short)consoleHeight }, { 0,0 }, &r) == 0)
                return 1;
            return 0;
        }

        void write(std::wstring str, int colour, int x, int y, bool useCurrentXY = true)
        {
            if (x >= consoleWidth)
            {
                x = 0;
                y++;
            }
            if (currentXY >= consoleWidth * consoleHeight)
            {
                currentXY = 2 * consoleWidth + 0;
                for (int x = 0; x < consoleWidth; x++)
                    for (int y = 0; y < consoleHeight; y++)
                    {
                        buffer[y * consoleWidth + x].Char.UnicodeChar = L' ';
                        buffer[y * consoleWidth + x].Attributes = 0x0;
                    }
            }
            int cxy = 0;
            if (!useCurrentXY)
            {
                for (int i = 0; i < str.length(); i++)
                {
                    if (x >= consoleWidth)
                    {
                        x = 0;
                        y++;
                    }
                    if (y * consoleWidth + x >= consoleWidth * consoleHeight)
                    {
                        y = 0;
                        x = 0;
                        for (int x = 0; x < consoleWidth; x++)
                            for (int y = 0; y < consoleHeight; y++)
                            {
                                buffer[y * consoleWidth + x].Char.UnicodeChar = L' ';
                                buffer[y * consoleWidth + x].Attributes = 0x0;
                            }
                    }

                    buffer[y * consoleWidth + x].Char.UnicodeChar = str[i];
                    buffer[y * consoleWidth + x].Attributes = colour;
                    x++;
                }
            }
            else
            {
                for (int i = 0; i < str.length(); i++)
                {
                    buffer[currentXY].Char.UnicodeChar = str[i];
                    buffer[currentXY].Attributes = colour;
                    currentXY++;
                    if (currentXY >= consoleWidth * consoleHeight)
                    {
                        currentXY = 2 * consoleWidth + 0;
                        for (int x = 0; x < consoleWidth; x++)
                            for (int y = 0; y < consoleHeight; y++)
                            {
                                buffer[y * consoleWidth + x].Char.UnicodeChar = L' ';
                                buffer[y * consoleWidth + x].Attributes = 0x0;
                            }
                    }
                    cxy = i;
                }
                cxy = consoleWidth - cxy - 1;
                SetConsoleCursorPosition(consoleHandle, { (short)x, (short)y });

            }

            currentXY += cxy;
        }

        void writeAscii(std::string str, int colour, int x, int y, bool useCurrentXY = true)
        {
            if (x >= consoleWidth)
            {
                x = 0;
                y++;
            }
            if (currentXY >= consoleWidth * consoleHeight)
            {
                currentXY = 2 * consoleWidth + 0;
                for (int x = 0; x < consoleWidth; x++)
                    for (int y = 0; y < consoleHeight; y++)
                    {
                        buffer[y * consoleWidth + x].Char.AsciiChar = L' ';
                        buffer[y * consoleWidth + x].Attributes = 0x0;
                    }
            }
            int cxy = 0;
            if (!useCurrentXY)
            {
                for (int i = 0; i < str.length(); i++)
                {
                    if (x >= consoleWidth)
                    {
                        x = 0;
                        y++;
                    }
                    if (y * consoleWidth + x >= consoleWidth * consoleHeight)
                    {
                        y = 0;
                        x = 0;
                        for (int x = 0; x < consoleWidth; x++)
                            for (int y = 0; y < consoleHeight; y++)
                            {
                                buffer[y * consoleWidth + x].Char.AsciiChar = L' ';
                                buffer[y * consoleWidth + x].Attributes = 0x0;
                            }
                    }

                    buffer[y * consoleWidth + x].Char.AsciiChar = str[i];
                    buffer[y * consoleWidth + x].Attributes = colour;
                    x++;
                }
            }
            else
            {
                for (int i = 0; i < str.length(); i++)
                {
                    buffer[currentXY].Char.UnicodeChar = str[i];
                    buffer[currentXY].Attributes = colour;
                    currentXY++;
                    if (currentXY >= consoleWidth * consoleHeight)
                    {
                        currentXY = 2 * consoleWidth + 0;
                        for (int x = 0; x < consoleWidth; x++)
                            for (int y = 0; y < consoleHeight; y++)
                            {
                                buffer[y * consoleWidth + x].Char.AsciiChar = L' ';
                                buffer[y * consoleWidth + x].Attributes = 0x0;
                            }
                    }
                    cxy = i;
                }
                cxy = consoleWidth - cxy - 1;

            }

            currentXY += cxy;
        }

        void error(std::wstring errorStr, int lineNum)
        {
            std::wstring line = L" (line number " + std::to_wstring(lineNum) + L')';
            errorStr = L"ERROR: " + errorStr;
            errorStr.append(line);

            write(errorStr, FORE_DARKRED, 0, 0, true);
        }
        void consoleLog(std::wstring logStr, int lineNum)
        {
            std::wstring line = L" (line number " + std::to_wstring(lineNum) + L')';
            logStr.append(line);
            logStr = L"LOG: " + logStr;

            write(logStr, FORE_WHITE, 0, 0, true);
        }

        int getConsoleWidth()
        {
            return consoleWidth;
        }
        int getConsoleHeight()
        {
            return consoleHeight;
        }

        void destroyConsole()
        {
            if (buffer != nullptr)
                delete buffer;
            FreeConsole();
            exit(0);
        }

        int initializeConsole(std::wstring title = L"Debug Console", int w = 120, int h = 30)
        {
            consoleWidth = w;
            consoleHeight = h;
            buffer = new CHAR_INFO[w * h];

            SMALL_RECT r = { 0, 0, consoleWidth - 1, consoleHeight - 1 };
            consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            if (SetConsoleWindowInfo(consoleHandle, TRUE, &r) == NULL)
            {
                error(L"couldnt set the console widow info", __LINE__);
                return 1;
            }

            COORD coord = { (short)consoleWidth, (short)consoleHeight };
            if (SetConsoleScreenBufferSize(consoleHandle, coord) == NULL)
            {
                error(L"Could not set the console screen buffer size", __LINE__);
                return 1;
            }

            if (SetConsoleActiveScreenBuffer(consoleHandle) == NULL)
            {
                error(L"Could not activate the screen buffer", __LINE__);
                return 1;
            }

            CONSOLE_FONT_INFOEX cfi = { 0 };
            cfi.cbSize = sizeof(cfi);
            cfi.nFont = 0;
            cfi.dwFontSize.X = 8;
            cfi.dwFontSize.Y = 16;
            cfi.FontFamily = FF_DONTCARE;
            cfi.FontWeight = FW_NORMAL;

            wcscpy_s(cfi.FaceName, L"Consolas");
            if (!SetCurrentConsoleFontEx(consoleHandle, false, &cfi))
            {
                error(L"Could not set the font size", __LINE__);
                return 1;
            }

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(consoleHandle, &csbi) == NULL)
                error(L"Could not get the console screen buffer info", __LINE__);
            if (consoleHeight > csbi.dwMaximumWindowSize.Y)
                error(L"Font Height is too big", __LINE__);
            if (consoleWidth > csbi.dwMaximumWindowSize.X)
                error(L"Font Width is too big", __LINE__);

            if (SetConsoleWindowInfo(consoleHandle, TRUE, &r) == NULL)
                error(L"Could not set the console window info", __LINE__);

            for (int i = 0; i < consoleWidth * consoleHeight; i++)
            {
                buffer[i].Char.UnicodeChar = L' ';
                buffer[i].Attributes = 0x0;
            }
            currentXY = 2 * consoleWidth;

            SetConsoleTitleW(title.c_str());

            HWND debugConsoleHwnd = GetConsoleWindow();
            if (ShowWindow(debugConsoleHwnd, SW_SHOW) == 0)
            {
                error(L"Couldnt initialize the console", __LINE__);
            }

            write(L"Console initialized", FORE_DARKGREEN, 0, 0, true);
            writeToConsoleBuffer();

            return 0;
        }  
    };

    /////////////////////////////////////////////
    /// GRAPHICS RELEATED ///////////////////////
    /////////////////////////////////////////////

    /*
        * an object can represent anything like a point in space or an image. I love objects
        */
    class object
    {
        ID2D1Bitmap* spriteBitmap;
        ID2D1RenderTarget* renderTarget;
        console* debugConsole;

        int spriteWidth, spriteHeight;
    public:

        int x = 0, y = 0;
        float spriteOpacity = 1.0f;
        int resizeFactorX = 100;
        int resizeFactorY = 100;

        int loadObjectSprite(std::wstring filePath)
        {
            IWICImagingFactory* wicFactory = NULL;
            int errorVal = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt create imaging factory. Process ended with error code " + std::to_wstring(errorVal), __LINE__);
                return 1;
            }

            IWICBitmapDecoder* wicImageDecoder = NULL;
            errorVal = wicFactory->CreateDecoderFromFilename(filePath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicImageDecoder);
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt decode bitmap. Process ended with error code " + std::to_wstring(errorVal), __LINE__);
                return 1;
            }

            IWICBitmapFrameDecode* wicFrame = NULL;
            errorVal = wicImageDecoder->GetFrame(0, &wicFrame);
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt get the bitmap. Process ended with error code " + std::to_wstring(errorVal), __LINE__);
                return 1;
            }

            IWICFormatConverter* wicFormatConverter = NULL;
            errorVal = wicFactory->CreateFormatConverter(&wicFormatConverter);
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt create the converter for the bitmap. Process ended with error code " + std::to_wstring(errorVal), __LINE__);
                return 1;
            }

            errorVal = wicFormatConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt initialize the converter for the bitmap. Process ended with error code " + std::to_wstring(errorVal), __LINE__);
                return 1;
            }

            errorVal = renderTarget->CreateBitmapFromWicBitmap(wicFormatConverter, NULL, &spriteBitmap);
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt translate bitmap to ID2D1Bitmap. Process ended with error code " + std::to_wstring(errorVal), __LINE__);
                return 1;
            }
            if (wicFactory)
                wicFactory->Release();
            if (wicImageDecoder)
                wicImageDecoder->Release();
            if (wicFormatConverter)
                wicFormatConverter->Release();
            if (wicFrame)
                wicFrame->Release();

            spriteWidth = spriteBitmap->GetSize().width;
            spriteHeight = spriteBitmap->GetSize().height;

            if (debugConsole != nullptr)
                debugConsole->consoleLog(L"Successfully loaded object sprite. Function exited with error code " + std::to_wstring(errorVal), __LINE__);

            return 0;
        }
        int loadSpriteFromMemory(HSPD::colour* buffer, int width, int height)
        {
            uint8_t* data = new uint8_t[width * height * 4];
            int posBuffer = 0;
            for (int pos = 0; pos < width * height * 4; pos += 4)
            {
                data[pos] = buffer[posBuffer].b;
                data[pos + 1] = buffer[posBuffer].g;
                data[pos + 2] = buffer[posBuffer].r;
                data[pos + 3] = buffer[posBuffer].a;
                posBuffer++;
            }

            if (renderTarget == nullptr)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Render target not initialized. Process ended with error code " + std::to_wstring(GetLastError()), __LINE__);
                return 1;
            }
            HRESULT errorVal = renderTarget->CreateBitmap(D2D1::SizeU(width, height), data, width * 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &spriteBitmap);
            delete[] data;
            if (errorVal != S_OK)
            {
                if (debugConsole != nullptr)
                    debugConsole->error(L"Couldnt create bitmap. Process ended with error code " + std::to_wstring(GetLastError()), __LINE__);
                return 1;
            }
            return 0;
        }

        int getSpriteWidth()
        {
            return spriteWidth;
        }
        int getSpriteHeight()
        {
            return spriteHeight;
        }
        ID2D1Bitmap* getSpriteBitmap()
        {
            return spriteBitmap;
        }

        void renderSprite()
        {
            if (spriteBitmap != nullptr)
                renderTarget->DrawBitmap(spriteBitmap,
                    D2D1::RectF(x, y, (float)spriteBitmap->GetSize().width * (float)resizeFactorX / 100 + x, (float)spriteBitmap->GetSize().height * (float)resizeFactorY / 100 + y)), spriteOpacity,
                D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                D2D1::RectF(0, 0, spriteBitmap->GetSize().width, spriteBitmap->GetSize().height);
        }

        void renderObjectSpriteRect(point pos, point startImg, point endImg)
        {
            if (spriteBitmap != nullptr)
                renderTarget->DrawBitmap(spriteBitmap, D2D1::RectF(pos.x, pos.y, (startImg.x - endImg.x) * (float)resizeFactorX / 100 + pos.x,
                    (float)(startImg.y - endImg.x) * (float)resizeFactorY / 100 + pos.y)), spriteOpacity,
                D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(startImg.x, startImg.y, endImg.x, endImg.y);
        }

        void destroySprite()
        {
            if (spriteBitmap != nullptr)
                spriteBitmap->Release();
        }

        void loadDebugConsole(console* c)
        {
            debugConsole = c;
        }

        void initializeObject(ID2D1RenderTarget* target)
        {
            renderTarget = target;
        }
    };

    class gfx2D
    {
        int bufferWidth, bufferHeight;

        ID2D1Factory* factory;
        ID2D1HwndRenderTarget* renderTarget;
        HSPD::colour* buffer;
        object screenObj;

        console* mainConsole;

    public:

        void drawLine(point startPos, point endPos, colour c)
        {
            if (startPos.x > bufferWidth || startPos.x < 0 || startPos.y > bufferHeight || startPos.y < 0 ||
                endPos.x > bufferWidth || endPos.x < 0 || endPos.y > bufferHeight || endPos.y < 0)
            {
                if (mainConsole != nullptr)mainConsole->error(L"Trying to draw outside of the window", __LINE__);
                return;
            }
            if (startPos.x > endPos.x)
                std::swap(startPos, endPos);

            int x0 = startPos.x;
            int y0 = startPos.y;
            int x1 = endPos.x;
            int y1 = endPos.y;

            int dx = x1 - x0;
            int dy = y1 - y0;

            if (dx == 0)
            {
                int y = y0;
                while (y <= y1)
                {
                    buffer[y * bufferWidth + x0] = c;
                    y++;
                }
                return;
            }

            if (dy == 0)
            {
                int x = x0;
                while (x <= x1)
                {
                    buffer[y0 * bufferWidth + x] = c;
                    x++;
                }
                return;
            }

            int dx2 = dx << 1;
            int dy2 = dy << 1;

            int d = dy2 - dx;

            int y = y0;
            for (int x = x0; x <= x1; x++)
            {
                buffer[y * bufferWidth + x] = c;

                if (d > 0)
                {
                    y++;
                    d -= dx2;
                }

                d += dy2;
            }
        }

        void drawTriangle(point p1, point p2, point p3, HSPD::colour c)
        {
            if (p1.x > bufferWidth || p1.x < 0 || p1.y > bufferHeight || p1.y < 0 ||
                p2.x > bufferWidth || p2.x < 0 || p2.y > bufferHeight || p2.y < 0 ||
                p3.x > bufferWidth || p3.x < 0 || p3.y > bufferHeight || p3.y < 0)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"Trying to draw outside of the window", __LINE__);
                return;
            }

            drawLine(p1, p2, c);
            drawLine(p2, p3, c);
            drawLine(p3, p1, c);
        }

        //src: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
        void fillTriangle(point p1, point p2, point p3, colour c)
        {
            if (p1.x > bufferWidth || p1.x < 0 || p1.y > bufferHeight || p1.y < 0 ||
                p2.x > bufferWidth || p2.x < 0 || p2.y > bufferHeight || p2.y < 0 ||
                p3.x > bufferWidth || p3.x < 0 || p3.y > bufferHeight || p3.y < 0)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"Trying to draw outside of the window", __LINE__);
                return;
            }

            int maxX = std::max(p1.x, std::max(p2.x, p3.x));
            int minX = std::min(p1.x, std::min(p2.x, p3.x));
            int maxY = std::max(p1.y, std::max(p2.y, p3.y));
            int minY = std::min(p1.y, std::min(p2.y, p3.y));


            /* spanning vectors of edge (v1,v2) and (v1,v3) */
            point vs1 = point{ p2.x - p1.x, p2.y - p1.y };
            point vs2 = point{ p3.x - p1.x, p3.y - p1.y };

            for (int x = minX; x <= maxX; x++)
            {
                for (int y = minY; y <= maxY; y++)
                {
                    point q = point{ x - p1.x, y - p1.y };

                    float s = float(q.x * vs2.y - vs2.x * q.y) / float(vs1.x * vs2.y - vs2.x * vs1.y);
                    float t = float(vs1.x * q.y - q.x * vs1.y) / float(vs1.x * vs2.y - vs2.x * vs1.y);

                    if (s >= 0 && t >= 0 && s + t <= 1)
                    { /* inside triangle */
                        buffer[y * bufferWidth + x] = c;
                    }
                }
            }

        }

        void fill(point startPos, point endPos, colour c)
        {
            if (startPos.x > bufferWidth || startPos.x < 0 || startPos.y > bufferHeight || startPos.y < 0 ||
                endPos.x > bufferWidth || endPos.x < 0 || endPos.y > bufferHeight || endPos.y < 0)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"Trying to draw outside of the window", __LINE__);
                return;
            }

            for (int y = startPos.y; y < endPos.y; y++)
                for (int x = startPos.x; x < endPos.y; x++)
                    buffer[y * bufferWidth + x] = c;
        }

        void draw(point pos, colour c)
        {
            if (pos.x > bufferWidth || pos.x < 0)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"Trying to draw outside of the window", __LINE__);
                return;
            }
            buffer[pos.y * bufferWidth + pos.x] = c;
        }

        void cls(colour c = black)
        {
            renderTarget->Clear();
            for (int pos = 0; pos < bufferWidth * bufferHeight; pos++)
                buffer[pos] = HSPD::black;
        }

        //NOTE: anything not releated to rendering object sprites are through the render function(beginDraw should be active and for render to take effect after it you must call endDraw)
        void render()
        {
            screenObj.destroySprite();
            screenObj.loadSpriteFromMemory(buffer, bufferWidth, bufferHeight);

            if (buffer != nullptr && screenObj.getSpriteBitmap() != nullptr)
            {
                renderTarget->DrawBitmap(screenObj.getSpriteBitmap(), D2D1::RectF(0, 0, (float)screenObj.getSpriteBitmap()->GetSize().width * (float)screenObj.resizeFactorX / 100,
                    (float)screenObj.getSpriteBitmap()->GetSize().height * (float)screenObj.resizeFactorY / 100), screenObj.spriteOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                    D2D1::RectF(0, 0, screenObj.getSpriteBitmap()->GetSize().width, screenObj.getSpriteBitmap()->GetSize().height));
            }
        }

        void beginDraw()
        {
            renderTarget->BeginDraw();
        }
        HRESULT endDraw()
        {
            HRESULT errorCode = renderTarget->EndDraw();
            if (errorCode != S_OK && mainConsole != nullptr)
                mainConsole->error(L"EndDraw failed. Error Code: " + std::to_wstring(errorCode), __LINE__);
            return errorCode;
        }
        void drawObjectSprite(object obj, point pos)
        {
            renderTarget->DrawBitmap(obj.getSpriteBitmap(), D2D1::RectF(pos.x, pos.y, pos.x + (float)obj.getSpriteBitmap()->GetSize().width * (float)obj.resizeFactorX / 100,
                pos.y + (float)obj.getSpriteBitmap()->GetSize().height * (float)obj.resizeFactorY / 100), obj.spriteOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                D2D1::RectF(0, 0, obj.getSpriteBitmap()->GetSize().width, obj.getSpriteBitmap()->GetSize().height));
        }
        void drawObjectSpriteRect(object obj, point pos, point startImg, point endImg)
        {
            if (obj.getSpriteBitmap() != nullptr)
                renderTarget->DrawBitmap(obj.getSpriteBitmap(),
                    D2D1::RectF(pos.x, pos.y, (startImg.x - endImg.x) * (float)obj.resizeFactorX / 100 + pos.x,
                        (float)(startImg.y - endImg.y) * (float)obj.resizeFactorY / 100 + pos.y), obj.spriteOpacity,
                    D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(startImg.x, startImg.y, endImg.x, endImg.y));
        }

        ID2D1RenderTarget* getRenderTarget()
        {
            return renderTarget;
        }

        int getBufferWidth()
        {
            return bufferWidth;
        }
        int getBufferHeight()
        {
            return bufferHeight;
        }
        HSPD::colour* getBuffer()
        {
            return buffer;
        }

        void destroyGfx2D()
        {
            if (buffer != nullptr)
                delete[] buffer;
            renderTarget->Release();
            factory->Release();
        }

        int initGfx2D(int w, int h, HWND hWnd) { return initGfx2D(w, h, hWnd, nullptr); }
        int initGfx2D(int w, int h, HWND hWnd, console* c, bool createBuffer = true)
        {
            bufferWidth = w;
            bufferHeight = h;
            mainConsole = c;
            if (createBuffer)
            {
                buffer = new HSPD::colour[w * h];
                for (int pos = 0; pos < w * h; pos++)
                    buffer[pos] = HSPD::black;
            }
            if (D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory) != S_OK)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"couldnt create D2D1 factory", __LINE__);
                return 1;
            }
            if (factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(w, h)), &renderTarget) != S_OK)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"couldnt create D2D1 render target", __LINE__);
                return 1;
            }
            screenObj.initializeObject(renderTarget);
            if (mainConsole != nullptr)
                screenObj.loadDebugConsole(c);
            if (mainConsole != nullptr)
                mainConsole->write(L"Successfully initialized Gfx2D", mainConsole->FORE_DARKGREEN, 0, 0, true);

            renderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
            renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

            renderTarget->BeginDraw();
            renderTarget->EndDraw();

            return 0;
        }
    };

    /////////////////////////////////////////////
    /// WINDOW RELEATED /////////////////////////
    /////////////////////////////////////////////

    struct debugCommand
    {
        std::string command;
        int argsc = 0;
        std::string argv[126];

        uint8_t isTriggered = { 0 };
    };

    class window
    {
        int screenWidth, screenHeight;
        std::wstring winTitle;
        bool customWC;
        bool customIcon;
        bool customCursor;
        bool debug;
        std::string commandStr;
        int keys[512];

        HMODULE hIn = GetModuleHandleW(NULL);
        HWND mainWindowHandle;
        MSG msg;
        WNDCLASSEX wc;
        HDC mainWindowHDC;

        console* mainConsole;
        debugCommand customCommands[126];
        int n_customCommands;

        std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
        float elapsedTime = 0;

        static LRESULT CALLBACK winProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            window* pThis = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (!pThis && message == WM_CREATE)
            {
                CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
                pThis = reinterpret_cast<window*>(pCreate->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            }

            if (pThis)
                return pThis->mainLoop(hwnd, message, wParam, lParam);
            else
                return DefWindowProc(hwnd, message, wParam, lParam);
        }

        LRESULT mainLoop(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            switch (message)
            {
            case WM_SIZE:
                if (LOWORD(lParam) != 0)
                    screenWidth = LOWORD(lParam);
                if (HIWORD(lParam) != 0)
                    screenHeight = HIWORD(lParam);
                break;
            case WM_CLOSE:
                DestroyWindow(hwnd);
                mainConsole->destroyConsole();
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            }

            if (debug)
            {
                std::wstring clearLine;
                for (int i = 0; i < mainConsole->getConsoleWidth(); i++)
                    clearLine += L' ';
                mainConsole->write(clearLine, mainConsole->BACK_BLACK, 0, 0, false);

                PROCESS_MEMORY_COUNTERS_EX pmc = { 0 };
                std::wstring debugStr = L"";
                if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
                {
                    SIZE_T virtualMemUsed = pmc.PrivateUsage;

                    MEMORYSTATUSEX memInfo = { 0 };
                    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

                    if (GlobalMemoryStatusEx(&memInfo))
                    {
                        SIZE_T physcalMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
                        debugStr = L"FPS: " + std::to_wstring(int(1.0 / elapsedTime)) + L" | USED VIRTUAL MEMORY: " + std::to_wstring(float(virtualMemUsed / 8000000.0f)) + L" MB | USED PHYSICAL MEMORY: " + std::to_wstring(float(physcalMemUsed / 8000000.0f)) + L" MB | COMMAND: ";
                    }
                    else
                        debugStr = L"debug functions failed";
                    mainConsole->write(debugStr, mainConsole->FORE_GREEN, 0, 0, false);
                }
                
                if (GetConsoleWindow() == GetForegroundWindow())
                {
                    for (int key = 0; key < 126; key++)
                    {
                        if (GetAsyncKeyState(key) < 0 && keys[key] == false)
                        {
                            if (key == 0x0020)
                                commandStr += ' ';
                            if (key == 0x000D)
                            {
                                commandStr += ' ';
                                int args = 0;
                                std::string argument;
                                bool exitBool = false;
                                for (int i = 0; i < commandStr.length(); i++)
                                {
                                    if (commandStr[i] == ' ' || commandStr[i] == '\0')
                                    {
                                        if (exitBool)
                                        {
                                            if (argument != "\0")
                                                for (int j = 1; j < argument.length(); j++)
                                                    if (!std::isdigit(argument[j]))
                                                        exit(0);
                                            exit(std::stoi(argument));
                                            exitBool = false;
                                        }
                                        if (argument == "exit")
                                            exitBool = true;

                                        for (int command = 0; command < n_customCommands; command++)
                                        {
                                            if (customCommands[command].isTriggered == 2)
                                            {
                                                customCommands[command].argv[args - 1] = argument;
                                                customCommands[command].argsc++;
                                                customCommands[command].isTriggered = 1;
                                            }
                                            if (customCommands[command].command == argument)
                                                customCommands[command].isTriggered = 2;
                                        }
                                        
                                        args++;
                                        argument = "";
                                    }
                                    argument += commandStr[i];
                                }
                                commandStr = "";
                            }
                            if (key == 0x0008)
                            {
                                if (!commandStr.empty())
                                    commandStr.erase(std::prev(commandStr.end()));
                            }
                            else if (key > 64 && key < 91)
                                commandStr += char(key + 32);
                            else if (key > 32 && key < 64)
                                commandStr += (char)key;
                          
                            keys[key] = true;
                        }
                        if (GetAsyncKeyState(key) == 0 && keys[key])
                            keys[key] = false;
                    }
                }
                mainConsole->writeAscii(commandStr, mainConsole->FORE_GREEN, debugStr.length(), 0, false);

                std::wstring seperator;
                for (int i = 0; i < mainConsole->getConsoleWidth(); i++)
                    seperator += L'_';
                mainConsole->write(seperator, mainConsole->FORE_GREEN, 0, 1, false);
                mainConsole->writeToConsoleBuffer();
            }
            
            return DefWindowProcW(hwnd, message, wParam, lParam);
        }
    public:

        debugCommand getCommandOutput(std::string command)
        {
            debugCommand o;
            for (int i = 0; i < n_customCommands; i++)
                if (customCommands[i].command == command)
                {
                    o = customCommands[i];
                    customCommands[i].isTriggered = 0;
                }
            return o;
        }

        void addDebugCommand(std::string command)
        {
            customCommands[n_customCommands].command = command;
            n_customCommands++;
        }

        bool getKeyState(int key, int wantedKeyState)
        {
            if (keys[key] == wantedKeyState)
                return true;
            else
                return false;
        }

        int getScreenWidth()
        {
            return screenWidth;
        }
        int getScreenHeight()
        {
            return screenHeight;
        }

        HWND getScreenHWND()
        {
            return mainWindowHandle;
        }

        HDC getDC()
        {
            return mainWindowHDC;
        }

        MSG getCurrentMessage()
        {
            return msg;
        }

        void setNewWindowTitle(std::wstring newTitle)
        {
            if (newTitle == L"\0")
                if (mainConsole != nullptr)
                    mainConsole->error(L"Cannot set a NULL string as a window title", __LINE__);
            winTitle = newTitle;
            SetWindowTextW(mainWindowHandle, winTitle.c_str());
        }

        //https://stackoverflow.com/questions/2382464/win32-full-screen-and-hiding-taskbar
        int enterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits = 8, int refreshRate = 60)
        {
            DEVMODE fullscreenSettings = { 0 };
            int isChangeSuccessful;
            RECT windowBoundary = { 0 };

            EnumDisplaySettings(NULL, 0, &fullscreenSettings);
            fullscreenSettings.dmPelsWidth = fullscreenWidth;
            fullscreenSettings.dmPelsHeight = fullscreenHeight;
            fullscreenSettings.dmBitsPerPel = colourBits;
            fullscreenSettings.dmDisplayFrequency = refreshRate;
            fullscreenSettings.dmFields = DM_PELSWIDTH |
                DM_PELSHEIGHT |
                DM_BITSPERPEL |
                DM_DISPLAYFREQUENCY;

            SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
            SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
            isChangeSuccessful = ChangeDisplaySettingsW(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
            ShowWindow(hwnd, SW_MAXIMIZE);

            return isChangeSuccessful;
        }

        void loadCustomWindowCursor(std::wstring cursorPath)
        {
            wc.hCursor = (HCURSOR)LoadImageW(hIn, cursorPath.c_str(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            if (mainConsole != nullptr)
            {
                if (GetLastError() != 0)
                    mainConsole->error(L"Couldnt successfully load the custom window cursor. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
                else
                    mainConsole->consoleLog(L"Successfully loaded custom window cursor. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
            }
            customCursor = true;
        }

        void loadCustomWindowIcon(std::wstring iconPath)
        {
            wc.hIcon = (HICON)LoadImageW(NULL, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            wc.hIconSm = (HICON)LoadImageW(NULL, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            if (mainConsole != nullptr)
            {
                if (GetLastError() != 0)
                    mainConsole->error(L"Couldnt successfully load the custom window icon. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
                else
                    mainConsole->consoleLog(L"Successfully loaded custom window icon. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
            }
            customIcon = true;
        }

        void loadCustomWindowCursorFromResource(int resourceName)
        {
            wc.hCursor = LoadCursorW(hIn, MAKEINTRESOURCEW(resourceName));
            if (mainConsole != nullptr)
            {
                if (GetLastError() != 0)
                    mainConsole->error(L"Couldnt successfully load the custom window cursor. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
                else
                    mainConsole->consoleLog(L"Successfully loaded custom window cursor. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
            }
            customCursor = true;
        }

        void loadCustomWindowIconFromResource(int resourceName)
        {
            wc.hIcon = (HICON)LoadImageW(hIn, MAKEINTRESOURCEW(resourceName), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
            wc.hIconSm = (HICON)LoadImageW(hIn, MAKEINTRESOURCEW(resourceName), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
            if (mainConsole != nullptr)
            {
                if (GetLastError() != 0)
                    mainConsole->error(L"Couldnt successfully load the custom window icon. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
                else
                    mainConsole->consoleLog(L"Successfully loaded custom window icon. Function exited with code " + std::to_wstring(GetLastError()), __LINE__);
            }
            customIcon = true;
        }

        int setCustomWindowClass(std::wstring title, LPCWSTR icon, LPCWSTR cursor, short int defaultBackgroundColour)
        {
            customWC = true;
            winTitle = title;

            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = winProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = hIn;
            wc.hIcon = LoadIconW(NULL, icon);
            wc.hCursor = LoadCursorW(NULL, cursor);
            wc.hbrBackground = (HBRUSH)(defaultBackgroundColour);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = title.c_str();
            wc.hIconSm = LoadIconW(NULL, icon);

            if (!RegisterClassEx(&wc))
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"Couldnt register HMODULE", __LINE__);
                return 1;
            }
            return 0;
        }

        int loadConsole(console* c)
        {
            mainConsole = c;
            return 0;
        }

        virtual void setup() = 0;
        virtual void winMain(float elapsedTime) = 0;

        int constructWindow(int w, int h, std::wstring title, bool debugMode, int maxFPS = 60)
        {
            //make window
            screenWidth = w;
            screenHeight = h;
            debug = debugMode;

            if (!customWC)
            {
                winTitle = title;

                wc.cbSize = sizeof(WNDCLASSEX);
                wc.style = CS_HREDRAW | CS_VREDRAW;
                wc.lpfnWndProc = winProc;
                wc.cbClsExtra = 0;
                wc.cbWndExtra = 0;
                wc.hInstance = hIn;
                if (!customIcon)
                {
                    wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
                    wc.hIconSm = LoadIconW(NULL, IDI_APPLICATION);
                }
                if (!customCursor)
                    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
                wc.hbrBackground = (HBRUSH)(COLOR_WINDOW - 3);
                wc.lpszMenuName = NULL;
                wc.lpszClassName = title.c_str();

                if (!RegisterClassExW(&wc))
                    if (mainConsole != nullptr)
                        mainConsole->error(L"Couldnt register HMODULE", __LINE__);
            }
            mainWindowHandle = CreateWindowExW(0, title.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, w, h, NULL, NULL, hIn, NULL);
            if (mainWindowHandle == NULL)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"No hwnd found", __LINE__);
                return 1;
            }

            mainWindowHDC = GetWindowDC(0);
            if (mainWindowHDC == NULL)
            {
                if (mainConsole != nullptr)
                    mainConsole->error(L"Couldnt set window DC", __LINE__);
                return 1;
            }

            ShowWindow(mainWindowHandle, SW_SHOW);
            SetFocus(mainWindowHandle);

            if (!debugMode)
                if (FreeConsole() == 0)
                {
                    if (mainConsole != nullptr)
                        mainConsole->error(L"FreeConsole error", __LINE__);
                    return 1;
                }

            SetWindowLongPtrW(mainWindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

            SetTimer(mainWindowHandle, 1, (1000 / maxFPS), NULL);

            setup();
            if (mainConsole != nullptr)
                mainConsole->write(L"The program setup ran", mainConsole->FORE_DARKGREEN, 0, 0, true);

            for (int i = 0; i < 256; i++)
                keys[i] = KEY_NOT_TOUCHED;

            bool running = true;
            while (running)
            {
                if (mainWindowHandle == GetFocus())
                {
                    for (int i = 0; i < 256; i++)
                    {
                        if (GetAsyncKeyState(i) & 0x8000)
                        {
                            keys[i] = KEY_DOWN;
                            keys[i + 256] = KEY_DOWN;
                        }
                        else if (!GetAsyncKeyState(i) && keys[i + 256] == KEY_DOWN)
                        {
                            keys[i] = KEY_RELEASED;
                            keys[i + 256] = KEY_RELEASED;
                        }
                        else
                        {
                            keys[i] = KEY_NOT_TOUCHED;
                            keys[i + 256] = KEY_NOT_TOUCHED;
                        }
                            
                    }
                }
                tp2 = std::chrono::system_clock::now();
                winMain(elapsedTime);
                std::chrono::duration<float> d_elapsedTime = tp2 - tp1;
                tp1 = tp2;
                elapsedTime = d_elapsedTime.count();
                if (PeekMessageW(&msg, mainWindowHandle, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                    if (msg.message == WM_QUIT)
                        running = false;
                }
            }
            return 0;
        }
    };
}