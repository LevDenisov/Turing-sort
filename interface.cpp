#include "interface.h"

std::vector<std::unique_ptr<Tape>>
Interface::createBlockSort(Tape &inputFile, uint blockSize, uint blockCount) {
    std::vector<T> arr(blockSize);
    std::vector<std::unique_ptr<Tape>> tmpFiles;

    uint i, j;

    for (i = 0; i < blockCount; i++) {
        for (j = 0; j < blockSize && !inputFile.isEnd();
             j++, inputFile.moveRight()) {
            arr[j] = inputFile.getData();
        }

        std::sort(arr.begin(), arr.begin() + j);

        tmpFiles.emplace_back(std::make_unique<Tape>(
            j * sizeof(T), "tmpFile" + std::to_string(i + 1)));

        for (uint t = 0; t < j; t++, tmpFiles[i]->moveRight()) {
            tmpFiles[i]->writeData(arr[t]);
        }

        tmpFiles[i]->headBegin();
    }

    return tmpFiles;
}

void Interface::merge(Tape &a, Tape &b, const std::string &path) {
    Tape result(a.size() + b.size(), path);
    a.headBegin();
    b.headBegin();

    for (uint i = 0; i < result.count(); i++) {
        if (!a.isEnd() && (b.isEnd() || b.getData() > a.getData())) {
            result.writeData(a.getData());
            a.moveRight();
        } else {
            result.writeData(b.getData());
            b.moveRight();
        }

        result.moveRight();
    }
}

void Interface::Extern_sort(const std::string &path_in,
                            const std::string &path_out) {
    Tape inputFile(path_in);

    uint blockCount = inputFile.size() / RAM;
    uint blockSize = RAM / sizeof(T);

    std::vector<std::unique_ptr<Tape>> tmpFiles =
        createBlockSort(inputFile, blockSize, blockCount);

    for (uint i = 0; i + 1 < blockCount; i++) {
        Tape tape("tmpFilesMerge" + std::to_string(i));
        merge(tape, *tmpFiles[i], "tmpFilesMerge" + std::to_string(i + 1));

        tape.deleteTape();
    }

    Tape tape("tmpFilesMerge" + std::to_string(blockCount - 1));

    merge(tape, *tmpFiles[blockCount - 1], path_out);

    tape.deleteTape();

    for (auto &v : tmpFiles) {
        v->deleteTape();
    }
}

void Interface::show(Tape &tape) {
    tape.headBegin();

    while (!tape.isEnd()) {
        std::cout << tape.getData() << ' ';
        tape.moveRight();
    }
    std::cout << std::endl;

    tape.headBegin();
}

bool Interface::isSorted(Tape &tape) {
    if (tape.count() <= 1) {
        return true;
    }

    tape.headBegin();

    T prevValue = tape.getData();

    tape.moveRight();

    while (!tape.isEnd()) {
        if (tape.getData() < prevValue) {
            return false;
        }

        prevValue = tape.getData();
        tape.moveRight();
    }

    return true;
}