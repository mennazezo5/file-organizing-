#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <cstdlib>
using namespace std;
namespace fs = filesystem;

map<string, string> extensions = {
    {".png", "imgs"}, {".jpg", "imgs"}, {".jpeg", "imgs"}, {".gif", "imgs"},
    {".pdf", "docs"}, {".md", "docs"}, {".doc", "docs"}, {".docx", "docs"},
    {".txt", "writings"},
    {".exe", "programs"}, {".msi", "programs"},
    {".mp3", "multimedia"}, {".mp4", "multimedia"}, {".avi", "multimedia"}, {".wav", "multimedia"},
};




//1
void organize_files(const fs::path& path) {
    map<string, vector<fs::path>> files_to_move;

    // التكرار عبر الملفات في الدليل
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            string extension = entry.path().extension().string();
            string folder = "others";

            // تحديد المجلد الهدف بناءً على امتداد الملف
            if (extensions.find(extension) != extensions.end()) {
                folder = extensions[extension];
            }

            // جمع الملفات لنقلها
            files_to_move[folder].push_back(entry.path());
        }
    }

    // إنشاء الأدلة ونقل الملفات
    for (const auto& pair : files_to_move) {
        fs::path target_dir = path / pair.first;
        fs::create_directory(target_dir);

        for (const auto& file : pair.second) {
            fs::path target = target_dir / file.filename();
            fs::rename(file, target);
        }
    }

    cout << "done!" << endl;
}








int main() {
    string folder_path;
    cout << "enter the path: ";
    getline(cin, folder_path);

    fs::path path(folder_path);
    if (!fs::exists(path) || !fs::is_directory(path)) {
        cerr << "file path not correct" << endl;
        return 1;
    }

    organize_files(path);

    // فتح المجلد في مستكشف الملفات
    string command = "explorer " + folder_path;
    system(command.c_str());

    return 0;
}