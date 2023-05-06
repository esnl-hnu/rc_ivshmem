
//#define ddline 100000
#include <math.h>
#include<stdio.h>
#include<stdlib.h>
const int ddline = 1000;
const int T = 2000;
struct flow_packet
{
    unsigned char dest;
    unsigned char op;
    unsigned char id;
    unsigned char length;
    unsigned char key;
    unsigned char priority;
    unsigned int sec;
    unsigned int  nsec;
    unsigned int deadline;
    unsigned int period;
    unsigned int WCTT1;
    unsigned int WCTT2;
    unsigned int WCTT3;
    unsigned int WCRT;
    int isalloc;
    int alloc_order;
};

int sort[8];
int sort_temp[8][30];
int average = 0;
int alloc_o = 0;
//int eq[30],eqnum;//分别表示同优先级和同优先级的个数；
int value[30], p_num;//分别表示优先级的个数；
struct flow_packet t1[30] = {
{3, 2, 1, 100, 1,2, 0, 0, ddline * 3,T,0,0,0,0,0,0},
{3, 2, 2, 200, 0,6, 0, 0, ddline * 4,T,0,0,0,0,0,0},
{3, 2, 3, 120, 1,5, 0, 0, ddline * 5,T,0,0,0,0,0,0},
{3, 2, 4, 140, 0,1, 0, 0, ddline * 7,T,0,0,0,0,0,0},
{3, 2, 5, 130, 1,3, 0, 0, ddline * 6,T,0,0,0,0,0,0},
{3, 2, 6, 140, 0,7, 0, 0, ddline * 5,T,0,0,0,0,0,0},
{3, 2, 7, 230, 1,0, 0, 0, ddline * 3,2 * T,0,0,0,0,0,0},
{3, 2, 8, 250, 0,1, 0, 0, ddline * 4,2 * T,0,0,0,0,0,0},
{3, 2, 9, 130, 1,4, 0, 0, ddline * 8,2 * T,0,0,0,0,0,0},
{3, 2, 10, 80, 0,6, 0, 0, ddline * 5,2 * T,0,0,0,0,0,0},
{3, 2, 11, 170, 1,3, 0, 0, ddline * 4,2 * T,0,0,0,0,0,0},
{3, 2, 12, 190, 0,0, 0, 0, ddline * 3,2 * T,0,0,0,0,0,0},
{3, 2, 13, 140, 1,5, 0, 0, ddline * 6,5 * T,0,0,0,0,0,0},
{3, 2, 14, 110, 0,4, 0, 0, ddline * 3,5 * T,0,0,0,0,0,0},
{3, 2, 15, 180, 1,1, 0, 0, ddline * 6,5 * T,0,0,0,0,0,0},
{3, 2, 16, 160, 0,2, 0, 0, ddline * 4,5 * T,0,0,0,0,0,0},
{3, 2, 17, 210, 1,4, 0, 0, ddline * 3,5 * T,0,0,0,0,0,0},
{3, 2, 18, 230, 0,0, 0, 0, ddline * 5,5 * T,0,0,0,0,0,0},
{3, 2, 19, 200, 1,7, 0, 0, ddline * 8,5 * T,0,0,0,0,0,0},
{3, 2, 20, 100, 0,5, 0, 0, ddline * 6,5 * T,0,0,0,0,0,0},
{3, 2, 21, 200, 1,4, 0, 0, ddline * 3,5 * T,0,0,0,0,0,0},
{3, 2, 22, 130, 0,6, 0, 0, ddline * 6,10 * T,0,0,0,0,0,0},
{3, 2, 23, 90, 1,0, 0, 0, ddline * 8,10 * T,0,0,0,0,0,0},
{3, 2, 24, 150, 0,3, 0, 0, ddline * 4,10 * T,0,0,0,0,0,0},
{3, 2, 25, 170, 1,2, 0, 0, ddline * 5,10 * T,0,0,0,0,0,0},
{3, 2, 26, 230, 0,7, 0, 0, ddline * 7,10 * T,0,0,0,0,0,0},
{3, 2, 27, 90, 1,8, 0, 0, ddline * 4,10 * T,0,0,0,0,0,0},
{3, 2, 28, 180, 0,1, 0, 0, ddline * 3,10 * T,0,0,0,0,0,0},
{3, 2, 29, 190, 1,7, 0, 0, ddline * 4,10 * T,0,0,0,0,0,0},
{3, 2, 30, 200, 0,5, 0, 0, ddline * 7,10 * T,0,0,0,0,0,0},
};

