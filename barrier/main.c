#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define RADIX 10


struct timeval start,end;
double execute_time;


void *fsync_op(void *data)
{
	struct timeval w_start, w_end;
	struct timeval f_start, f_end;
	double w_c_time, f_c_time;
	int count = 0;
	int check;

	int fd,i;
	char f_name[50];	
	char path[100];
	char buf[4096];
	int su = (*(int *)data);
	sprintf(f_name,"test%d.txt",su);
	sprintf(path,"/media/sata/blktrace_data/%s",f_name);

	fd = open(path, O_WRONLY | O_CREAT, 0644);
	
	for(i = 0 ; i < 100; i++)
	{
		write(fd, buf, 4096);
	/*
		gettimeofday(&w_end, NULL);
		w_c_time = (double) (w_end.tv_usec - start.tv_usec) / 1000000;
		printf("%lf\t(sec)\t thread_num (%d) -- write_time\n",w_c_time, su);
	*/
			
		check = fsync(fd);
		if(check == 0)
		{
			count++;
		}
		
	/*
			gettimeofday(&f_end,NULL);
			f_c_time = (double) (f_end.tv_usec - start.tv_usec) / 1000000;
			printf("%lf\t(sec)\t thread_num (%d) --> fsync_time\n",f_c_time,su);
	*/
	}

	close(fd);

	printf("thread (%d) fsync_count = %d\n",su,count);
	//printf("thread %d exit!!\n",su);
	return NULL;	


}
int main(int argc, char **argv)
{
	int *k;
	int i, n;
	pthread_t *p_thread;

	if(argc != 2)
	{
		fprintf(stderr,"Argument error.\n");
		exit(1);
	}
	n = atoi(argv[1]);
	p_thread = (pthread_t *)malloc(sizeof(pthread_t) * n);
	k = (int *)malloc(sizeof(int) * n);

	printf("TEST START!!\n");

	//gettimeofday(&start, NULL);
	for(i = 0 ; i < n; i++)
	{
		k[i] = i;
		pthread_create(&p_thread[i], NULL, fsync_op, (void *)&k[i]);
	}


	for(i = 0 ; i < n; i++)
	{
		pthread_join(p_thread[i],NULL);
	}
	//gettimeofday(&end,NULL);

	/*
	execute_time = (double) (end.tv_usec - start.tv_usec) / 1000000;
	printf("Program execute_time : %lf sec\n", execute_time);
	*/
/*(
	for(i = 0; i < n ; i++)
	{
		sprintf(msg,"test%d.txt",k[i]);
		remove(msg);	
	}

*/
		
	printf("TEST EXIT!!\n");
  
  
	return 0;

}

