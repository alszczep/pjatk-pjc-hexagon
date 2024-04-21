#ifndef PJC_HEXAGON_FILEMANAGER_H
#define PJC_HEXAGON_FILEMANAGER_H

#include <string>
#include <fstream>
#include <optional>

namespace FileManagement {
    const std::string SAVE_FILE_EXTENSION = ".save";
    const std::string RANKING_FILE_NAME = "ranking.txt";

    /**
     * Write methods will return true if an operation was successful
     * Read methods will return a null option if any exception occurs
     */
    class FileManager {
    private:
        static bool overwriteFile(const std::string &fileName, const std::string &newFileContent);

        static std::optional<std::string> loadFile(const std::string &fileName);

    public:
        static bool createSaveFile(const std::string &fileName, const std::string &saveData);

        static std::optional<std::string> loadSaveFile(const std::string &fileName);

        static bool updateRankingFile(const std::string &ranking);

        static std::optional<std::string> loadRankingFile();
    };
}

#endif //PJC_HEXAGON_FILEMANAGER_H
