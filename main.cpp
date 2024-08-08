#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;
namespace fs = filesystem;

map<string, string> extensions = {
    {".png", "imgs"}, {".jpg", "imgs"}, {".jpeg", "imgs"}, {".gif", "imgs"},
    {".pdf", "docs"}, {".md", "docs"}, {".doc", "docs"}, {".docx", "docs"},
    {".txt", "writings"},
    {".exe", "programs"}, {".msi", "programs"},
    {".mp3", "music"}, {".mp4", "multimedia"}, {".avi", "multimedia"}, {".wav", "music"},
};

void load_settings(const string& filename) {
    ifstream file(filename);
    string line, extension, folder;
    while (getline(file, line)) {
        istringstream iss(line);
        getline(iss, extension, ',');
        getline(iss, folder, ',');
        extensions [extension] = folder;
    }
}

void save_settings(const string& filename) {
    ofstream file(filename);
    for (const auto& pair : extensions) {
        file << pair.first << "," << pair.second << "\n";
    }
}

void edit_settings(const string& filename) {
    string extension, folder;
    while (true) {
        cout << "Enter file extension to customize (or 'done' to finish): ";
        cin >> extension;
        if (extension == "done") break;
           cout << "Enter folder name for " << extension << ": ";
           cin >> folder;
        extensions[extension] = folder;
    }
    save_settings(filename);
}

void organize_files(const fs::path& path) {
    map<string, vector<fs::path>> files_to_move;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
           string extension = entry.path().extension().string();
           string folder = "others";
            if (extensions.find(extension) != extensions.end()) {
                folder = extensions[extension];
            }
            files_to_move[folder].push_back(entry.path());
        }
    }

    for (const auto& pair : files_to_move) {
        fs::path target_dir = path / pair.first;
        fs::create_directory(target_dir);
        for (const auto& file : pair.second) {
            fs::path target = target_dir / file.filename();
            fs::rename(file, target);
        }
    }

   cout << "Files organized successfully!" <<  endl;
}

int main() {
   string choice;
    const string settings_filename = "settings.csv";

   cout << "Do you want to (1) edit settings or (2) organize a folder? Enter 1 or 2: ";
    cin >> choice;

    if (choice == "1") {
        edit_settings(settings_filename);
    } else if (choice == "2") {
       // load_settings(settings_filename);
        string folder_path;
          cout << "Enter the path to the folder to organize: ";
          cin >> folder_path;

        fs::path path(folder_path);
        if (!fs::exists(path) || !fs::is_directory(path)) {
            cerr << "Invalid folder path!" << endl;
            return 1;
        }

        organize_files(path);

        string command = "explorer " + folder_path;
        system(command.c_str());
    } else {
        cerr << "Invalid choice!" << endl;
    }

    return 0;
}