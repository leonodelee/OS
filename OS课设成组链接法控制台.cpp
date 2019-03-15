#include <iostream>
#include <cstring>
#include <conio.h>
#include <time.h>
using namespace std;

const int MAXGROUP=20;//定义组的大小为20
const int MAXPROCESS=100;//定义一个进程最大能申请的块数 
//组结构体
typedef struct node{
	int num_gp;//组中元素计数 
	int cell[MAXGROUP]; //组中元素存放的数组 
	struct node *next;//指向链表下一节点的指针 
}group;
group *g_GroupHead;//全局组链表头，为所有进程共享

//进程结构体 
typedef struct node1{
	char name[20];//进程名
	int num_ps;//进程个数
	int cell[MAXPROCESS];//进程号数组
	struct node1 *next;
}process; 
process *g_processHead;//定义进程链表头，为全局变量

int idletotal;//当前剩余总空闲块数

//初始化组函数
group *initial_group()
{
	int i;
	cout<<"\n**********************************************\n";
 	cout<<"		组初始化\n";
 	
	group *p;
	p= new group;
	p->num_gp=0;
	p->next=NULL;
	for(i=0;i<MAXGROUP;i++)
	{
		p->cell[i]=-1;
	}
//	cout<<"**********************************************\n";
	return p;
} 
//初始化进程函数
process *initial_process()
{
	int i;
	cout<<"**********************************************\n";
 	cout<<"		进程初始化\n";
 	cout<<"**********************************************\n";
	process *p;
	p=new process;
	strcpy(p->name,"");
	p->num_ps=0;
	p->next=NULL;
	for(i=0;i<MAXGROUP;i++)
	{
		p->cell[i]=-1;
	}
	return p;
} 
//读入空闲块文件并组织成成组链表 
void readData()
{
	FILE *fp; 
	char fname[20] = "Test.txt"; 
	int temp; 
	group *p; 

	while( (fp=fopen(fname,"r")) == NULL )
	{
		//打开默认的文件TestUnix.txt
		//fp=fopen("TestUnix.txt","r");

		if (NULL == fp)
		{
			cout<<"错误,文件"<< fname << "打不开"<<endl; 
		}
		//打开成功后就不要输入文件名了直接返回
		else
		{
			break;
		}
		//如果默认的文件打不开，手动输入文件名打开
		cout<<"请输入初始空闲块数据文件名:"; 		
		cin>>fname;
	}

	cout<<"**************************************************"<<endl; 
	cout<<"从文件" << fname << "读入的初始空闲块号为:"; 
	while(!feof(fp))
	{ 
		fscanf(fp,"%d ",&temp); 
		if(g_GroupHead->num_gp<MAXGROUP)
		{ 
			g_GroupHead->cell[g_GroupHead->num_gp]=temp; 
			g_GroupHead->num_gp++; 
		}
		else//所存储的空闲块号大于MAXGROUP时需要另申请节点
		{ 
			p=initial_group();
			/*
			p--
			  │
			   -->[head]--->[a]---->[b]--->[c]-->....
			*/
			p->next=g_GroupHead; //将申请的p节点插入到链首
			g_GroupHead=p; 
			p->cell[p->num_gp]=temp; 
			p->num_gp++; 
		} 
		
		if (0 == idletotal++%20 )//一组20一行 
		{
			cout << endl;
		}
		
		//输出初始数据 
		printf("%04d ",temp);
		//cout<<temp<<" ";
	}
	
	cout<<endl<<"当前总空闲块数:"<<idletotal<<endl; 
} 

void init()
{
	//初始化组链表 
	g_GroupHead=initial_group();
	//初始化计数器
	idletotal=0;
	//初始化作业链表
	g_processHead=initial_process();
	//从文件读取数据
	readData(); 
}

