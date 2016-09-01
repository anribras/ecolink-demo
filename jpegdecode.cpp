/**
 * @file paintimage.cpp
 * @brief
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-07-12
 */
#ifndef _JPEG_DECODE_C
#define _JPEG_DECODE_C

#include "jpegdecode.h"

#include "debug.h"

CJpegDecode::CJpegDecode()
{
    m_rgb_ptr = NULL;
    memset(&m_cinfo,0,sizeof(m_cinfo));
}

CJpegDecode::~CJpegDecode()
{
    m_rgb_ptr = NULL;
    memset(&m_cinfo,0,sizeof(m_cinfo));
}



int CJpegDecode::decode_init()
{
    return 0;
}

int CJpegDecode::decode_from_file(const char* path)
{
    int height,width,level;
    FILE *fp = NULL;
    if((fp = fopen(path,"rb")) == NULL) {
        DBG("error:open %s failed \n",path);
        return -1;
    }
    m_cinfo.err=jpeg_std_error(&m_jerr);
    jpeg_CreateDecompress(&m_cinfo,70,sizeof(struct jpeg_decompress_struct));
    jpeg_stdio_src(&m_cinfo, fp);  //from file
    jpeg_read_header(&m_cinfo, TRUE);
    m_cinfo.out_color_space = JCS_EXT_BGRA;
    jpeg_start_decompress(&m_cinfo);
    height = m_cinfo.output_height;
    width = m_cinfo.output_width;
    level = m_cinfo.output_components;
    m_size = m_cinfo.output_width * m_cinfo.output_components *m_cinfo.output_height;
    DBG("decode mem width = %d ,height = %d , output_components = %d\n",width,height,level);
    if((m_rgb_ptr = (char *)malloc(m_size)) <= 0) {
        DBG("decode malloc error\n");
        return -1;
    }
    int y = 0;
    while(m_cinfo.output_scanline < height) {
        jpeg_read_scanlines(&m_cinfo, &m_rgb_ptr + y * height * level
                            , 1);
        y++;
    }

    return 0;

}

void CJpegDecode::release_rgb_buffer()
{
    if(m_rgb_ptr) {
        free(m_rgb_ptr);
        m_rgb_ptr = NULL;
    }

}

int CJpegDecode::decode_from_mem(const char* mem_ptr, unsigned long size)
{
    int height,width,level;
    m_cinfo.err=jpeg_std_error(&m_jerr);
    jpeg_CreateDecompress(&m_cinfo,70,sizeof(struct jpeg_decompress_struct));
    jpeg_mem_src(&m_cinfo,mem_ptr,size);  //from mem
    jpeg_read_header(&m_cinfo, TRUE);
    m_cinfo.out_color_space = JCS_EXT_RGBA;
    jpeg_start_decompress(&m_cinfo);
    height = m_cinfo.output_height;
    width = m_cinfo.output_width;
    level = m_cinfo.output_components;
    m_size =  height * width * level;
    if((m_rgb_ptr = (char *)malloc(m_size)) <= 0) {
        DBG("decode malloc error\n");
        return -1;
    }
    int y = 0;
    while(m_cinfo.output_scanline < height) {
        //DBG("y =%d m_cinfo.output_scanline = %d\n",y,m_cinfo.output_scanline);
        jpeg_read_scanlines(&m_cinfo, &m_rgb_ptr, 1);
        m_rgb_ptr += width * level;
        y++;
    }

    m_rgb_ptr -= m_size;

    return 0;

}

#endif