int comp(const void* a1, const void* b1)
{
    struct flow_packet a = *(struct flow_packet*)a1;
    struct flow_packet b = *(struct flow_packet*)b1;
    if (a.key < b.key)return 0;
    if (a.key > b.key)return 1;

    if (a.deadline < b.deadline)return 1;
    if (a.deadline == b.deadline && a.period < b.period)return 1;
    return 0;
}
int comp1(const void* a1, const void* b1)
{
    struct flow_packet a = *(struct flow_packet*)a1;
    struct flow_packet b = *(struct flow_packet*)b1;
    if (a.id < b.id)return 0;
    return 1;
}
int comp2(const void* a1, const void* b1)
{
    struct flow_packet a = *(struct flow_packet*)a1;
    struct flow_packet b = *(struct flow_packet*)b1;
    if(a.priority!=b.priority)return b.priority - a.priority;
    else {
        return (a.alloc_order - b.alloc_order);
    
    }
   /// if (a.priority == b.priority && a.isalloc < b.isalloc)return 1;
   // if (a.priority == b.priority && a.isalloc == b.isalloc && a.alloc_order > b.alloc_order)return 1;
   // return 0;

}
/*
int comp2( const void *a1,const void * b1)
{
	struct flow_packet a =*(struct flow_packet *)a1;
	struct flow_packet b =*(struct flow_packet *)b1;
	if(a.priority<b.priority)return 1;
	if(a.priority==b.priority&&a.isalloc<b.isalloc)return 1;
	if(a.priority==b.priority&&a.isalloc==b.isalloc&&a.alloc_order>b.alloc_order)return 1;
	return 0;
	
}*/



void message_init()
{

    for (int i = 0; i < 30; i++)
    {
        t1[i].priority = 0;
        t1[i].WCTT1 = t1[i].length*7/10;
        t1[i].WCTT2 = t1[i].length/4;
        t1[i].WCTT3 = 25;
        average = average + t1[i].WCTT1 * (10 * T / t1[i].period);
    }
    average = average / 8;
    qsort(t1, 30, sizeof(struct flow_packet), comp);
    return;
}

int get_blocktime1(struct flow_packet t1[], int n, struct flow_packet fi)
{
    int max = 0;
    for (int i = 0; i < n; i++)
    {
        if (fi.priority < t1[i].priority&&t1[i].isalloc!=0)
        {
            if (max < t1[i].WCTT1) max = t1[i].WCTT1;
        }
    }
    return max;
}
int get_blocktime2(struct flow_packet t1[], int n, struct flow_packet fi)
{
    int max = 0;
    for (int i = 0; i < n; i++)
    {
        if (fi.priority < t1[i].priority&&t1[i].isalloc!=0)
        {
            if (max < t1[i].WCTT2) max = t1[i].WCTT2;
        }
    }
    return max;
}
int calWCRT1(struct flow_packet fi, int blocktime, int eqtime, int hp_num, int wcqd) {
    //计算任务i的响应时间
    int  tmp = 0, i;
    for (i = 0; i < hp_num; i++)
    {
        if (((t1[i].isalloc == 0||(t1[i].isalloc != 0&&t1[i].priority<fi.priority)))&& fi.id != t1[i].id) {
            tmp = tmp + ceil((wcqd + 0.0001) / t1[i].period) * t1[i].WCTT1;
        }
    }
    tmp = tmp + blocktime + eqtime;
    //printf("tmp%4d\n")
    if(tmp>fi.deadline)return fi.deadline+1;
    if (wcqd == tmp)
        return wcqd;
    else
        return calWCRT1(fi, blocktime, eqtime, hp_num, tmp);
}

