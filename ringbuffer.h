/**
 * @file ringbuffer.h
 * @brief
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-05-09
 */

#ifndef __RING_BUF_H
#define __RING_BUF_H


#include "jpegdecode.h"

/***********************************************************************
    DEFINITION FOR RING BUFFER
***********************************************************************/
#define BUFFFER_SIZE  500*1024
#define RING_DEPTH 2

typedef struct ring_buffer {
    //unsigned char (*buffer)[BUFFFER_SIZE];
    char (*buffer)[BUFFFER_SIZE];
    long avaluable_length_in_buffer[RING_DEPTH];
    unsigned int size;
    unsigned long long in;
    unsigned long long out;
} ring_buffer;


typedef int (*use_image_data_from_buffer)(const char*,unsigned long);

class MainWindow;

class CRingBuf
{

public:
    CRingBuf();
    ~CRingBuf();
    void ring_buffer_free(struct ring_buffer *ring_buf);
    void init();
    unsigned char ring_buffer_len(const struct ring_buffer *ring_buf);
    unsigned char ring_buffer_put_1(char * buffer, int buffersize);
    unsigned char ring_buffer_get();
    void set_mainwindow_ptr(MainWindow* w);
    struct ring_buffer* buf_ptr ;
    CJpegDecode m_decoder;
private:
    MainWindow * m_win;
    unsigned char __ring_buffer_len(const struct ring_buffer * ring_buf);
    void* thread_consume(void* pdata);
    pthread_mutex_t lock ;
    pthread_cond_t cond ;

};


#endif

