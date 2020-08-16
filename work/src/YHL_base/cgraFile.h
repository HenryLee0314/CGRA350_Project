#ifndef CGRA_FILE_H
#define CGRA_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

class cgraFile {
public:
    cgraFile(const char* const fileAddress);
    cgraFile(const std::string& fileName, const std::string& filePath);

    ~cgraFile();

    bool read(std::string& content);

    bool write(const std::string& buff);

private:
    bool open();

    bool close();

private:
    const char* const m_fileAddress;
    long m_size;
    FILE* m_file;
};

#endif // CGRA_FILE_H