int calWCRT2(struct flow_packet fi, int blocktime, int eqtime, int hp_num, int wcqd) {
    //计算任务i的响应时间
    int  tmp = 0, i;
    for (i = 0; i < hp_num; i++)
    {
        if (((t1[i].isalloc == 0||(t1[i].isalloc != 0&&t1[i].priority<fi.priority)))&& fi.id != t1[i].id){
            tmp = tmp + ceil((wcqd + 0.0001) / t1[i].period) * t1[i].WCTT2;
        }
    }
    tmp = tmp + blocktime + eqtime;
    if(tmp>fi.deadline)return fi.deadline+1;
    if (wcqd == tmp)
        return wcqd;
    else
        return calWCRT2(fi, blocktime, eqtime, hp_num, tmp);
}
int calWCRT3(struct flow_packet fi, int blocktime, int eqtime, int hp_num, int wcqd) {
    //计算任务i的响应时间
    int  tmp = 0, i;
    for (i = 0; i < hp_num; i++)
    {
        if (((t1[i].isalloc == 0||(t1[i].isalloc != 0&&t1[i].priority<fi.priority)))&& fi.id != t1[i].id){
            tmp = tmp + ceil((wcqd + 0.0001) / t1[i].period) * t1[i].WCTT3;
        }
    }
    tmp = tmp + blocktime + eqtime;
    if(tmp>fi.deadline)return fi.deadline+1;
    if (wcqd == tmp)
        return wcqd;
    else
        return calWCRT3(fi, blocktime, eqtime, hp_num, tmp);
}

int schedulable(struct flow_packet t1[], int n, int i)
{
    int blocktime;
    int eqtime, lp_num;

    blocktime = get_blocktime1(t1, n, t1[i]);
    eqtime = 6 * blocktime;
    t1[i].WCRT = 0;
    t1[i].WCRT = t1[i].WCRT + calWCRT1(t1[i], blocktime, eqtime, n, t1[i].WCTT1);
    blocktime = get_blocktime2(t1, n, t1[i]);
    eqtime = 6 * blocktime;
    t1[i].WCRT = t1[i].WCRT + calWCRT2(t1[i], blocktime, eqtime, n, t1[i].WCTT2);
     blocktime = 25;
    eqtime = 6 * blocktime;
    t1[i].WCRT = t1[i].WCRT + calWCRT3(t1[i], blocktime, eqtime, n, t1[i].WCTT3);
    t1[i].WCRT = t1[i].WCRT + t1[i].WCTT1 + t1[i].WCTT2 + t1[i].WCTT3;
    if (t1[i].WCRT <= t1[i].deadline) { return 1; }
    return 0;
}


int allocp1(struct flow_packet t1[], int n, int num)
{
    int  p = 100;
    int i = 0,t_num;
    int average_tmp = 0, tmpp;
    int alloc;
    while (num < n)
    {
    //printf("tmp%4d\n",num);
        average_tmp = 0;
        alloc = 0;
        t_num = 0;
        for (i = 0; i < n; i++)  //查找未调度的集合
        {

            if (t1[i].isalloc == 1)  //已经分配了优先级的认为是低优先级
            {
                //if(t1[i].priority == p)
                //average_tmp = average_tmp+t1[i].WCTT1*(10*T/t1[i].period);
                continue;

            }


            if (t1[i].isalloc != 0)continue; //不可调度的任务      
            //average_tmp = average_tmp+t1[i].WCTT1*(10*T/t1[i].period);
 t_num++;
             //if(average_tmp>=average){average_tmp = average_tmp-t1[i].WCTT1*(10*T/t1[i].period);continue;}
            tmpp = t1[i].priority;
            t1[i].priority = p;
            if (schedulable(t1, n, i)) {
                t1[i].isalloc = 1;
                //p--;
                alloc_o = 10;
                t1[i].alloc_order = alloc_o;
                num++;

                // i=-1;
                // average_tmp = 0;
                alloc = 1;
                break;

            }
            else t1[i].priority = tmpp;
     

        }
        if (alloc||t_num==0) {
            //假如分配成功了，那么查看等优先级的分配顺序
            while (1) {
                alloc = 0;
                for (i = n; i >= 0; i--)
                {
                    if (t1[i].isalloc == 2 && t1[i].priority == p)
                    {
                        if (schedulable(t1, n, i)) {
                            t1[i].isalloc = 1;
                            alloc_o--;
                            t1[i].alloc_order = alloc_o;
                            num++;
                            alloc = 1;
                        }
                    }
                }
                if (alloc == 0)break;
            }
            for (i = n; i >= 0; i--)
            {
                if (t1[i].isalloc == 2 && t1[i].priority == p)
                    num++;
            }
            p--;

        }
        else {

            for (i = 0; i < n; i++)
            {
                if (t1[i].isalloc == 0)break;
            }
            t1[i].isalloc = 2;
            //t1[i].WCTT1 = 0;
            //t1[i].WCTT2 = 0;
            t1[i].alloc_order = 0;
            t1[i].priority = p;
            //num++;
            //allocp1(t1,n,num);
        }
        

    }

    return 1;
}
void  resetp(struct flow_packet t1[], int n)
{
    int i, p = 100, d;
    for (i = 0; i < n; i++)
    {
        if (t1[i].isalloc != 0) {
            if (p > t1[i].priority)p = t1[i].priority;
        }
    }
    d = p;
    for (i = 0; i < n; i++)
    {
        if (t1[i].isalloc != 0) {
            t1[i].priority = t1[i].priority - d;
        }
    }
}
int patition(struct flow_packet t1[], int n)
{
    p_num = 0;
    int i,k=-1,p = t1[0].priority;
    for (i = 0; i < n; i++)
    {
        if (i == 0 || t1[i].priority != t1[i - 1].priority || t1[i].isalloc == 2||(t1[i-1].isalloc == 2&& t1[i].isalloc!=2))
        {
            k++;
            value[k] = t1[i].WCTT1 * (10 * T / t1[i].period); 
            t1[i].alloc_order = k;
           // printf("k%4d", k );
        }
        else {
            value[k] = value[k]+t1[i].WCTT1 * (10 * T / t1[i].period);
            t1[i].alloc_order = k;
        }
    }
    p_num = k + 1;
    return 1;
}

