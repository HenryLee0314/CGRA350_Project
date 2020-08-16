#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"
#include "cgraFile.h"

cgraFile::cgraFile(const char* const fileAddress)
: m_fileAddress(fileAddress)
, m_size(0)
, m_file(nullptr)
{
    // CGRA_LOGD();
}

cgraFile::cgraFile(const std::string& fileName, const std::string& filePath)
: m_fileAddress(std::string(filePath + fileName).c_str())
, m_size(0)
, m_file(nullptr)
{
    // CGRA_LOGD();
}

cgraFile::~cgraFile() {
    // CGRA_LOGD();

    close();

    m_size = 0;
}

bool cgraFile::open() {
    // CGRA_LOGD("open file(%s)", m_fileAddress);

    do {
        if (m_fileAddress == nullptr) {
            CGRA_LOGE("file address is nullptr");
            break;
        }

        if (m_file != nullptr) {
            CGRA_LOGE("file point exist");
            break;
        }

        m_file = fopen(m_fileAddress, "r+");
        if (m_file == nullptr) {
            // CGRA_LOGD("file is not exist, try to create it");

            m_file = fopen(m_fileAddress, "w+");
            if (m_file == nullptr) {
                CGRA_LOGE("create file fail");
                break;
            }
            else {
                // CGRA_LOGD("create file successed, file size: m_size(0)");
                m_size = 0;
            }
        }
        else {
            fseek(m_file, 0L, SEEK_END);
            m_size = ftell(m_file);
            // CGRA_LOGD("file size: m_size(%ld)", m_size);
        }

        fseek(m_file, 0L, SEEK_SET);
        return true;
    } while (0);

    m_size = 0;
    return false;
}

bool cgraFile::close() {
    // CGRA_LOGD();
    if (m_file != nullptr) {
        fclose(m_file);
        m_file = nullptr;
    }
    return true;
}

bool cgraFile::read(std::string& content) {
    size_t readSize = 0;
    char* buff = nullptr;

    do {
        if (!open()) {
            CGRA_LOGE("can not open file");
            return 0;
        }

        CGRA_ACTIVITY_START(FILE);

        if (buff != nullptr) {
            CGRA_FREE(buff);
            buff = nullptr;
        }

        buff = (char*)CGRA_MALLOC((m_size + 1) * sizeof(char), FILE);
        if (buff == nullptr) {
            CGRA_LOGE("malloc buff error");
            break;
        }
        buff[m_size] = '\0';

        readSize = fread(buff, sizeof(char), m_size, m_file);
        if (static_cast<long>(readSize) == m_size && readSize >= 0 && m_size >= 0) {
            // // CGRA_LOGD("read success, size(%zu), buff(\n%s\n)", readSize, buff);
            content = std::string(buff, readSize);
            CGRA_FREE(buff);
        }
        else {
            CGRA_LOGE("read file error, m_size(%ld), readSize(%zu)", m_size, readSize);
            CGRA_FREE(buff);
            break;
        }

        close();
        CGRA_ACTIVITY_END(FILE);
        return true;
    } while (0);

    close();
    CGRA_ACTIVITY_END(FILE);
    return false;
}

bool cgraFile::write(const std::string& buff) {
    // CGRA_LOGD();
    do {
        if (!open()) {
            CGRA_LOGE("can not open file");
            return false;
        }

        if (!fwrite(buff.c_str(), buff.size(), 1, m_file)) {
            CGRA_LOGE("can not write file");
            break;
        }

        close();
        return true;
    } while (0);

    close();
    return false;
}
