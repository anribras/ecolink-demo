/**
 * @file debug.h
 * @brief
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-06-28
 */

#ifndef __DEBUG__H
#define __DEBUG__H

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*support ./server.bin inet*/
#define INTEGRATE_DEBUG
/*for changcheng demonstration only*/
//#define CHANGCHENG_DEMO

#define TIME_DEBUG
#define TRACE_DEBUG

#define LOG_FILE_PATH   "/usr/app/ecolink.log"
#define MAX_CHECK_TIME 100

typedef struct TIME_DEBUG_STRUCT {
    struct timeval t[MAX_CHECK_TIME ];
    unsigned long long _time_count[MAX_CHECK_TIME];
    unsigned long long _time_acc_val[MAX_CHECK_TIME];
    unsigned int _check_list[MAX_CHECK_TIME][2];
    unsigned int _current_seq;
    unsigned int _check_seq;
} TIME_DBG;


typedef struct {
    const char* tag;
    int flag;
} TAG;

void trace_init(const char* name);
void dump(const char* buf, int len, int endian);


extern int trace_mask;
extern int trace_bit;
extern int trace_count;
extern pthread_mutex_t dbg_lock;
extern TAG tags[];
extern TIME_DBG dbg;
extern int save_errno;
extern timeval a5a_t;

#define TRACE_GOGOGO(name) trace_init(#name);
#define ENABLE_TRACING  ((trace_mask & (1 << tags[trace_bit].flag)) != 0)
/* you must define TRACE_TAG before using this macro */
#define  DBG(...)                                      \
        do {                                           \
            if (ENABLE_TRACING) {                         \
                pthread_mutex_lock(&dbg_lock);           \
				gettimeofday(&a5a_t,NULL);  					\
                fprintf(stderr,"%d ms|%s|%s|%d|: ",           \
				(int)(a5a_t.tv_sec * 1000 + a5a_t.tv_usec / 1000),     \
				tags[tags[trace_bit].flag].tag,__FILE__, __LINE__);       \
                errno = save_errno;                    \
                fprintf(stderr, __VA_ARGS__ );         \
                fflush(stderr);                        \
                pthread_mutex_unlock(&dbg_lock);             \
                errno = save_errno;                    \
           }                                           \
        }while(0)

#define DUMP_L(data,len) \
	do{ \
		if(ENABLE_TRACING){ 							\
			pthread_mutex_lock(&dbg_lock);           	\
			dump(data,len,0); 							\
			pthread_mutex_unlock(&dbg_lock);            \
		}\
	}while(0)

unsigned int check_point_in_list(unsigned int from, unsigned int to);

#ifdef TIME_DEBUG

#define TIME_DECLARE() TIME_DBG dbg
//#define TIME_IMPORT() extern TIME_DBG dbg
#define TIME_INIT()  do{\
	dbg._current_seq = 0;\
	dbg._check_seq = 0;\
	memset(dbg._time_count,0,MAX_CHECK_TIME * sizeof(unsigned long long));\
	memset(dbg._check_list,0,MAX_CHECK_TIME * 2 * sizeof(unsigned int));\
	memset(dbg._time_acc_val,0,MAX_CHECK_TIME * sizeof(unsigned long long));\
}while(0)

#define TIME_INSERT(index)  do{\
	gettimeofday(&(dbg.t[index]),NULL);\
}while(0)

#define TIME_PRINT(event,index)   do{\
	TIME_INSERT(index);\
	printf("[%d ms] %s\n",dbg.t[index].tv_sec * 1000 + dbg.t[index].tv_usec / 1000, event);\
}while(0)

#define TIME_CHECK(from,to) do{\
	dbg._time_acc_val[check_point_in_list(from,to)] +=((dbg.t[from].tv_sec-dbg.t[to].tv_sec) * 1000 + (dbg.t[from].tv_usec-dbg.t[to].tv_usec) / 1000);\
printf("t"#from" to t"#to" cost %d ms\n",((dbg.t[from].tv_sec-dbg.t[to].tv_sec) * 1000 + (dbg.t[from].tv_usec-dbg.t[to].tv_usec) / 1000));\
}while(0)


#define TIME_ROUND(from,to) do{\
	dbg._time_count[check_point_in_list(from,to)] ++;}while(0)

#define TIME_AVERAGE(content,from,to) do{\
	dbg._current_seq = check_point_in_list(from,to);\
	dbg._time_acc_val[dbg._current_seq] +=((dbg.t[from].tv_sec-dbg.t[to].tv_sec)* 1000 + (dbg.t[from].tv_usec-dbg.t[to].tv_usec) / 1000);\
	printf("time acc =%lld count = %lld\n",dbg._time_acc_val[dbg._current_seq],dbg._time_count[dbg._current_seq]);\
	if(dbg._time_count[dbg._current_seq] % 100 == 0){\
		if(dbg._time_acc_val[dbg._current_seq] == 0){\
			dbg._time_count[dbg._current_seq] = 0;\
			break;\
		}\
		printf("<%s> cost %lld ms in %lld times, average cost %lld ms/image , %lld frames/s\n,",\
			content,\
			dbg._time_acc_val[dbg._current_seq],\
			dbg._time_count[dbg._current_seq],\
			dbg._time_acc_val[dbg._current_seq] / dbg._time_count[dbg._current_seq],\
			1000 * dbg._time_count[dbg._current_seq] / dbg._time_acc_val[dbg._current_seq]);\
		}\
}while(0)
#else
#define TIME_DECLARE()
#define TIME_IMPORT()
#define TIME_INIT()
#define TIME_INSERT(index)
#define TIME_PRINT(event,index)
#define TIME_CHECK(from,to)
#define TIME_ROUND()
#define TIME_AVERAGE(content,from,to)
#endif

#ifdef __cplusplus
}
#endif

#endif

