#ifndef CLASS_FLX
#define CLASS_FLX

#include <string>
#include <fstream>
#include <vector>

class FLXFile
{
private:

    struct Record
    {
        uint32_t offset;
        uint32_t length;
        std::vector<uint8_t> data;
    };

    std::ifstream m_File;

    std::string m_HeaderTitle;
    uint32_t m_HeaderMagic1;
    uint32_t m_HeaderCount;
    uint32_t m_HeaderMagic2;
    std::vector<uint8_t> m_HeaderOther;

    std::vector<Record> m_Records;

    std::string bytes_to_str(int bytes);
    uint32_t bytes_to_uint32();
    uint8_t bytes_to_uint8();

public:
    FLXFile();
    ~FLXFile();

    bool open(std::string filename);
    std::string getTitle() { return m_HeaderTitle;}
    int getRecordCount() { return int(m_Records.size());}
    std::vector<uint8_t> getRecord(int index);

    // debug
    void show();
};

#endif // CLASS_FLX
