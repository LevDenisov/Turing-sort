#include "tape.h"

Tape::Tape(const std::string &path)
    : file(path, std::ios::binary | std::ios::in | std::ios::out) {
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        filePath = path;

        sizeTapeCell = fileSize / sizeof(T);
    }
}

Tape::Tape(std::streampos fileSizeInBytes, const std::string &fileName)
    : file(fileName,
           std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc) {
    if (file.is_open()) {
        fileSize = fileSizeInBytes;

        const uint numBlocks = fileSizeInBytes / sizeof(T);

        filePath = fileName;

        sizeTapeCell = fileSize / sizeof(T);

        for (uint i = 0; i < numBlocks; i++) {
            T zeroByte = 0;
            file.write(reinterpret_cast<const char *>(&zeroByte),
                       sizeof(zeroByte));
        }

        headBegin();
    }
}

T Tape::getData() {
    T value;

    std::this_thread::sleep_for(
        std::chrono::seconds(config.get<uint>("getData")));
    file.read(reinterpret_cast<char *>(&value), sizeof(value));
    file.seekg(uint(file.tellg()) - BLOCK_SIZE);

    return value;
}

void Tape::writeData(T value) {
    std::this_thread::sleep_for(
        std::chrono::seconds(config.get<uint>("writeData")));
    file.write(reinterpret_cast<char *>(&value), sizeof(value));
    file.seekg(uint(file.tellg()) - BLOCK_SIZE);
}

void Tape::moveRight() {
    if (!isEnd()) {
        std::this_thread::sleep_for(
            std::chrono::seconds(config.get<uint>("move")));
        file.seekg(uint(file.tellg()) + BLOCK_SIZE);
    }
}

void Tape::moveLeft() {
    if (!isBegin()) {
        std::this_thread::sleep_for(
            std::chrono::seconds(config.get<uint>("move")));
        file.seekg(uint(file.tellg()) - BLOCK_SIZE);
    }
}

void Tape::headBegin() {
    while (!isBegin()) {
        moveLeft();
    }
}

void Tape::headEnd() {
    while (!isEnd()) {
        moveRight();
    }
}

bool Tape::isBegin() {
    if (file.tellg() == 0 || file.tellg() == -1) {
        return true;
    }

    return false;
}

bool Tape::isEnd() {
    if (file.tellg() == fileSize || file.tellg() == -1) {
        return true;
    }

    return false;
}

void Tape::deleteTape() {
    file.close();
    std::filesystem::remove(filePath);

    fileSize = 0;
    filePath = "";
}