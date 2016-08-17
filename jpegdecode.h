/**
 * @file paintimage.h
 * @brief
 * @author
 * @version v0.1
 * @date 2016-07-12
 */
#ifndef _JPEG_DECODE_H
#define _JPEG_DECODE_H

#include <QImage>
#include <QPixmap>
#include "/usr/local/libjpeg-turbo-1.4.2/freescale/include/jpeglib.h"

using namespace std;
class CJpegDecode
{
public:
    CJpegDecode();
    ~CJpegDecode();
    int decode_init();
    int decode_from_file(const char* path);
    int decode_from_mem(const char* mem_ptr, unsigned long size);
    void release_rgb_buffer();
    unsigned char* m_rgb_ptr;
    unsigned long m_size;
private:
    struct jpeg_error_mgr m_jerr;
    struct jpeg_decompress_struct m_cinfo;
protected:
};

#endif
