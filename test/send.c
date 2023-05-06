#define _GNU_SOURCE
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>
#include<time.h>
#include <pthread.h>
#include <signal.h>
#include "my_queue.h"
#include "my_message.h"
#define GET_MAP 1
#define REP_GET_MAP 2
#define PR_BUF 3
#define max_offset  33554432
#pragma  pack(1)

//struct timeval ttime;
struct timespec ttime; 
int time_count=0,size =0 ;
timer_t timerid;
//struct timeval ttime;
struct sigevent sev;
struct itimerspec trigger;

struct time_packet
{
    unsigned char dest ;
    unsigned char op ;       //操作数
    unsigned char length;
    unsigned char priority; 
     unsigned int  sec;
    unsigned int  usec;
    unsigned char deadline;
};
struct time_packet timesend;
int send[8];
int key[2];

void signal_handler(int sig) {

	int i,j,k;
      //gettimeofday(&ttime,NULL);
   if(time_count>=1000)return;
   //printf("%d\n",time_count);
    for(i=1;i<8;i++)sort[i]=0;
   clock_gettime(CLOCK_REALTIME,&ttime);
   time_count++;
  
   if(time_count%10==0){
   for(i=0;i<30;i++)
   {
   	if(t1[i].priority==0){
   	t1[i].nsec=ttime.tv_nsec;
   	t1[i].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[i],t1[i].length,t1[i].priority,3));
   	key[t1[i].key]++;
        send[t1[i].priority]++;	
   	}
   	
   	else {
   	sort_temp[t1[i].priority][sort[t1[i].priority]]=i;
   	sort[t1[i].priority]++; 	
   	}
   }
   for(i=1;i<8;i++)
   {
   	for(j=0;j<sort[i];j++)
   	{
   		k=sort_temp[i][j];
   		t1[k].nsec=ttime.tv_nsec;
   	t1[k].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[k],t1[k].length,t1[k].priority,3));
   	key[t1[k].key]++;
        send[t1[k].priority]++;
   	sort_temp[i][j] = 0;
   	}
   }
   }
   else  if(time_count%5==0){
   for(i=0;i<21;i++)
   {
   	if(t1[i].priority==0){
   	t1[i].nsec=ttime.tv_nsec;
   	t1[i].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[i],t1[i].length,t1[i].priority,3));	
   	key[t1[i].key]++;
        send[t1[i].priority]++;
   	}
   	
   	else {
   	sort_temp[t1[i].priority][sort[t1[i].priority]]=i;
   	sort[t1[i].priority]++; 	
   	}
   	if(i==5)i=12;
   }
   for(i=1;i<8;i++)
   {
   	for(j=0;j<sort[i];j++)
   	{
   		k=sort_temp[i][j];
   		t1[k].nsec=ttime.tv_nsec;
   	t1[k].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[k],t1[k].length,t1[k].priority,3));
   	key[t1[k].key]++;
        send[t1[k].priority]++;
   	sort_temp[i][j] = 0;
   	}
   }
   }
   else  if(time_count%2==0){
   for(i=0;i<12;i++)
   {
   	if(t1[i].priority==0){
   	t1[i].nsec=ttime.tv_nsec;
   	t1[i].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[i],t1[i].length,t1[i].priority,3));	
   	key[t1[i].key]++;
        send[t1[i].priority]++;
   	}
   	
   	else {
   	sort_temp[t1[i].priority][sort[t1[i].priority]]=i;
   	sort[t1[i].priority]++; 	
   	}
   }
   for(i=1;i<8;i++)
   {
   	for(j=0;j<sort[i];j++)
   	{
   		k=sort_temp[i][j];
   		t1[k].nsec=ttime.tv_nsec;
   	t1[k].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[k],t1[k].length,t1[k].priority,3));
   	key[t1[k].key]++;
        send[t1[k].priority]++;
   	sort_temp[i][j] = 0;
   	}
   }
   }
   else  if(time_count%1==0){
   for(i=0;i<6;i++)
   {
   	if(t1[i].priority==0){
   	t1[i].nsec=ttime.tv_nsec;
   	t1[i].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[i],t1[i].length,t1[i].priority,3));
   	key[t1[i].key]++;
        send[t1[i].priority]++;	
   	}
   	
   	else {
   	sort_temp[t1[i].priority][sort[t1[i].priority]]=i;
   	sort[t1[i].priority]++; 	
   	}
   }
   for(i=1;i<8;i++)
   {
   	for(j=0;j<sort[i];j++)
   	{
   		k=sort_temp[i][j];
   		t1[k].nsec=ttime.tv_nsec;
   	t1[k].sec=ttime.tv_sec;
   	while(send_msg((char *)&t1[k],t1[k].length,t1[k].priority,3));
   	key[t1[k].key]++;
        send[t1[k].priority]++;
   	sort_temp[i][j] = 0;
   	}
   }
   }
 
   return ;
}

