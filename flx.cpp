#include "flx.hpp"


#include <iostream>
#include <iomanip>
/*
  header  = title, magic1, count, magic2, d1..d9
  title   = $50 characters (optionnal, filled with 00s)
  magic1  = longint (seems to be always $FFFF1A00)
  count   = longint (number of object in table, including empty objects)
  magic2  = longint (seems to be always $000000CC)
  d1..d9 = longints (often set to 0, but sometimes used, meaning?)
*/

FLXFile::FLXFile()
{

    m_HeaderCount = 0;
    m_HeaderMagic1 = 0;
}

FLXFile::~FLXFile()
{

}

bool FLXFile::open(std::string filename)
{
    if(m_File.is_open()) std::cout << filename << " is already open...\n";
    m_File.open(filename.c_str(), std::ios::binary|std::ios::in);
    if(!m_File.is_open()) return false;

    // read in header
    m_HeaderTitle = bytes_to_str(0x50);
    m_HeaderMagic1 = bytes_to_uint32();
    m_HeaderCount = bytes_to_uint32();
    m_HeaderMagic2 = bytes_to_uint32();
    for(int i = 0; i <= 8; i++) m_HeaderOther.push_back(bytes_to_uint8());

    // read in record table
    for(int i = 0; i < m_HeaderCount; i++)
    {
        Record newrecord;
        newrecord.offset = bytes_to_uint32();
        newrecord.length = bytes_to_uint32();
        m_Records.push_back(newrecord);
    }

    m_File.close();
    return true;
}

std::string FLXFile::bytes_to_str(int bytes)
{
    bytes++;
    if(!m_File.is_open()) return "";
    char tc[bytes];
    m_File.get(tc, bytes);
    return std::string(tc);
}

uint32_t FLXFile::bytes_to_uint32()
{
    if(!m_File.is_open()) return 0;

    uint32_t val = 0;
    for(int i = 0; i < 4; i++)
    {
        unsigned char c = m_File.get();
        val += int(c) << (i*8);
    }

    return val;
}

uint8_t FLXFile::bytes_to_uint8()
{
    if(!m_File.is_open()) return 0;

    uint8_t val = 0;
    unsigned char c = m_File.get();
    val = int(c);

    return val;
}

void FLXFile::show()
{
    std::cout << "Title:" << m_HeaderTitle << std::endl;
    std::cout << "Magic1:" << std::hex << "0x" << int(m_HeaderMagic1) << std::endl;
    std::cout << "Object Count:" << std::dec << m_HeaderCount << std::endl;
    std::cout << "Magic2:" << std::hex << "0x" << int(m_HeaderMagic2) << std::endl;
    std::cout << "Other Data: ";
    for(int i = 0; i < int(m_HeaderOther.size()); i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << int(m_HeaderOther[i]) << " ";
        if(i == int(m_HeaderOther.size()-1) ) std::cout << std::endl;
    }
    std::cout << "Records:" << m_Records.size();


}
