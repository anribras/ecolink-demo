/**
 * @file ringbuffer.c
 * @brief
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-05-07
 */
#ifndef __RING_BUFFER_C
#define __RING_BUFFER_C


#include "debug.h"
#include "ringbuffer.h"
#include "mainwindow.h"
#include <pthread.h>
#include <unistd.h>


#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
#define min(a, b) (((a) < (b)) ? (a) : (b))


/**
 * @brief ring_buffer_free
 * @param ring_buf
 */
void CRingBuf::ring_buffer_free(struct ring_buffer *ring_buf)
{
    if (ring_buf) {
        if (ring_buf->buffer) {
            free(ring_buf->buffer);
        }
        free(ring_buf);
        ring_buf = NULL;
    }
}

void* CRingBuf::thread_consume(void* pdata)
{
    while(1) {
            m_win->m_rbuf.ring_buffer_get();
        }
}

void CRingBuf::set_mainwindow_ptr(MainWindow * w)
{
    m_win = w;
}

CRingBuf::CRingBuf()
{
    lock = PTHREAD_MUTEX_INITIALIZER;
    cond = PTHREAD_COND_INITIALIZER;
    buf_ptr = NULL;
    m_win = NULL;
}

/** @brief ~CRingBuf  */
CRingBuf::~CRingBuf()
{
    ring_buffer_free(buf_ptr);
    buf_ptr = NULL;
    m_win = NULL;
}

unsigned char CRingBuf::__ring_buffer_len(const struct ring_buffer * ring_buf)
{
    return (ring_buf->in - ring_buf->out);
}
/**
 * @brief ring_buffer_len
 * @param ring_buf
 * @return
 */
unsigned char CRingBuf::ring_buffer_len(const struct ring_buffer *ring_buf)
{
    unsigned char len = 0;
    pthread_mutex_lock(&lock);
    len = __ring_buffer_len(ring_buf);
    pthread_mutex_unlock(&lock);
    return len;
}

/**
 * @brief ring_buffer_put_1
 * @param ring_buf
 * @param fun
 * @return
 */
unsigned char CRingBuf::ring_buffer_put_1(char * buffer, int buffersize)
{
    unsigned char ret = -1;
    unsigned char len = 0;
    unsigned long recv_len = 0;

    pthread_mutex_lock(&lock);
    unsigned int size = min(1, buf_ptr->size - (buf_ptr->in - buf_ptr->out));
    if(size == 0) {
        /*buffer is full ,size = 4,  out = 0 ,in = 4; wait here*/
        //DBG("buffer full now, wait for buffer free\n");
        pthread_cond_wait(&cond,&lock);
        size =  (buf_ptr->size - (buf_ptr->in - buf_ptr->out));
        //DBG("buffer is free now ,size = %d in = %lld out = %lld\n",size, buf_ptr->in,buf_ptr->out);
    }
    {
        /*buffer_get can work at the same time*/
        pthread_mutex_unlock(&lock);
        /*get image data from buffer and put into ring buffer*/
        memcpy(buf_ptr->buffer + (buf_ptr->in & (buf_ptr->size - 1)), buffer, buffersize);
        buf_ptr->avaluable_length_in_buffer[buf_ptr->in & (buf_ptr->size - 1)] =  buffersize;
        buf_ptr->in += size;
        //DBG("buf_ptr->in = %d\n",buf_ptr->in);
        /*save length*/
        pthread_cond_signal(&cond);
    }
    return 0;
}

unsigned char CRingBuf::ring_buffer_get()
{
    pthread_mutex_lock(&lock);
    unsigned int size = min(1, buf_ptr->in - buf_ptr->out);
    if(size == 0) {
        /*in = out ,no buffer is avaluable here*/
        //DBG(" no buffer is avaluable \n");
        pthread_cond_wait(&cond,&lock);
        size = min(1, buf_ptr->in - buf_ptr->out);
        //size =  buf_ptr->in - buf_ptr->out;
        //DBG("now buffer has something! size = %d in = %lld out = %lld\n",size,buf_ptr->in,buf_ptr->out);
    }
    {
        /*buffer put can work at same time*/
        pthread_mutex_unlock(&lock);
#if 0
        fun(image_display_is_need,
            NULL,
            (const char*)(buf_ptr->buffer + (buf_ptr->out & (buf_ptr->size - 1))),
            buf_ptr->avaluable_length_in_buffer[buf_ptr->out & (buf_ptr->size - 1)]);
        buf_ptr->out +=size;
        //DBG("buf_ptr->out = %lld\n", buf_ptr->out);
        //buf_ptr->out +=  (buf_ptr->in - buf_ptr->out  - 1);
#else
        m_win->paint_image(0,(unsigned char*)(buf_ptr->buffer + (buf_ptr->out & (buf_ptr->size - 1))),buf_ptr->avaluable_length_in_buffer[buf_ptr->out & (buf_ptr->size - 1)]);
#endif
        pthread_cond_signal(&cond);
    }
    return 0;
}

void CRingBuf::init()
{
    buf_ptr = (struct ring_buffer *)malloc(sizeof(struct ring_buffer));
    if(buf_ptr == NULL) {
        //DBG("crate struct ring_buffer error\n");
        return;
    }
    memset(buf_ptr,0,sizeof(struct ring_buffer));
    buf_ptr->buffer = (char (*)[BUFFFER_SIZE])malloc(BUFFFER_SIZE * RING_DEPTH);
    if(buf_ptr->buffer == NULL) {
        DBG("buffer malloc error\n");
        return;
    }
    buf_ptr->size = RING_DEPTH;
    buf_ptr->in = 0;
    buf_ptr->out = 0;
#if 0
	pthread_t pid;
    int ret;
    ret = pthread_create(&pid, NULL, (void*)&CRingBuf::thread_consume, NULL);
    if(ret != 0) {
        DBG("thread_consume create error\n");
        return ;
    }
#endif


}

#endif


