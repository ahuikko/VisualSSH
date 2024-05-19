#include "file_operations.h"
#include <windows.h>
#include <commdlg.h>
#include <Objbase.h> // For CoInitialize and CoUninitialize
#include <ShObjIdl.h> // For IFileDialog
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

FileOperations::FileOperations()
{
}

FileOperations::~FileOperations()
{
}

bool FileOperations::Browse(char* buffer) {
    IFileOpenDialog* pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

    if (SUCCEEDED(hr)) {
        DWORD dwOptions;
        hr = pfd->GetOptions(&dwOptions);
        if (SUCCEEDED(hr)) {
            hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
            if (SUCCEEDED(hr)) {
                hr = pfd->Show(NULL);
                if (SUCCEEDED(hr)) {
                    IShellItem* psi;
                    hr = pfd->GetResult(&psi);
                    if (SUCCEEDED(hr)) {
                        PWSTR pszPath = NULL;
                        hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                        if (SUCCEEDED(hr)) {
                            // Convert PWSTR to char*
                            WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, buffer, 256, NULL, NULL);
                            CoTaskMemFree(pszPath);
                            psi->Release();
                            pfd->Release();
                            return true;
                        }
                        psi->Release();
                    }
                }
            }
        }
        pfd->Release();
    }
    return false;
}

std::string FileOperations::fileToString(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (file) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str(); // Return the contents of the file as a std::string
    }
    else {
        std::cout << "\033[31mUnable to open " + filePath + "\033[0m" << std::endl;
        return "null";
    }
}

void FileOperations::stringToFile(const std::string& filePath, const std::string& data)
{
    std::ofstream file(filePath,std::ios::app);
    if (file) {
        file << data;
    }
    else {
        std::cout << "\033[31mUnable to open " + filePath + "\033[0m" << std::endl;
    }


}

