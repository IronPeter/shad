#include "compressor.h"
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <time.h>

template<class TIterator> void Test(const std::vector<std::string> &files) {
    TIterator it;
    std::string name = typeid(it).name();
    std::cout << "test for " << name << "\n";
    std::vector<TIterator> iterators;
    iterators.resize(files.size());
    for (size_t i = 0; i < files.size(); ++i) {
        it.LoadText(files[i]);
        std::string binaryFileName = "temp/" + files[i] + "." + name;
        it.SaveBinary(binaryFileName);
        iterators[i].LoadBinary(binaryFileName);
        {//sanity check
            TSimpleIterator a;
            TIterator b;
            b.LoadBinary(binaryFileName);
            a.LoadText(files[i]);
            while (a.Valid() && b.Valid()) {
                ui32 adoc = 0, apos = 0;
                ui32 bdoc = 0, bpos = 0;
                a.CurrentDocPos(adoc, apos);
                b.CurrentDocPos(bdoc, bpos);
                if (adoc != bdoc) {
                    throw std::logic_error("doc diverges");
                }
                if (apos != bpos) {
                    throw std::logic_error("pos diverges");
                }
                a.Next();
                b.Next();
            }
            if (a.Valid() || b.Valid()) {
                throw std::logic_error("doc count diverges");
            }
        }
        std::cout << files[i] << "\n";
    }
    ui32 hashedDoc = 0;
    ui32 hashedPos = 0;
    size_t count = 0;
    float timeBefore = clock();
    while (count < 1000 * 1000 * 100) {
        for (size_t i = 0; i < iterators.size(); ++i) {
            TIterator &it = iterators[i];
            it.Reset();
            while(it.Valid()) {
                ui32 doc = 0, pos = 0;
                it.CurrentDocPos(doc, pos);
                hashedDoc = hashedDoc * 5 +  doc;
                hashedPos = hashedPos * 5 +  pos;
                ++count;
                it.Next();
            }
        }
    }
    float timeAfter = clock();
    std::cout << "fetching of " << count << " positions\n";
    std::cout << "iterating over dataset took " << (timeAfter - timeBefore) / float(CLOCKS_PER_SEC) << " seconds and produce " << (hashedDoc ^ hashedPos) << " crc\n";
}

int main(int argc, const char *argv[]) {
    std::vector<std::string> files;
    while (std::cin) {
        std::string fileName;
        std::cin >> fileName;
        if (!fileName.empty()) {
            files.push_back(fileName);
        }
    }
    try {
        Test<TSimpleIterator>(files);
    } catch(std::logic_error &err) {
        std::cout << err.what() << "\n";
    }
};


