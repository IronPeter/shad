#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>

typedef unsigned int ui32;

class TSimpleIterator {
    typedef  std::vector<std::pair<ui32, ui32> > WordPosVector;
    WordPosVector Positions;
    WordPosVector::const_iterator  CurrentPosition;
public:
    void Reset() {
       CurrentPosition = Positions.begin();
    }
    void LoadText(const std::string &textFile) {
        Positions.clear();
        std::ifstream fp;
        fp.open(textFile.c_str());
        fp >> std::hex;
        while (fp) {
            ui32 doc = 0;
            ui32 pos = 0;
            fp >> doc >> pos;
            Positions.push_back(std::make_pair(doc, pos));
        }
        Reset();
    }
    void SaveBinary(const std::string &binaryFile) const {
        std::ofstream fp;
        fp.open(binaryFile.c_str());
        size_t size = Positions.size();
        fp.write((const char *)&size, sizeof(size));
        if (size != 0) {
            fp.write((const char *)&Positions[0], sizeof(Positions[0]) * size);
        }
    }
    void LoadBinary(const std::string &binaryFile) {
        Positions.clear();
        std::ifstream fp;
        fp.open(binaryFile.c_str());
        size_t size;
        if (fp.read((char *)&size, sizeof(size)) && size > 0) {
            Positions.resize(size);
            fp.read((char *)&Positions[0], sizeof(Positions[0]) * size);
        }
        Reset();
    }
    void CurrentDocPos(ui32 &doc, ui32 &pos) const {
        doc = CurrentPosition->first;
        pos = CurrentPosition->second;
    }
    bool Valid() const {
        return CurrentPosition != Positions.end();
    }
    void Next() {
        assert(CurrentPosition != Positions.end());
        ++CurrentPosition;
    }
};