//分配内存块 
void distribute() 
{ 
	char processname[20]; 
	int number; 
	int i; 
	process *p; 

	cout<<"**********************************"<<endl; 
	cout<<"请输入新进程名:"; 
	cin>>processname; 
	cout<<"所需内存块数:"; 
	cin>>number; 
	
	if(number > idletotal)
	{ 
		cout<<"所需内存块数大于当前空闲块数,分配失败!"<<endl; 
	} 
	else
	{ 
		p=initial_process(); 
		strcpy(p->name,processname);
		/* 将节点p插入链表 */
		p->next=g_processHead->next; 
		g_processHead->next=p; 
		p->num_ps=number; 
		
		cout<<"申请成功，所申请到的空闲块号依次为:";
		
		for(i=0;i<number;i++)
		{ 
			if(g_GroupHead->num_gp > 1)
			{ 
				cout<<g_GroupHead->cell[g_GroupHead->num_gp-1]<<" "; 
				g_GroupHead->num_gp--; 
				p->cell[i]=g_GroupHead->cell[g_GroupHead->num_gp-1]; 
			} 
			else
			{ 
				cout<<g_GroupHead->cell[0]<<" "; 
				p->cell[i]=g_GroupHead->cell[g_GroupHead->num_gp-1]; 
				g_GroupHead->num_gp--; 
				
				if(g_GroupHead->next!=NULL)
				{ 
					g_GroupHead=g_GroupHead->next; 
				} 
			} 
			idletotal--; 
		} 
	} 
	cout<<endl; 
} 

//撤消进程 ，回收内存块 
void recycle() 
{ 
	char processname[20]; 
	int i; 
	process *p,*q; 
	group *r;
	cout<<"请输入要撤消的进程名:"; 
	cin>>processname; 
	
	q=g_processHead; 
	p=g_processHead->next;  
	while((p!=NULL)&&(strcmp(p->name,processname))){ //遍历，寻找指定进程 
		q=q->next; 
		p=p->next; 
	} 	
	if(p==NULL)
	{ 
		cout<<"Sorry,没有该进程"<<endl; 
	} 
	else
	{ 
		for(i=0;i<p->num_ps;i++)//释放物理块到相应的组链表 
		{ 
			if(g_GroupHead->num_gp<MAXGROUP)
			{ 
				g_GroupHead->cell[g_GroupHead->num_gp]=p->cell[i]; 
				g_GroupHead->num_gp++; 
			} 
			else{ //新建组 
				r=initial_group(); 
				r->next=g_GroupHead; 
				g_GroupHead=r; 
				r->cell[r->num_gp]=p->cell[i]; 
				r->num_gp++; 
			} 
		} 
		idletotal+=p->num_ps; //p进程多个物理块 
		
		q->next=p->next;//进程链表中删除指定进程节点 
		delete p; 
	} 
} 
void view()
{
	cout<<"*****************************************************"<<endl;
	cout<<endl<<"总空闲块数:"<<idletotal<<endl;  
	cout<<"当前组号为:"<<(g_GroupHead->cell[g_GroupHead->num_gp]/20)+1<<endl; 
	
	cout<<"*****************************************************************************"<<endl;
	cout<<"当前组内空闲块号为:"<<endl; 
	for(int i=0;i<g_GroupHead->num_gp;i++){ 
		cout<<g_GroupHead->cell[i]<<" "; 
	} 
	cout<<endl;
	cout<<"*****************************************************************************"<<endl;
} 

int main()
{
	bool flag = 1;
	int choice,n;char name[20];
	init();
	while(flag)
	{  
	  getch();
	  //system("cls");
	  cout<<"\n请输入选择:";
	  cout<<"1--分配，2--回收，3--显示信息，0--退出\n";
	  cin>>choice;
	  switch(choice)
	  {
	   case 0:
	   		flag=0;
	   		break;
	   case 1:
			distribute(); 
			break;
	   case 2:
			recycle();
		    break;
	   case 3:
	   		view();
		   break;     
	   default:
		   cout<<"选择错误！";
		   break;
	  }
	}
	return 0; 
}
