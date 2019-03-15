#include <iostream>
#include <cstring>
#include <conio.h>
#include <time.h>
using namespace std;

const int cylinder=3,track=2,sector=4;//柱面、磁道、物理块 号 
#define SIZE 100//最大块数 
const int M=cylinder,N=track*sector;

struct process//process顺序表数据结构，存储申请的物理块信息 
{
	char name[20];
	int c[SIZE],t[SIZE],s[SIZE];
	int n;
};
process processtable[SIZE];//process格式表 
int ppointer=-1;//process指针 

class bitmap//位示图 结构体 
{
	public:
		int bitmap[M][N];
		void Initbitmap();//初始化位示图
		bool spaceisok(int n);//位示图符合判断
		void displaybitmap();//打印 
};
bitmap bm;//全局位示图，为所有进程共享 


void bitmap::Initbitmap()
{
	int i,j;
	 cout<<"**********************************************\n";
 	 cout<<"		位示图初始化\n";
 	 cout<<"**********************************************\n";
 	for(i=0;i<M;i++)
 	{
 		for(j=0;j<N;j++)
 		{
 			bitmap[i][j]=0;
 		}
 	}
 //	getchar();
 	displaybitmap();//初始化后位示图 
 	getchar();
 //system("cls");
}
bool bitmap::spaceisok(int n)//判断位示图空闲物理块是否足够 
{
	int count=0;
	for(int i=0;i<M;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(bitmap[i][j]==0)
			{
				count++;
			}
		}
	}
	if(count<n)
		return false;
	else
		return true;
}
void bitmap::displaybitmap()//打印位示图
{
	int i,j;
	cout<<"\n当前位示图信息如下：\n";
	cout<<"\n***********************************************************************\n";
	for(i=0;i<M;i++)
	{
		for(j=0;j<N;j++)
		{
			cout<<"\t"<<bitmap[i][j];
		}
		cout<<endl;
	}
	cout<<"\n***********************************************************************\n";
	if(ppointer<0)
	{
		cout<<"\n尚未分配磁盘空间\n";
		return;
	}
	
	else
	{
		cout<<"\n当前分配信息如下：\n";
		cout<<"\n#######################################################";
		cout<<"\n进程名\t\t分配的物理块数\n";
		
	//	cout<<"\n进程名\t\t分配的物理块数\t\t物理块信息\n";
		for(int i=0;i<=ppointer;i++)
		{
			if(processtable[i].n==0)
				continue;
			else
			{
				cout<<"\n"<<processtable[i].name<<"\t\t   "<<processtable[i].n;
				/*
				for(j=0;j<processtable[i].n;j++)
				{
					if(j==0)
					{
						cout<<"\t\t\t("<<processtable[i].c[j]<<","<<processtable[i].t[j]<<","<<processtable[i].s[j]<<")\n";	
					}
					else
					{
						cout<<"\t\t\t\t\t("<<processtable[i].c[j]<<","<<processtable[i].t[j]<<","<<processtable[i].s[j]<<")\n";
					}
				}
				*/
			}
		}
		
		cout<<"\n#######################################################\n";
	}
} 

void distribute(char name[20],int n)//分配
{
  int i,j;
  int count=0;/*计数器*/
  	for(i=0;i<=ppointer;i++)//processtable中逐个搜索指定进程 
	{
		if(!strcmp(processtable[i].name,name))
		{
			cout<<"\n进程名重复，请检查后命名。\n";
			goto end; 
		}
	}
  
  if(!bm.spaceisok(n))
  {
	cout<<"空间不足，找不到"<<n<<"块物理块，分配失败!";
	return;
  }
  ppointer++;
  strcpy(processtable[ppointer].name,name);//分配的物理块赋予名字 
  processtable[ppointer].n=n;//物理块数
  for(i=0;i<M;i++)//二维数组逐个搜索空闲物理块 
  {
	  for(j=0;j<N;j++)
		if(bm.bitmap[i][j]==0) 
		{
			processtable[ppointer].c[count]=i;/*柱面号*/
			processtable[ppointer].t[count]=j/4;/*磁道号*/
			processtable[ppointer].s[count]=j%4;/*物理记录号*/
			bm.bitmap[i][j]=1;
	//		cout<<666666<<endl;
			count++;
			if(count==n)
				return;
		}
  }
  end:
  	return;
}

void recycle(char name[20])//回收内存 
{
	int i,j,flag=0;
	for(i=0;i<=ppointer;i++)//processtable中逐个搜索指定进程 
	{
		if(!strcmp(processtable[i].name,name))
		{
			for(j=0;j<processtable[i].n;j++)
				bm.bitmap[processtable[i].c[j]][4*processtable[i].t[j]+processtable[i].s[j]]=0;//位示图相应置零 
		
			for(int k=i;k<=ppointer-i;k++)//process表项移动
			{
				strcpy(processtable[k].name,processtable[k+1].name);//删除，前移 
				processtable[k].n=processtable[k+1].n;
				for(int l=0;l<processtable[k].n;l++)
				{
					processtable[k].c[l]=processtable[k+1].c[l];
					processtable[k].t[l]=processtable[k+1].t[l];
					processtable[k].s[l]=processtable[k+1].s[l];
				}
			}
			ppointer--;
			flag=1;
			//delay();
			cout<<"\n找到进程，回收完毕。\n";
		}
	}
	if(flag==0)
		cout<<"\n未找到进程名为"<<name<<"的进程！！可能尚未此进程分配物理块\n";
}

int main()
{
	int choice,n;char name[20];
	bm.Initbitmap();
	while(1)
	{  
	  getch();
	//  system("cls");
	  cout<<"\n请输入选择:";
	  cout<<"1--分配，2--回收，3--显示位示图，0--退出\n";
	  cin>>choice;
	  switch(choice)
	  {
	   case 1:
		   {
			   cout<<"请输入进程名和需要分配的物理块数：";
			   cin>>name>>n;
			   distribute(name,n);
			   break;
		   }
	   case 2:
		   {
			   cout<<"给出要回收的进程名：";
			   cin>>name;
			   recycle(name);
		       break;
		   }
	   case 3:
		   bm.displaybitmap();
		   break;     
	   case 0:
		   exit(0);
	   default:
		   cout<<"选择错误！";
		   break;
	  }
	}
	return 0; 
}
