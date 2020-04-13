#pragma once



class ResultUtil
{
public:
    ResultUtil();

    // HRESULT 受け取り
    ResultUtil(HRESULT hr);

    ResultUtil(bool result, const std::string& text);

    ~ResultUtil();

    operator bool() const;

    const std::string& GetText() const;

private:
    bool        m_IsSucceeded;
    std::string m_Text;
};

