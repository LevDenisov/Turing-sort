#ifndef EXTERNSORT_INTERFACE_H
#define EXTERNSORT_INTERFACE_H

#include "tape.h"
#include <iostream>
#include <vector>

class Interface {
    uint RAM; // it's byte

    std::vector<std::unique_ptr<Tape>>
    createBlockSort(Tape &inputFile, uint blockSize, uint blockCount);

    void merge(Tape &a, Tape &b, const std::string &path);

  public:
    explicit Interface(uint RAM) : RAM(RAM) {}

    void Extern_sort(const std::string &path_in, const std::string &path_out);

    void show(Tape &tape);
    bool isSorted(Tape &tape);
};

#endif // EXTERNSORT_INTERFACE_H
