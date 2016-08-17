
/**
 * @file debug.c
 * @brief
 * @author yang.ding@le.com
 * @version v.01
 * @date 2016-06-21
 */

#include "debug.h"


#define MAX_TAGS 100


TIME_DBG dbg;

TAG tags[MAX_TAGS] = {
    { "all", 0 },
    /*added items here in a new "example" project*/
#if 0
    { "ledaemon", TRACE_ledaemon },
    { "ecolink", TRACE_ecolink },
    { "libso", TRACE_libso },
    { "mirror", TRACE_mirror },
    { "cariphone", TRACE_cariphone },
#endif
};
int trace_mask = 0;
int trace_bit = 0;
int trace_count = 0;
pthread_mutex_t dbg_lock = PTHREAD_MUTEX_INITIALIZER ;
int save_errno;
struct timeval a5a_t; 					

/**
 * @brief check_point_in_list
 * @param from
 * @param to
 * @return
 */
unsigned int check_point_in_list(unsigned int from, unsigned int to)
{
    int i = 0;
    for(; i < MAX_CHECK_TIME ; i++) {
        if(dbg._check_list[i][0] == from  &&
           dbg._check_list[i][1] == to) {
            return i;
        }
    }

    dbg._check_list[dbg._check_seq][0] = from;
    dbg._check_list[dbg._check_seq][1] = to;
    dbg._check_seq ++;

    return (dbg._check_seq - 1);
}

static void trace_init(const char* name)
{
    /*read from env*/
    const char*  p = getenv("ECOLINK_TRACE_CFG");
    const char*  q;

    if (p == NULL)
        return;
    int i;
    for(i = 0 ; i <  MAX_TAGS ; i ++) {
        if(tags[i].tag != NULL) {
            printf("name %s tag[i] %s\n",name ,tags[i].tag);
            if(!strcmp(name,tags[i].tag)) {
                trace_bit = tags[i].flag /*= num*/;
                break;
            }
            continue;
        }
        tags[i].tag = name;
        trace_bit = tags[i].flag = (++ trace_count);
        break;
    }
    if(i == MAX_TAGS - 1) {
        printf("error reach max tagsize\n");
        return;
    }



    /* use a comma/column/semi-colum/space separated list */
    while (*p) {
        int  len, tagn;

        q = strpbrk(p, " ,:;");
        if (q == NULL) {
            q = p + strlen(p);
        }
        len = q - p;

        for (tagn = 0; tags[tagn].tag != NULL; tagn++) {
            int  taglen = strlen(tags[tagn].tag);

            if (len == taglen && !memcmp(tags[tagn].tag, p, len) ) {
                int  flag = tags[tagn].flag;
                if (flag == 0) {
                    trace_mask = ~0;
                    printf("tag = %04x\n",trace_mask);
                    return;
                }
                trace_mask |= (1 << flag);
                printf("tag = %04x\n",trace_mask);
                break;
            }
        }
        p = q;
        if (*p)
            p++;
    }
    return;
}

void dump(const char* buf, int len, int endian){
	int i = 0;
	fprintf(stderr,"%d ms|%s|%s|%d|: ",           
				(int)(a5a_t.tv_sec * 1000 + a5a_t.tv_usec / 1000),     
				tags[tags[trace_bit].flag].tag,__FILE__, __LINE__);       
	fprintf(stderr,"dump\n");
	if(endian == 0){
		for(;i < len; i++){
			fprintf(stderr,"%02x ",buf[i]);
			if((i+1) % 20 == 0)
				fprintf(stderr,"\n");
		}
	}
	else{
		for(;i < len; i++){
			fprintf(stderr,"%02x ",buf[len-i-1]);
			if((i+1) % 20 == 0)
				fprintf(stderr,"\n");
		}
	}
	fprintf(stderr,"\n");
	fflush(stderr);
}


