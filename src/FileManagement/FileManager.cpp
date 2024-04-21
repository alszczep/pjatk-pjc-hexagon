#include "FileManager.h"

using namespace FileManagement;

bool FileManager::createSaveFile(const std::string &fileName, const std::string &saveData) {
    return overwriteFile(fileName + SAVE_FILE_EXTENSION, saveData);
}

std::optional<std::string> FileManager::loadSaveFile(const std::string &fileName) {
    return loadFile(fileName + SAVE_FILE_EXTENSION);
}

bool FileManager::updateRankingFile(const std::string &ranking) {
    return overwriteFile(RANKING_FILE_NAME, ranking);
}

std::optional<std::string> FileManager::loadRankingFile() {
    return loadFile(RANKING_FILE_NAME);
}

bool FileManager::overwriteFile(const std::string &fileName, const std::string &newFileContent) {
    try {
        std::ofstream stream(fileName, std::ios::trunc);
        stream << newFileContent;
        stream.close();
    } catch (const std::exception &) {
        return false;
    }

    return true;
}

std::optional<std::string> FileManager::loadFile(const std::string &fileName) {
    std::string fileContents;

    try {
        std::ifstream stream(fileName);
        std::string input;

        while (getline(stream, input)) {
            fileContents += input + '\n';
        }
    } catch (const std::exception &) {
        return std::nullopt;
    }

    return fileContents;
}
