#include <iostream>
#include <cstring>
#include <conio.h>
#include <time.h>
using namespace std;

const int cylinder=3,track=2,sector=4;//���桢�ŵ�������� �� 
#define SIZE 100//������ 
const int M=cylinder,N=track*sector;

struct process//process˳������ݽṹ���洢������������Ϣ 
{
	char name[20];
	int c[SIZE],t[SIZE],s[SIZE];
	int n;
};
process processtable[SIZE];//process��ʽ�� 
int ppointer=-1;//processָ�� 

class bitmap//λʾͼ �ṹ�� 
{
	public:
		int bitmap[M][N];
		void Initbitmap();//��ʼ��λʾͼ
		bool spaceisok(int n);//λʾͼ�����ж�
		void displaybitmap();//��ӡ 
};
bitmap bm;//ȫ��λʾͼ��Ϊ���н��̹��� 


void bitmap::Initbitmap()
{
	int i,j;
	 cout<<"**********************************************\n";
 	 cout<<"		λʾͼ��ʼ��\n";
 	 cout<<"**********************************************\n";
 	for(i=0;i<M;i++)
 	{
 		for(j=0;j<N;j++)
 		{
 			bitmap[i][j]=0;
 		}
 	}
 //	getchar();
 	displaybitmap();//��ʼ����λʾͼ 
 	getchar();
 //system("cls");
}
bool bitmap::spaceisok(int n)//�ж�λʾͼ����������Ƿ��㹻 
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
void bitmap::displaybitmap()//��ӡλʾͼ
{
	int i,j;
	cout<<"\n��ǰλʾͼ��Ϣ���£�\n";
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
		cout<<"\n��δ������̿ռ�\n";
		return;
	}
	
	else
	{
		cout<<"\n��ǰ������Ϣ���£�\n";
		cout<<"\n#######################################################";
		cout<<"\n������\t\t������������\n";
		
	//	cout<<"\n������\t\t������������\t\t�������Ϣ\n";
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

void distribute(char name[20],int n)//����
{
  int i,j;
  int count=0;/*������*/
  	for(i=0;i<=ppointer;i++)//processtable���������ָ������ 
	{
		if(!strcmp(processtable[i].name,name))
		{
			cout<<"\n�������ظ��������������\n";
			goto end; 
		}
	}
  
  if(!bm.spaceisok(n))
  {
	cout<<"�ռ䲻�㣬�Ҳ���"<<n<<"������飬����ʧ��!";
	return;
  }
  ppointer++;
  strcpy(processtable[ppointer].name,name);//���������鸳������ 
  processtable[ppointer].n=n;//�������
  for(i=0;i<M;i++)//��ά�������������������� 
  {
	  for(j=0;j<N;j++)
		if(bm.bitmap[i][j]==0) 
		{
			processtable[ppointer].c[count]=i;/*�����*/
			processtable[ppointer].t[count]=j/4;/*�ŵ���*/
			processtable[ppointer].s[count]=j%4;/*�����¼��*/
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

void recycle(char name[20])//�����ڴ� 
{
	int i,j,flag=0;
	for(i=0;i<=ppointer;i++)//processtable���������ָ������ 
	{
		if(!strcmp(processtable[i].name,name))
		{
			for(j=0;j<processtable[i].n;j++)
				bm.bitmap[processtable[i].c[j]][4*processtable[i].t[j]+processtable[i].s[j]]=0;//λʾͼ��Ӧ���� 
		
			for(int k=i;k<=ppointer-i;k++)//process�����ƶ�
			{
				strcpy(processtable[k].name,processtable[k+1].name);//ɾ����ǰ�� 
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
			cout<<"\n�ҵ����̣�������ϡ�\n";
		}
	}
	if(flag==0)
		cout<<"\nδ�ҵ�������Ϊ"<<name<<"�Ľ��̣���������δ�˽��̷��������\n";
}

int main()
{
	int choice,n;char name[20];
	bm.Initbitmap();
	while(1)
	{  
	  getch();
	//  system("cls");
	  cout<<"\n������ѡ��:";
	  cout<<"1--���䣬2--���գ�3--��ʾλʾͼ��0--�˳�\n";
	  cin>>choice;
	  switch(choice)
	  {
	   case 1:
		   {
			   cout<<"���������������Ҫ��������������";
			   cin>>name>>n;
			   distribute(name,n);
			   break;
		   }
	   case 2:
		   {
			   cout<<"����Ҫ���յĽ�������";
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
		   cout<<"ѡ�����";
		   break;
	  }
	}
	return 0; 
}
