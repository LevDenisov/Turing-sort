#ifndef EXTERNSORT_TAPE_H
#define EXTERNSORT_TAPE_H

// If the magnetic head is on the very first cell, then this is considered the
// beginning of the tape. The tape has an additional cell indicating the end of
// the tape. The element in this cell is not defined. In this emulation, this
// means that if file.tellg() == fileSize, then the magnetic head is on the very
// last cell of the tape.

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

extern boost::property_tree::ptree config;

typedef int32_t T;

class Tape {
    std::fstream file;
    std::streampos fileSize;
    std::string filePath;

    uint sizeTapeCell;

    const uint BLOCK_SIZE = sizeof(T);

  public:
    explicit Tape(const std::string &path);

    Tape(std::streampos fileSizeInBytes, const std::string &fileName);

    Tape() : file(nullptr), fileSize(0), filePath(), sizeTapeCell(0) {}

    ~Tape() { file.close(); }

    T getData();

    void writeData(T value);

    std::streampos size() const { return fileSize; }

    void moveRight();

    void moveLeft();

    void headBegin();

    void headEnd();

    bool isBegin();

    bool isEnd();

    void deleteTape();

    uint count() { return sizeTapeCell; }
};

#endif // EXTERNSORT_TAPE_H