struct pr_buff_packet{
    unsigned char src;
    unsigned char op;
    unsigned char len;
    unsigned char p[packet_length-3];
};

//存放共享内存的队列的信息


unsigned char * get_buff_map(unsigned int filesize,unsigned int offset);
int get_id();
void askforid(unsigned int filesize);
void * send_hander();
void mmap_init();




const char *filename = "/dev/ivshmem0";
unsigned int my_filesize = 0x4000;

unsigned int count =0;
unsigned char testchar[64];


int main(int argc,char *argv[])
{
cpu_set_t mask;
CPU_ZERO(&mask);
CPU_SET(1, &mask);
sched_setaffinity(0, sizeof(cpu_set_t), &mask);

//CPU_SET(5, &mask);
key[0] = 0; key[1] = 0;
    que_init();
    mmap_init();
    message_init();
   // init_mkfifo();
 
	allocp1(t1,30,0);
	resetp(t1,30);
	 qsort(t1, 30, sizeof(struct flow_packet), comp2);
   patition(t1, 30);
    allocp2(t1,30,0,0,0,7);
        for (int i = 0; i < 30; i++)
    {//printf("%d,%d,%d,%d\n ",t1[i].id,t1[i].key, t1[i].deadline,t1[i].WCRT);
       // printf("%d,%d,%d,%d,%d\n", t1[i].id, t1[i].priority, t1[i].alloc_order,t1[i].isalloc, t1[i].WCRT);
       ;
    }
    qsort(t1, 30, sizeof(struct flow_packet), comp1);
    int result,i,t,num;
    int max_num=my_filesize/packet_length;//块数目

    struct packet_msg *rev_msg; //接收块内存
    struct pr_buff_packet *pbp;
    
    
    rev_msg = txbuff[(my_id-1)/2].buff;
    for(i=0;i<max_num;i++)
       rev_msg[i].value=0;
        init_mkfifo();
    //printf("my_id:%d\n",my_id);
    //printf("my_offset:%d\n",txbuff[(my_id-1)/2].offset);
    //testchar[0]=my_id;
   // testchar[1]=0;
    //testchar[2]=61; 
    //testchar[3]=1+'0';
    //testchar[63]=0;
    pthread_t send_thread,rev_thread;
    struct timeval start_time,end_time,time;
    long long elapsed_time;
    int get ;
    struct timespec ts;
    
   
    clock_gettime(CLOCK_REALTIME,&ts);
    
    result = pthread_create(&send_thread,NULL,send_hander,NULL);
    if (result= 0) {
        printf("发送线程创建失败");
        return 0;
    } //*/
    
sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        return -1;
    }

    trigger.it_value.tv_sec = 0;
    trigger.it_value.tv_nsec = 50000; // 10 微秒
    trigger.it_interval.tv_sec = 0;
    trigger.it_interval.tv_nsec = 50000; // 10 微秒
    if (timer_settime(timerid, 0, &trigger, NULL) == -1) {
        perror("timer_settime");
        return -1;
    }
    
 for(i=0;i<8;i++)send[i]=0;
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }
	timesend.op = 2;
	//send_msg((char *)&t1[30],t1[30].length,7,3);
	
    while (1) {
    //if(time_count)
    if(time_count>=1000)
    {
    timer_delete(timerid);
     printf("|   Priority & critical level  |  P1  |  P2  |  P3  |  P4  |  P5  |  P6  |  P7  |  P8  |  C1  |  C0  |\n");
           printf("------------------------------------------------------------------------------------------------------\n");
              printf("|  num of rev flow instances   | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n",send[0],send[1],send[2],send[3],send[4],send[5],send[6],send[7],key[1],key[0]);
              printf("------------------------------------------------------------------------------------------------------\n");
    //printf("here\n");
    while(1){
    	timesend.op = 3;
	send_msg((char *)&timesend,60,0,3);
    }
    pause();
    }
     }   
    
    // 无限循环等待定时器信号
    return 0;
}



unsigned char * get_buff_map(unsigned int filesize, unsigned int offset)
{
    int fd;

    if((fd = open(filename, O_RDWR,0644))<0)
    {
        printf("%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    unsigned char *map;
    if((  map = (char *)mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd,offset)) <= 0)
    {
        printf("%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    close(fd);
    return map;
}




void mmap_init()
{
    //my_id=1;
    //dest_id=3;
    int i;
    i = (my_id-1)/2;
    txbuff[i].id = my_id;
    txbuff[i].filesize = my_filesize;
    txbuff[i].offset = my_filesize;
    txbuff[i].buff = get_buff_map (txbuff[i].filesize,txbuff[i].offset);

    i= (dest_id-1)/2;
    txbuff[i].id = dest_id;
    txbuff[i].filesize = my_filesize;
    txbuff[i].offset = 2*my_filesize;
    txbuff[i].buff = get_buff_map (txbuff[i].filesize,txbuff[i].offset);
          
   return ;
}

void * send_hander()
{


    while(1)
    {
        send_packet();
        //sleep(5);
    }
}

