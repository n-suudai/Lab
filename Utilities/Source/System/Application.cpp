#include "Application.hpp"
#include "Path.hpp"
#include <shlobj.h>


// フォルダ選択ダイアログのコールバック関数
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    lParam;

    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        SendMessageA(hwnd, BFFM_SETSELECTIONA, (WPARAM)TRUE, lpData);
        break;

    case BFFM_VALIDATEFAILEDA:
        MessageBoxA(hwnd, (LPSTR)lParam, "無効なフォルダー名が入力されました", MB_OK);
        return 1;
    }
    return 0;
}


HWND Application::s_hWnd = NULL;

// 初期化
void Application::Init(HWND hWnd)
{
    s_hWnd = hWnd;
}

// 保存ダイアログ
bool Application::GetFileNameForSave(std::string& out, const char* filterText)
{
    static CHAR strFile[MAX_PATH], strCustom[256] = "Before files\0*.*\0\0";
    OPENFILENAMEA ofn = { 0 };

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = s_hWnd;
    ofn.lpstrFilter = filterText;
    ofn.lpstrCustomFilter = strCustom;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = strFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

    if (::GetSaveFileNameA(&ofn) == 0)
    {
        return false;
    }

    out = strFile;
    return true;
}

// 開くダイアログ
bool Application::GetFileNameForLoad(std::string& out, const char* filterText)
{
    static CHAR strFile[MAX_PATH], strCustom[256] = "Before files\0*.*\0\0";
    OPENFILENAMEA ofn = { 0 };

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = s_hWnd;
    ofn.lpstrFilter = filterText;
    ofn.lpstrCustomFilter = strCustom;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = strFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

    if (::GetOpenFileNameA(&ofn) == 0)
    {
        return false;
    }

    out = strFile;
    return true;
}

// ディレクトリ選択ダイアログ
bool Application::GetDirectoryName(std::string& out)
{
    std::string result = "";

    BROWSEINFOA bi = { 0 };
    LPITEMIDLIST idl = NULL;
    LPMALLOC g_pMalloc = NULL;

    std::string dirname = "";
    GetAppFileDirectory(dirname);

    char szTmp[MAX_PATH];
    sprintf_s(szTmp, "%s", dirname.c_str());

    SHGetMalloc(&g_pMalloc);

    bi.hwndOwner = s_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szTmp;
    bi.lpszTitle = "フォルダを選択してください";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_VALIDATE;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = (LPARAM)szTmp;
    bi.iImage = 0;

    //ダイアログを表示
    idl = SHBrowseForFolderA(&bi);
    if (idl != NULL)
    {
        if (SHGetPathFromIDListA(idl, szTmp) != FALSE)
        {
            result = szTmp;
        }
        //PIDLを解放する
        g_pMalloc->Free(idl);
    }

    g_pMalloc->Release();

    if (result != "")
    {
        out = result;
        return true;
    }

    return false;
}

// 実行ファイルのフルパスを取得
bool Application::GetAppFileFullPath(std::string& out)
{
    char Path[MAX_PATH + 1] = "";

    // 実行ファイルの完全パスを取得
    if (0 != GetModuleFileNameA(NULL, Path, MAX_PATH))
    {
        out = Path;
        return true;
    }
    return false;
}

// 実行ファイルのディレクトリを取得
bool Application::GetAppFileDirectory(std::string& out)
{
    if (GetAppFileFullPath(out))
    {
        out = Path::GetDirectoryName(out);
        return true;
    }
    return false;
}

// 外部プロセスを実行
bool Application::ExecuteProcess(const std::string& cmd, int& exitCode, bool hide)
{
    struct CMD_BUFF
    {
        CMD_BUFF(const std::string& cmd)
        {
            data = new char[cmd.size() + 1];
            strcpy_s(data, cmd.size() + 1, cmd.c_str());
        }

        ~CMD_BUFF()
        {
            delete[] data;
        }

        char* data;
    } cmdBuff(cmd);

    STARTUPINFOA si;        // スタートアップ情報
    PROCESS_INFORMATION pi; // プロセス情報

    //STARTUPINFO 構造体の内容を取得 
    GetStartupInfoA(&si);
    
    if (hide)
    {
        //見えない状態で起動させるには、
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
    }

    BOOL result = CreateProcessA(
        NULL,           // 実行可能モジュールの名前
        cmdBuff.data,   // コマンドラインの文字列
        NULL,           // セキュリティ記述子
        NULL,           // セキュリティ記述子
        FALSE,          // ハンドルの継承オプション
        NULL,           // 作成のフラグ 

                        //CREATE_NEW_PROCESS_GROUP  : 新たなプロセス
        NULL,           // 新しい環境ブロック
        NULL,           // カレントディレクトリの名前
        &si,            // スタートアップ情報
        &pi             // プロセス情報
    );

    if (result == 0)
    {
        return false;
    }

    //プロセスの終了を待機する
    DWORD code = 0;
    
    CloseHandle(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);
    
    // 終了コードを取得
    if (GetExitCodeProcess(pi.hProcess, &code) != 0)
    {
        exitCode = static_cast<int>(code);
    }
    
    CloseHandle(pi.hProcess);

    return true;
}


