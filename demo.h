/**
 * @file demo.h
 * @brief 
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-08-12
 */
#ifndef __DEMO_H
#define __DEMO_H
void connect();
void disconnect();
void st_changed(int* st);
void data_gotten(int* mode ,char* buffer, int* size);
#endif	

