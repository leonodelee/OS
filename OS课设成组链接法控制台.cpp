#include <iostream>
#include <cstring>
#include <conio.h>
#include <time.h>
using namespace std;

const int MAXGROUP=20;//������Ĵ�СΪ20
const int MAXPROCESS=100;//����һ���������������Ŀ��� 
//��ṹ��
typedef struct node{
	int num_gp;//����Ԫ�ؼ��� 
	int cell[MAXGROUP]; //����Ԫ�ش�ŵ����� 
	struct node *next;//ָ��������һ�ڵ��ָ�� 
}group;
group *g_GroupHead;//ȫ��������ͷ��Ϊ���н��̹���

//���̽ṹ�� 
typedef struct node1{
	char name[20];//������
	int num_ps;//���̸���
	int cell[MAXPROCESS];//���̺�����
	struct node1 *next;
}process; 
process *g_processHead;//�����������ͷ��Ϊȫ�ֱ���

int idletotal;//��ǰʣ���ܿ��п���

//��ʼ���麯��
group *initial_group()
{
	int i;
	cout<<"\n**********************************************\n";
 	cout<<"		���ʼ��\n";
 	
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
//��ʼ�����̺���
process *initial_process()
{
	int i;
	cout<<"**********************************************\n";
 	cout<<"		���̳�ʼ��\n";
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
//������п��ļ�����֯�ɳ������� 
void readData()
{
	FILE *fp; 
	char fname[20] = "Test.txt"; 
	int temp; 
	group *p; 

	while( (fp=fopen(fname,"r")) == NULL )
	{
		//��Ĭ�ϵ��ļ�TestUnix.txt
		//fp=fopen("TestUnix.txt","r");

		if (NULL == fp)
		{
			cout<<"����,�ļ�"<< fname << "�򲻿�"<<endl; 
		}
		//�򿪳ɹ���Ͳ�Ҫ�����ļ�����ֱ�ӷ���
		else
		{
			break;
		}
		//���Ĭ�ϵ��ļ��򲻿����ֶ������ļ�����
		cout<<"�������ʼ���п������ļ���:"; 		
		cin>>fname;
	}

	cout<<"**************************************************"<<endl; 
	cout<<"���ļ�" << fname << "����ĳ�ʼ���п��Ϊ:"; 
	while(!feof(fp))
	{ 
		fscanf(fp,"%d ",&temp); 
		if(g_GroupHead->num_gp<MAXGROUP)
		{ 
			g_GroupHead->cell[g_GroupHead->num_gp]=temp; 
			g_GroupHead->num_gp++; 
		}
		else//���洢�Ŀ��п�Ŵ���MAXGROUPʱ��Ҫ������ڵ�
		{ 
			p=initial_group();
			/*
			p--
			  ��
			   -->[head]--->[a]---->[b]--->[c]-->....
			*/
			p->next=g_GroupHead; //�������p�ڵ���뵽����
			g_GroupHead=p; 
			p->cell[p->num_gp]=temp; 
			p->num_gp++; 
		} 
		
		if (0 == idletotal++%20 )//һ��20һ�� 
		{
			cout << endl;
		}
		
		//�����ʼ���� 
		printf("%04d ",temp);
		//cout<<temp<<" ";
	}
	
	cout<<endl<<"��ǰ�ܿ��п���:"<<idletotal<<endl; 
} 

void init()
{
	//��ʼ�������� 
	g_GroupHead=initial_group();
	//��ʼ��������
	idletotal=0;
	//��ʼ����ҵ����
	g_processHead=initial_process();
	//���ļ���ȡ����
	readData(); 
}

//�����ڴ�� 
void distribute() 
{ 
	char processname[20]; 
	int number; 
	int i; 
	process *p; 

	cout<<"**********************************"<<endl; 
	cout<<"�������½�����:"; 
	cin>>processname; 
	cout<<"�����ڴ����:"; 
	cin>>number; 
	
	if(number > idletotal)
	{ 
		cout<<"�����ڴ�������ڵ�ǰ���п���,����ʧ��!"<<endl; 
	} 
	else
	{ 
		p=initial_process(); 
		strcpy(p->name,processname);
		/* ���ڵ�p�������� */
		p->next=g_processHead->next; 
		g_processHead->next=p; 
		p->num_ps=number; 
		
		cout<<"����ɹ��������뵽�Ŀ��п������Ϊ:";
		
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

//�������� �������ڴ�� 
void recycle() 
{ 
	char processname[20]; 
	int i; 
	process *p,*q; 
	group *r;
	cout<<"������Ҫ�����Ľ�����:"; 
	cin>>processname; 
	
	q=g_processHead; 
	p=g_processHead->next;  
	while((p!=NULL)&&(strcmp(p->name,processname))){ //������Ѱ��ָ������ 
		q=q->next; 
		p=p->next; 
	} 	
	if(p==NULL)
	{ 
		cout<<"Sorry,û�иý���"<<endl; 
	} 
	else
	{ 
		for(i=0;i<p->num_ps;i++)//�ͷ�����鵽��Ӧ�������� 
		{ 
			if(g_GroupHead->num_gp<MAXGROUP)
			{ 
				g_GroupHead->cell[g_GroupHead->num_gp]=p->cell[i]; 
				g_GroupHead->num_gp++; 
			} 
			else{ //�½��� 
				r=initial_group(); 
				r->next=g_GroupHead; 
				g_GroupHead=r; 
				r->cell[r->num_gp]=p->cell[i]; 
				r->num_gp++; 
			} 
		} 
		idletotal+=p->num_ps; //p���̶������� 
		
		q->next=p->next;//����������ɾ��ָ�����̽ڵ� 
		delete p; 
	} 
} 
void view()
{
	cout<<"*****************************************************"<<endl;
	cout<<endl<<"�ܿ��п���:"<<idletotal<<endl;  
	cout<<"��ǰ���Ϊ:"<<(g_GroupHead->cell[g_GroupHead->num_gp]/20)+1<<endl; 
	
	cout<<"*****************************************************************************"<<endl;
	cout<<"��ǰ���ڿ��п��Ϊ:"<<endl; 
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
	  cout<<"\n������ѡ��:";
	  cout<<"1--���䣬2--���գ�3--��ʾ��Ϣ��0--�˳�\n";
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
		   cout<<"ѡ�����";
		   break;
	  }
	}
	return 0; 
}
