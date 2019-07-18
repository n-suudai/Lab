
#pragma once


struct Position2D
{
    s32 x;
    s32 y;
};


struct Size2D
{
    u32 width;
    u32 height;
};


class IApp
{
public:
    virtual ~IApp() = default;

    virtual bool IsLoop() const = 0;

    virtual void Idle() = 0;

    virtual void PostQuit() = 0;

    virtual Size2D GetClientSize() const = 0;

    virtual void* GetWindowHandle() const = 0;

    virtual void ShowMessageBox(const std::string& message, const std::string& caption) = 0;
};


bool CreateApp(const Size2D& clientSize, const std::string& title, IApp ** ppOut);

void AppMain(int argc, char* argv[]);

