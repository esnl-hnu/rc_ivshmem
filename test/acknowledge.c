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
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include "my_queue.h"
#include "my_message.h"
#define GET_MAP 1
#define REP_GET_MAP 2
#define PR_BUF 3
#define max_offset  33554432
#pragma  pack(1)

struct time_packet
{
    unsigned char dest ;
    unsigned char op;       //操作数
    unsigned char length;
    unsigned char priority; 
     unsigned int  sec;
    unsigned int  usec;
    unsigned char deadline;
};
struct flow_packet *timesend;
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




const char *filename = "/dev/ivshmem0";
unsigned int my_filesize = 0x4000;

unsigned int count =0;
unsigned char testchar[64];


int main(int argc,char *argv[])
{
    que_init();
    mmap_init();
     //init_mkfifo();
    unsigned int result,i,j,t,num;
    int max_num=my_filesize/packet_length;//块数目

    struct packet_msg *rev_msg; //接收块内存
    struct pr_buff_packet *pbp;
    
    struct timeval time;
    
    rev_msg = txbuff[(my_id-1)/2].buff;
    for(i=0;i<max_num;i++)
       rev_msg[i].value=0;
        init_mkfifo();
   // printf("my_id:%d\n",my_id);
    //printf("my_offset:%d\n",txbuff[(my_id-1)/2].offset);

    //testchar[0]=my_id;
    testchar[1]=0;
    //testchar[2]=61; 
    //testchar[3]=1+'0';
    //testchar[63]=0;
  
    pthread_t send_thread,rev_thread;
    struct timeval start_time,end_time;
    long int elapsed_time;
    int get;
    
    long long top;
    struct timespec ts,ttime;
    top = 0;
    int rev[8];
    int send[8];
    int key[2];
    key[1]=0;
    key[0] = 0;
    for(i=0;i<8;i++){send[i]=0;rev[i]=0;}
    //result = pthread_create(&send_thread,NULL,send_hander,NULL);
    if (result= 0) {
        printf("发送线程创建失败");
        return 0;
    } //*/    
    
    while(1){
  for(j=0;j<8;j++){
  	i = (mq[j]->out)%7;
  	rev_msg = mq[j]->buffer;
  	
  	
            if(rev_msg[i].value ==4){
            
            	
           	if(rev_msg[i].op == 2){
           		 clock_gettime(CLOCK_REALTIME,&ttime);
           		timesend = (struct flow_packet *)&rev_msg[i];
           		//timesend->deadline = timesend->deadline/20;
           		if(timesend->priority==0){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		{key[timesend->key]++;
           		send[0]++;}
           		//else printf("id:%d, %d\n",timesend->id,send[0]);
           		//printf("%d,%d\n",ttime.tv_nsec,timesend->nsec+timesend->deadline);
           		rev[0]++;
           		}
           		else if(timesend->priority==1){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		{key[timesend->key]++;
           		send[1]++;}
           		//else printf("id:%d\n",timesend->id);
           		rev[1]++;
           		}
           		else if(timesend->priority==2){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		{key[timesend->key]++;
           		send[2]++;}
           		//else printf("id:%d\n",timesend->id);
           		rev[2]++;
           		}
           		else if(timesend->priority==3){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		{key[timesend->key]++;
           		send[3]++;}
           		//else printf("id:%d\n",timesend->id);
           		rev[3]++;
           		}
           		else if(timesend->priority==4){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		 {key[timesend->key]++;
           		 send[4]++;}
           		 //else printf("id:%d\n",timesend->id);
           		 rev[4]++;
           		}
           		else if(timesend->priority==5){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		 {key[timesend->key]++;
           		 send[5]++;}
           		 //else printf("id:%d\n",timesend->id);
           		 rev[5]++;
           		}
           		else if(timesend->priority==6){
           		if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
           		{key[timesend->key]++;
           		send[6]++;}
           		//else printf("id:%d\n",timesend->id);
           		rev[6]++;
             		}
             		else {
			if(ttime.tv_nsec<=(timesend->nsec+timesend->deadline)||ttime.tv_sec<timesend->sec)
             		{key[timesend->key]++;
             		send[7]++;}
             		//else printf("id:%d\n",timesend->id);
             		rev[7]++;
             		}
		rev_msg[i].value = 0;
           	}
           	 else if(rev_msg[i].op == 3){
           	 printf("------------------------------------------------------------------------------------------------------\n");

 // printf("flows in time     :critical flows:%d ;  non-critical flows:%d\n",key[1],key[0]);
           printf("|   Priority & critical level  |  P1  |  P2  |  P3  |  P4  |  P5  |  P6  |  P7  |  P8  |  C1  |  C0  |\n");
           printf("------------------------------------------------------------------------------------------------------\n");
              printf("| num of flow instacne in time | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n",send[0],send[1],send[2],send[3],send[4],send[5],send[6],send[7],key[1],key[0]);
              printf("------------------------------------------------------------------------------------------------------\n");
              printf("|  num of rev flow instances   | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | 5800 | 5800 |\n",rev[0],rev[1],rev[2],rev[3],rev[4],rev[5],rev[6],rev[7]);
              printf("------------------------------------------------------------------------------------------------------\n");
           	rev_msg[i].value = 0;
           	return;
           	}	
           	//printf("j%d,id:%d\n",mq[j]->out,timesend->priority);
           	//printf("j%d\n",txbuff[1].buff[8]);
           	//printf("%ld   %ld\n",&txbuff[1].buff[1*8+1],mq[1]->buffer );
           	rev_msg[i].value = 0;
           	mq[j]->out++;
           	break;
           	}
          
           
         
        }}
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
       my_id=3;
    dest_id=1;   
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