int allocp2(struct flow_packet t1[], int n,int i,int j,int ave,int p)
{
	if(p<0&&i<p_num)return -1;
	if(i>=p_num)return ave;
	int average_tmp = 0,ave_tmp = ave,a,b,tmpj;
    for ( i ; i < p_num; i++)
    {
        tmpj = j;
        while (t1[j].alloc_order != i)
        {
            t1[j].priority = p;
            j++;
        }
        if (average_tmp + value[i] >=  average)
        {
        	break;
        }
         average_tmp = average_tmp + value[i];
     
    }
    if(i>=p_num)return abs(average_tmp - average+ave);
    p--;
    if(p<0)return -1;
    ave =  ave_tmp + average_tmp + value[i] - average;
    a = allocp2(t1,n,i+1,j,ave,p);
    ave =ave_tmp +  average - average_tmp;
    if(average_tmp!=0)b = allocp2(t1,n,i,tmpj,ave,p);
    if(b==-1)return a;
    if(a<=b&&a!=-1){
    allocp2(t1,n,i+1,j,ave,p);
    return a;
    }
    //ave = ave_tmp +average_tmp + value[i] - average;
    //a = allocp2(t1,n,i+1,j,ave,p);
   // if(a==-1)return b;
    //if(a<b){
   
   	
    //return a;}
    return b;
    
}
/*int allocp2(struct flow_packet t1[], int n)
{
    int average_tmp = 0, p = 7;
    for (int i = 0; i < n; i++)
    {
        average_tmp = average_tmp + t1[i].WCTT1 * (10 * T / t1[i].period);
        t1[i].priority = p;
        if (average_tmp >= average) { average_tmp = 0; p--; }
    }

    return 1;
}*/
int recheck(int n)
{
int i;
	for(i=0;i<n;i++)
	{
	if(schedulable(t1, n, i))t1[i].isalloc = 1;
	else t1[i].isalloc = 2;
	}
}
int main()
{
    message_init();

    allocp1(t1, 30, 0);
    resetp(t1, 30);

    qsort(t1, 30, sizeof(struct flow_packet), comp2);
    //allocp2(t1, 30);
   patition(t1, 30);
  
    allocp2(t1,30,0,0,0,7);
    //printf("ave%4d\n",as);
  qsort(t1, 30, sizeof(struct flow_packet), comp1);
  recheck(30);
    for (int i = 0; i < 30; i++)
    {printf("%4d,%4d,%4d,%4d ",t1[i].id,t1[i].WCTT1, t1[i].WCTT2,t1[i].WCTT3);
        printf("%4d,%4d,%4d,%4d,%4d\n", t1[i].id, t1[i].priority, t1[i].alloc_order,t1[i].isalloc, t1[i].WCRT);
    }
    return 0;
}

