
#pragma once


#include "System.hpp"
#include <string>


//-------------------------------------------------------------------
// アプリケーション ユーティリティ
//-------------------------------------------------------------------
class Application
{
public:
    // 初期化
    static void Init(HWND hWnd);

    // 保存ダイアログ
    static bool GetFileNameForSave(std::string& out, const char* filterText);

    // 開くダイアログ
    static bool GetFileNameForLoad(std::string& out, const char* filterText);

    // ディレクトリ選択ダイアログ
    static bool GetDirectoryName(std::string& out);

    // 実行ファイルのフルパスを取得
    static bool GetAppFileFullPath(std::string& out);

    // 実行ファイルのディレクトリを取得
    static bool GetAppFileDirectory(std::string& out);

    // 外部プロセスを実行
    static bool ExecuteProcess(const std::string& cmd, int& exitCode, bool hide  = true);

private:
    static HWND s_hWnd;
};

