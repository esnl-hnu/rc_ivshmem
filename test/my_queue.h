#ifndef _MY_QUEUE_H_
#define _MY_QUEUE_H_
#define packet_length 256 //块大小@
#define MAX_MSG_NUM 64 //队列最大长度 
#define PRIORITY_NUM 8 //优先级数目
#define max_vm 2 //最大主机数
unsigned char my_id = 1;
unsigned dest_id =  3;

unsigned char state=0; //用来表示当前的发送队列状态
unsigned int send_num = 0;

struct mesg     //队列节点
{
  unsigned char dest;           //目的id
  struct mesg *next;            
  unsigned char text[packet_length];    //发送内容
  unsigned int len;                 //长度
  unsigned char priority;           //优先级,优先级越小，优先级越高
  //static unsigned char num;
};

struct mkfifo{
	struct mesg *buffer;
	unsigned int size;
	unsigned int in;
	unsigned int out;
	unsigned int out2;
	pthread_mutex_t *f_lock;
		
}*mq[8];
struct mkfifo *fifo_t;
struct packet_msg{  //数据包的组成
    unsigned char value;    //状态字节
    unsigned char op;       //操作数
    unsigned char p[packet_length-2];   //数据字段
};

struct RWBUFF //接收内存
{
  int id;   
  struct packet_msg *buff; 
  unsigned int filesize;
  unsigned int offset;
 }txbuff[max_vm];


int insert_mesg(struct mesg *msg); 
int send_msg(unsigned char *p,unsigned int length,unsigned char priority,unsigned char  dest);
int send_packet();


//对应于分拣调度模块，发送函数所实际调用的函数
int send_msg(unsigned char *p,unsigned int length,unsigned char priority,unsigned char  dest)
{   

    int i,n,j;
    //发送部分
	
    i = (dest-1)/2;
    n= txbuff[i].filesize/packet_length/8;
    fifo_t = (struct mkfifo *)(txbuff[i].buff+priority*n);
    struct packet_msg *send_msg = txbuff[i].buff+priority*n+1;
    
    if(fifo_t->in-fifo_t->out<n-1){
    i = (fifo_t->in)%7;
     
   if( send_msg[i].value==0 ){

   // printf("%ld   %ld   %ld\n",&send_msg[i],&(txbuff[1].buff[priority*n+1+i]),i);
    fifo_t->in++;
    send_msg[i].value=my_id+1;
     p[0]=my_id+1;
    memcpy((unsigned char*)&send_msg[i],p,length);
   
     send_msg[i].value = my_id;
     
     return 0;
    }
    
     
    
     
    }
    	
       /* for(i=0;i<n;i++)
        {
        if(send_msg[i].value==0){
            send_msg[i].value=my_id+1;
            p[0]=my_id+1；
            memcpy((unsigned char*)&send_msg[i],p,length);
            //printf("send_packet:ok,%d\n",j);
           send_msg[i].value = my_id;
           return 1;
        }
        }*/

    return 1;
}


//对应于发送引擎，每次从发送队列中取出一个数据包发送
int send_packet()
{
   pause();
}

void que_init()
{
   
    return;
}


int init_mkfifo()
{
	int i,n,j;
    	i = (my_id-1)/2;
    	//printf("fifo%d\n",sizeof(struct mkfifo ));
    	n= txbuff[i].filesize/packet_length/8;
    	struct packet_msg *send_msg = txbuff[i].buff;
 	for(i=0;i<8;i++)
 	{
 	mq[i] = (struct mkfifo *)(send_msg+i*n);
 	mq[i]->in = 0;
 	mq[i]->out = 0;
 	mq[i]->out2 = 0;
 	mq[i]->size = n - 1;
 	mq[i]->buffer = send_msg+i*n+1 ;
 	//printf("%ld   %ld   %d\n",&txbuff[(my_id-1)/2].buff[i*n+1],mq[i]->buffer,mq[i]->in );
 	}
 	
}

#endif
