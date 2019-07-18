
#pragma once


class AppWin : public IApp
{
public:
    AppWin();

    virtual ~AppWin();

    bool Init(const Size2D& clientSize, const std::string& title);

    void Term();

    virtual bool IsLoop() const override final;

    virtual void Idle() override;

    virtual void PostQuit() override final;

    virtual Size2D GetClientSize() const override final;

    virtual void* GetWindowHandle() const override final;

    virtual void ShowMessageBox(const std::string& message, const std::string& caption) override final;

private:
    virtual void OnInitEnd() {}

    virtual void OnTermRequested() {}

    virtual void OnIdle() {}

    virtual void OnResize(const Size2D& /*newSize*/) {}

    virtual void OnKeyDown(KEY_CODE /*key*/) {}

    virtual void OnKeyUp(KEY_CODE /*key*/) {}

    virtual void OnMouseKeyDown(const Position2D& /*position*/, MOUSE_BUTTON /*button*/) {}

    virtual void OnMouseKeyUp(const Position2D& /*position*/, MOUSE_BUTTON /*button*/) {}
    
    virtual void OnMouseWheel(const Position2D& /*position*/, s32 /*delta*/) {}

    static LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

protected:
    virtual LRESULT CALLBACK WindowProcedureInstance(HWND, UINT, WPARAM, LPARAM);

protected:
    HWND         m_hWnd;
    HINSTANCE    m_hInstance;
    Size2D       m_ClientSize;
    Size2D       m_WindowSize;
};


