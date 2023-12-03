#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>

class FileCreator{
    static bool createDirectory(const std::string& path);

    static void createAllParentDirectories(const std::string& path);

    static void createFilesInDirectory(const std::string& path,const std::string&fileName, int numberOfFiles);

    static bool isFileExists(const std::string& path);
public:
    static void createFolderAndFiles(const std::string& folderPath,const std::string&fileName,int numberOfFiles);
};

bool FileCreator::createDirectory(const std::string& path) {
    DWORD ftyp = GetFileAttributesA(path.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        // 文件夹不存在，尝试创建
        return CreateDirectoryA(path.c_str(), NULL);
    }
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        // 文件夹已存在
        return true;
    }
    // 路径存在，但不是文件夹
    return false;
}

void FileCreator::createAllParentDirectories(const std::string& path) {
    size_t pos = 0;
    std::string currentPath;
    while ((pos = path.find_first_of("\\/", pos + 1)) != std::string::npos) {
        currentPath = path.substr(0, pos);
        createDirectory(currentPath);
    }
}

void FileCreator::createFilesInDirectory(const std::string& path, const std::string& fileName, int numberOfFiles) {
    for (int i = 0; i < numberOfFiles; ++i) {
        std::ostringstream filePath;
        filePath << path << "\\" << fileName << i ; 

        if (isFileExists(filePath.str())) {
            continue; 
        }
        std::ofstream file(filePath.str());
        if (file.is_open()) {
            file.close();
        } else {
            std::cerr << "Unable to create file: " << filePath.str() << std::endl;
        }
    }
}

void FileCreator::createFolderAndFiles(const std::string& folderPath,const std::string&fileName,int numberOfFiles) {
    createAllParentDirectories(folderPath);
    if (createDirectory(folderPath)) {
        createFilesInDirectory(folderPath, fileName,numberOfFiles);
    } else {
        std::cerr << "Unable to create or find folder: " << folderPath << std::endl;
    }
}

bool FileCreator::isFileExists(const std::string& path) {
    DWORD fileAttr = GetFileAttributesA(path.c_str());
    return (fileAttr != INVALID_FILE_ATTRIBUTES &&
            !(fileAttr & FILE_ATTRIBUTE_DIRECTORY));
}