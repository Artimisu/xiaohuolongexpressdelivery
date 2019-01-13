#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>
#include <math.h> 
#include <time.h>

#define NUM 5 //���ÿ������ 

 //һ����Ʒ�Ľṹ�� 
struct item
{
	 char brand[20];  //Ʒ�� 
	 char id[10];
	 float in_price;
	 float out_price;
	 int storage;  //����� 
};

//��Ҫ���빺�ﳵ����Ʒ�Ľṹ�� 
struct item_node
{
	 struct item wanted;
	 int amount;
	 struct item_node *next;
};


int menu();  //�˵�������ѡ���� 
void establish(); //���������Ϣ����  
void dis_all();  //��ʾ������Ʒ�Ŀ����Ϣ
void shop_cart();  //���ﳵ���ܼ��� 
int cart_menu();  //���ﳵ���ܲ˵��� 
void add();  //���ﳵ�������Ʒ 
void display();  //��ʾ��ǰ�����б� 
void calculate();  //���� 
void sales(); //����ͳ�Ʊ���
void out_of_stock(int i,int n); //������Ʒi��ȱ����Ϣ
void showoos(); //��ʾȱ����Ϣ�� 
//void stock(); //�������� 
void buyed(); // ��ʾ�Ѿ�����Ļ���
void address(); // �û���ӵ�ַ
int rnd(int nLow,int nUpper);//����ָ����Χ�ڵ��������
double rnd(double dbLow,double dbUpper);//����ָ����Χ�ڵ����������
double ROUND(double dbA);//���ظ�������������ȡ����ĸ�����
void Init();//��������ʼ��
void Update_Trial();//���������»�����Ϣ��
void Search();//�ҵ�ĿǰΪֹ������·������
int aco();//��Ⱥ�㷨 


struct item goods[NUM];  //��Ʒ���� 
struct item_node *cart;
struct item_node *oos; //ȱ���嵥
//struct item_node *stk; //�����嵥 
struct item_node *buy; // �����Ѿ������Ĺ������Ʒ
char addr[100]; // ��ַ
const double ALPHA=1.0; //�������ӣ���Ϣ�ص���Ҫ�̶�
const double BETA=2.0;   //�������ӣ����м�������Ҫ�̶�
const double ROU=0.5; //��Ϣ�ز�������

const int N_ANT_COUNT=34; //��������
const int N_IT_COUNT=1000; //��������
const int N_CITY_COUNT=5; //��������

const double DBQ=100.0; //�ܵ���Ϣ��
const double DB_MAX=10e9; //һ����־����10��9�η�

double g_Trial[N_CITY_COUNT][N_CITY_COUNT]; //�������м���Ϣ��
double g_Distance[N_CITY_COUNT][N_CITY_COUNT]; //�������м����

//eil51.tsp������������
double x_Ary[N_CITY_COUNT];//������

double y_Ary[N_CITY_COUNT];//������

 
int main()
{
	 item_node *p;
	 printf("***********************************\n");
	 printf("  ��ӭ���빺�����ϵͳ\n");
	 printf("***********************************\n");
	 while(1)
	 {
	 	switch(menu())
	    {
		 case 1:
		 establish();break;
		 case 2:
		 dis_all();break;
		 case 3:
		 shop_cart();break;
		 case 4:
		 calculate();break;
		 case 5:
		 sales();break;
		 case 6:
		 showoos();break;
		 case 7:
		 buyed();break;
		 case 8:
		 address();break;
		 case 9: 
		 aco();break; 
		 default : 
		 {
		 	printf("��лʹ�ã��ټ�!\n");
		 	free(p);
		 }
		 
		 exit(0);
	    }
	 }
}
 
//�˵�������ѡ���� 
int menu()
{
	 char str[7];
	 int select;
	 printf("\n\n��ѡ�����ֽ��в���\n");
	 printf("1.���������Ϣ\n");
	 printf("2.��ʾ������Ʒ�Ŀ����Ϣ\n");
	 printf("3.���ﳵ\n");
	 printf("4.����\n");
	 printf("5.�鿴���ۼ�¼\n");
	 printf("6.�鿴ȱ�����\n");
	 printf("7.�鿴�����¼\n");	
	 printf("8.����û���ַ\n");
	 printf("9.Ѱ���������·��\n");
	 printf("10.�˳�\n");
	 printf("��ѡ���Ӧ����\n");
	 while(1)
	 {
	 //fflush(stdin);  //������뻺������Ϊ��ȷ����Ӱ���������ݶ�ȡ��
	                  //���еı�������֧�֣�������������
					   //rewind(stdin); 
					   //��ָ�����(һ���ļ�)��ָ��ӵ�ǰλ��Ų������(һ���ļ�)��ͷ����
					   //�൱�������˵�ǰ���̻������е����ݣ����¿�ʼ���롣  
		 rewind(stdin); 
		 gets(str);
		 select=atoi(str); //���ַ���strת����һ������
		 printf("\n");
		 if(select<1||select>10)
		 printf("�����������������:");
		 else
		 break;
	  
	 }
	 return select;
  
}
 
 //���������Ϣ�ļ�
 void establish()
 {
	 FILE *fp;
	 int i; 
	 printf("���������������Ϣ:\n");
	 printf("----------------------------\n");
	 for(i=0;i<NUM;i++)
	 {
		 printf("Ʒ��: ");
		 //fflush(stdin);
		 rewind(stdin);  
		 gets(goods[i].brand);
		 printf("����: ");
		 rewind(stdin); 
		 gets(goods[i].id);
		 printf("����: ");
		 rewind(stdin); 
		 scanf("%f",&goods[i].in_price);
		 printf("�ۼ�: ");
		 rewind(stdin); 
		 scanf("%f",&goods[i].out_price);
		 printf("����: ");
		 rewind(stdin); 
		 scanf("%d",&goods[i].storage);
		 printf("\n");
	 }
	 if((fp=fopen("goods","w"))==NULL)
	 {
		 printf("�����ļ�ʧ��.\n");
		 return ;
	 }
	 fwrite(goods,sizeof(struct item),NUM,fp);
	 //��"w"�򿪵��ļ�ֻ������ļ�д�룬���򿪵��ļ������ڣ�����ָ�����ļ����������ļ���
	 //���򿪵��ļ��Ѿ����ڣ��򽫽�ɾȥ���ļ����ؽ�һ�����ļ��� 
	 fclose(fp);
	 return ;
}
 
//��ʾ������Ʒ�Ŀ����Ϣ
void dis_all() 
{
	 int i;
	 FILE *fp;
	 fp=fopen("goods","r");
	 for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++)
	    //goods�����У�ÿ�ζ�ȡһ��item��С������Ҳ���Ƕ�ȡһ����Ʒ����
		//��������� ���� ��0��Ҳ����˵��Ʒδ���꣩����ô������ȡgoods��������� 
	 {
	 printf("---------------------------------\n");
	 printf("��Ʒ    Ʒ��     ����     �����\n");
	 printf("%s%7s%7.2f%8d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
	  
	 }
	 fclose(fp);
	 return; 
}
  
//���ﳵ���ܼ��� 
void shop_cart()
{
	 while(1)
	 {
	 	switch(cart_menu())
	    {
		 case 1:
		 display();break;
		 case 2:
		 add();break;
		 case 3:
		 return;
	    }
	 }
	 return;
}

//���ﳵ�˵��� 
int cart_menu()
{
	 char str[5];
	 int select;
	 printf("\n��ѡ�����\n");
	 printf("-----------------------\n");
	 printf("1.��ʾ��ǰ�����б�\n");
	 printf("2.�����Ʒ\n");
	 printf("3.�˳�\n");
	 printf("-----------------------\n\n");
	 while(1)
	 {
		 rewind(stdin);
		 gets(str);
		 select=atoi(str);
		 if(select<1||select>3)
		 	printf("�����������������:");
		 else
		 	break;
	 }
	 return select;
}
 
 // �����Ʒ�����ﳵ���� 
 void add()
{
	 FILE *fp;
	 int i,n,n1;
	 char str[20];
	 char choice1,choice2; //����ѡ������û���ѡ�� 
	 struct item_node *p,*p1; 
	 // do...while 
	 do
	 {
		 printf("�����빺����Ʒ�����ƻ����: ");
		 rewind(stdin); 
		 gets(str);
		 if((fp=fopen("goods","r"))==NULL)
		 {
			 printf("���ļ�ʧ��\n");  //goodsӦ���ǿ����Ϣ�ļ� 
			 continue;
		 }
		 for(i=0;fread(goods+i,sizeof(struct item),1,fp)!=0;i++)
		 {
			 if((strcmp(goods[i].brand,str)==0||strcmp(goods[i].id,str)==0)&&goods[i].storage!=0)
			 {
			 	 //strcmp(str1,str2)��str1��str2�Ա� 
				 printf("�Ѿ��ҵ��빺�����Ʒ: \n");
				 printf("---------------------\n");
				 printf("����    Ʒ��   ����   �����\n");
				 printf("%s     %6s    %3.2f    %4d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
				 printf("----------------------------------\n");
				 printf("�������빺�������: ");
				 scanf("%d",&n);
				 if(n>goods[i].storage)
				 {
					printf("��治��\n");
					n1 = n-goods[i].storage;
					out_of_stock(i,n1);
					break;
				 }
				 printf("\n�Ƿ���?(Y/N)");
				 rewind(stdin); 
				 choice1=getchar();
				 if(choice1=='Y'||choice1=='y')
				 {
					p1=(struct item_node*)malloc(sizeof(struct item_node));
					if(p1==NULL)
					{
					    printf("��������ʧ��!\n");
					    exit(1);
					}
				    p1->amount=n;
				    p1->wanted=goods[i];
				    p1->next=NULL;
				    p=cart;
				    if(cart==NULL)
				    	cart=p1;
				    else
					{
					    while(p->next!=NULL)
							p=p->next;
					    p1->next=p->next;
					    p->next=p1;
				    }
				 }
				 break;
			 }
		 }
		 if(i==NUM)
		 printf("δ�ҵ�������Ʒ\n");
		 fclose(fp);
		 printf("�Ƿ��������?(Y/N)");
		 rewind(stdin);
		 choice2=getchar();
	 }while(choice2=='Y'||choice2=='y');
}
 
 
//��ʾ��ǰ�����б�
void display()
{
	 struct item_node *p=cart;
	 if(p==NULL)
	 {
		 printf("���ﳵΪ��\n");
		 return ;
	 }
	 while(p!=NULL)
	 {
		 printf("----------------------------------\n");
		 printf("����    Ʒ��   ����   ����\n");
		 printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		 p=p->next;
	 }
}
 

 //����� 
void calculate()
{
	 float total=0,pay;
	 struct item_node *p;
	 int i;
	 FILE *fp;
	 printf("�����ǹ����嵥: \n");
	 display();
	 if((fp=fopen("goods","r"))==NULL)
	 {
		 printf("���ļ�ʧ��: \n");
		 return;
	 }
	 for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++);  //��֪����һ���������ʲô 
	 	fclose(fp);
	 p=cart;
	 while(p!=NULL)
	 {
		 total+=p->wanted.out_price*p->amount;
		 for(i=0;strcmp(goods[i].id,p->wanted.id)!=0;i++); 
		 goods[i].storage-=p->amount;//�ı����Ʒ����� 
		 p=p->next;
	 }
	 printf("�ܼ� %7.2f",total);
	 printf("\n����ʵ�����: ");
	 scanf("%f",&pay);
	 // ��ʼ��������buy,����ͷ���
	 buy=(struct item_node*)malloc(sizeof(struct item_node));
	 buy->next=NULL; 

	 // ֻ��ʵ�ʸ�������ܼ۲��ܹ�����
	 if(pay>=total)
	 {
	 	printf("ʵ��:   %7.2f ����:   %7.2f",pay,pay-total);
	 	// ����������
	 	p=cart;
	 	while(p!=NULL)
	 	{
	 		struct item_node *s;
	 		s=(struct item_node*)malloc(sizeof(struct item_node));
	 		s->wanted=p->wanted;
	 		s->amount=p->amount;
	 		s->next=NULL;
	 		// ͷ�巨
	 		if(buy->next=NULL)
	 		{
	 			buy->next=s;
	 		}
	 		else
	 		{
	 			s->next=buy->next;
	 			buy->next=s;
	 		}
	 		p=p->next;

	 	}
	 }
	 else
	 {
	 	printf("����");
	 }
	 if((fp=fopen("goods","w"))==NULL)
	 {
		 printf("���ļ�ʧ��.\n");
		 return;
	 }
	 fwrite(goods,sizeof(struct item),NUM,fp);
	 fclose(fp);
}

//��ʾ���۱� 
void sales()
{
	 struct item_node *p=cart;
	 float total=0;
	 if(p==NULL)
	 {
		 printf("�������۶�Ϊ:0\n");
		 return ;
	 }
	 while(p!=NULL)
	 {
		 printf("----------------------------------\n");
		 printf("����    Ʒ��   ����   ����\n");
		 printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		 total+=p->wanted.out_price*p->amount;
		 p=p->next;
	 }
	 printf("----------------------------------\n");
	 printf("�����ܹ������۶�Ϊ%��%7.2f\n",total);
	 return; 
}

//������Ʒi��ȱ����Ϣ
void out_of_stock(int i,int n)
{
	struct item_node *oos1,*oos2;
	FILE *fp;
	fp=fopen("goods","r");
	oos2=(struct item_node*)malloc(sizeof(struct item_node));
	if(oos2==NULL)
	{
	    printf("ȱ����Ϣ����ʧ��!\n");
	    exit(1);
	}
    oos2->amount=n;
    oos2->wanted=goods[i];
    oos2->next=NULL;
    oos1=oos;
    if(oos==NULL)
    	oos=oos2;
    else
	{
	    while(oos1->next!=NULL)
			oos1=oos1->next;
	    oos2->next=oos1->next;
	    oos1->next=oos2;
    }
    fclose(fp);
    return;
} 

//��ʾȱ���嵥 
void showoos()
{
	 struct item_node *p=oos;
	 char chose,id[20];
	 int num,i; 
	 FILE *fp;
 	 fp=fopen("goods","r+");
	 if(p==NULL)
	 {
		 printf("��ȱ���嵥\n");
		 return ;
	 }
	 while(p!=NULL)
	 {
		 printf("----------------------------------\n");
		 printf("����    Ʒ��   ����   ����\n");
		 printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		 p=p->next;
	 }
	 printf("----------------------------------\n");
	 printf("�Ƿ���Ҫ��������Y/N��");
	     rewind(stdin); 
		 chose=getchar();
		 if(chose=='Y'||chose=='y')
		 {
		 	printf("������Ҫ��������Ʒ���ź�������\n");
		 	rewind(stdin); 
		 	gets(id);
			rewind(stdin); 
			scanf("%d", &num); 
			for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++)
			{
				if(strcmp(goods[i].id,id)==0)
					goods[i].storage = num;
			}
		 }
	 fwrite(goods,sizeof(struct item),NUM,fp); 
	 fclose(fp);
	 return; 
}
 
void address()
{
	printf("���������ļ�ͥ��ַ��\n");
	gets(addr);
	printf("��������ͥ��ַ��%s\n",addr);
	printf("�Ƿ�ȷ�ϣ���Y/N��:");
	char ch=getchar();
	if( (ch=='y') ||( ch=='Y') )
	{
		printf("�ɹ���");
		addr[99]='0';
	}
	else
	{
		printf("�Ƿ������д��\n");
		char ch=getchar();
		if((ch=='y') ||( ch=='Y'))
		{
			printf("�������������ļ�ͥ��ַ��\n");
			address();
		}
		else
		{
			return;
		}
	}
}

// ��ʾ�Ѿ�������嵥
void buyed()
{
	struct item_node *p=buy->next;
	float total=0;
	printf("---------------------------------\n");
	printf("|            �����嵥           |\n");
	printf("---------------------------------\n");

	if(p==NULL)
	{
		printf("������δ�����Ʒ��~\n");
		return ;
	}
	while(p!=NULL)
	{
		printf("  ����  Ʒ��   ����   ����\n");
		printf("  %3s   %3s %7.2f %6d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		total+=p->wanted.out_price*p->amount;
		p=p->next;
	}
	printf("---------------------------------\n");
	printf("�������ܹ������ˣ�%7.2f\n",total);
 	printf("---------------------------------\n");
 	if(addr[99]=='0')
	{
		printf("�����ջ���ַ�ǣ�%s\n",addr);
 	}
 	else
 	{
		printf("�����ջ���ַ��δ��ȫ�ޣ�\n");
 	}
 	printf("---------------------------------\n");


	return; 
} 

//-----------------------����Ϊ���Ͳ���------------------------------- 

//����ָ����Χ�ڵ��������
int rnd(int nLow,int nUpper)
{
    return nLow+(nUpper-nLow)*rand()/(RAND_MAX+1);
}

//����ָ����Χ�ڵ����������
double rnd(double dbLow,double dbUpper)
{
    double dbTemp=rand()/((double)RAND_MAX+1.0);
    return dbLow+dbTemp*(dbUpper-dbLow);
}

//���ظ�������������ȡ����ĸ�����
double ROUND(double dbA)
{
    return (double)((int)(dbA+0.5));
}

//����������
class CAnt
{
public:
    CAnt(void);
    ~CAnt(void);

public:

    int m_nPath[N_CITY_COUNT]; //�����ߵ�·��,·��
    double m_dbPath_Length; //�����߹���·�����ȣ�����

    int m_nAllowed_City[N_CITY_COUNT]; //δ���ʹ��ĳ���
    int m_nCur_City_No; //��ǰ���ڳ��б��
    int m_nMoved_City_Count; //�Ѿ�ȥ���ĳ������������ɱ�

public:

    int Choose_Next_City(); //ѡ����һ������
    void Init(); //��ʼ��
    void Move(); //�����ڳ��м��ƶ�
    void Search(); //����·��
    void Cal_Path_Length(); //���������߹���·������

};

//���캯��
CAnt::CAnt(void)
{
}

//��������
CAnt::~CAnt(void)
{
}


//��������ʼ��
void CAnt::Init()
{

    for (int i=0;i<N_CITY_COUNT;i++)
    {
        m_nAllowed_City[i]=1; //����ȫ������Ϊû��ȥ��,m_nAllowed_City[i]��ʾδ���ʹ��ĳ���
        m_nPath[i]=0; //�����ߵ�·��ȫ������Ϊ0
    }

    //�����߹���·����������Ϊ0
    m_dbPath_Length=0.0; 

    //���ѡ��һ����������
    m_nCur_City_No=rnd(0,N_CITY_COUNT);
	//rnd��������һ��С��1�����ڻ����0��ֵ
    //m_nCur_City_No ��ǰ���ڳ��б��

    //�ѳ������б�����·��������
    m_nPath[0]=m_nCur_City_No;

    //��ʶ��������Ϊ�Ѿ�ȥ����
    m_nAllowed_City[m_nCur_City_No]=0; 
    //m_nAllowed_City[i]δ���ʹ��ĳ���,m_nCur_City_No��ǰ���ڳ��б��

    //�Ѿ�ȥ���ĳ�����������Ϊ1
    m_nMoved_City_Count=1; 

}


//������ѡ����һ������
//����ֵ Ϊ���б��
int CAnt::Choose_Next_City()
{
    int nSelected_City=-1; //���ؽ��������ʱ��������Ϊ-1
//======================================================
    //���㵱ǰ���к�ûȥ���ĳ���֮�����Ϣ���ܺ�

    double dbTotal=0.0;//��������Ϊ0    
    double prob[N_CITY_COUNT]; //����������б�ѡ�еĸ���

    for (int i=0;i<N_CITY_COUNT;i++)
    {
        if (m_nAllowed_City[i] == 1) //m_nAllowed_City[i]δ���ʹ��ĳ���,��ʾ����ûȥ��
        {
            prob[i]=pow(g_Trial[m_nCur_City_No][i],ALPHA)*pow(1.0/g_Distance[m_nCur_City_No][i],BETA); //�ó��к͵�ǰ���м����Ϣ�أ���������
            //pow(float x, float y) ����x��y����
			dbTotal=dbTotal+prob[i]; //�ۼ���Ϣ�أ��õ��ܺ�
        }
        else //�������ȥ���ˣ����䱻ѡ�еĸ���ֵΪ0
        {
            prob[i]=0.0;
        }
    }
//======================================================
    //��������ѡ��
    double dbTemp=0.0;//��ѡ���0~1֮����������Ϊ���̶ĵ�����
    if (dbTotal > 0.0) //�ܵ���Ϣ��ֵ����0
    {
        dbTemp=rnd(0.0,dbTotal); //ȡһ�������

        for (int i=0;i<N_CITY_COUNT;i++)
        {
            if (m_nAllowed_City[i] == 1) //����ûȥ��
            {
                dbTemp=dbTemp-prob[i]; //��������൱��ת�����̣����������ѡ����Ϥ����ϸ����һ��
                if (dbTemp < 0.0) //����ֹͣת�������³��б�ţ�ֱ������ѭ��
                 {
                    nSelected_City=i;
                    break;
                }
            }
        }
    }

    //======================================================
    //������м����Ϣ�طǳ�С(С����double�ܹ���ʾ����С�����ֻ�ҪС )
    //��ô���ڸ�����������ԭ���������ĸ����ܺͿ���Ϊ0
    //����־�������������û�г��б�ѡ�����
    //��������������Ͱѵ�һ��ûȥ���ĳ�����Ϊ���ؽ��
    //���⻰���տ�ʼ����ʱ��������δ����������Һܳ�ʱ�䣬�벻ͨΪ��Ҫ����δ��룬�����Ÿ������
    if (nSelected_City == -1)
    {
        for (int i=0;i<N_CITY_COUNT;i++)
        {
            if (m_nAllowed_City[i] == 1) //����ûȥ��
            {
                nSelected_City=i;
                break;
            }
        }
    }
//======================================================
    //���ؽ�������ǳ��еı��
    return nSelected_City;
}


//�����������ڳ��м��ƶ�
void CAnt::Move()
{
    int nCity_No=Choose_Next_City(); //ѡ����һ������,nCity_No��һ�����еı��

    m_nPath[m_nMoved_City_Count]=nCity_No; //����һ�����б���������ߵ�·��
    m_nAllowed_City[nCity_No]=0;//����һ���������ó��Ѿ����ʹ���
    m_nCur_City_No=nCity_No; //�ı䵱ǰ���ڳ���Ϊ��ѡ��ĳ��У���һ���ģ�
	//m_nCur_City_No ��ǰ���ڳ��б��
    m_nMoved_City_Count++; //�Ѿ�ȥ���ĳ���������1
}

//���������Ͻ�������һ��
void CAnt::Search()
{
    Init(); //���ú�������������ǰ���ȳ�ʼ��,

    //�������ȥ���ĳ�������С�ڳ����������ͼ����ƶ�
    while (m_nMoved_City_Count < N_CITY_COUNT)
    {
        Move();//���ú���
    }

    //�������������߹���·������
    Cal_Path_Length();//���ú���
}


//���������������߹���·������
void CAnt::Cal_Path_Length()
{

    m_dbPath_Length=0.0; //��ʼ�����Ȱ�·��������0
    int m=0;
    int n=0;

    for (int i=1;i<N_CITY_COUNT;i++)
    {
        m=m_nPath[i];//�ӽ���ǰ����֮����һ�ε�·��
        n=m_nPath[i-1];//�ӽ���ǰ����֮ǰ����һ�ε�·��
        m_dbPath_Length=m_dbPath_Length+g_Distance[m][n];
    }

    //���ϴ������з��س������еľ���
    n=m_nPath[0];
    m_dbPath_Length=m_dbPath_Length+g_Distance[m][n];    

}

//tsp��
class CTsp
{
public:
    CTsp(void);
    ~CTsp(void);

public:
    CAnt m_cAntAry[N_ANT_COUNT]; //��������
    CAnt m_cBestAnt; //����һ�����ϱ����������������������е����Ž��
                                        //�����ϲ�����������ֻ�������������Ž��

public:

    //��ʼ������
    void InitData(); 

    //��ʼ����
    void Search(); 

    //���»�����Ϣ��
    void Update_Trial();


};


//���캯��
CTsp::CTsp(void){}
CTsp::~CTsp(void){}

//��������ʼ������
void CTsp::InitData() 
{
    //�Ȱ��������ϵ�·���������ó�һ���ܴ��ֵ
    m_cBestAnt.m_dbPath_Length=DB_MAX; 
    //�����������м����
    double dbTemp=0.0;//�������м����
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            dbTemp=(x_Ary[i]-x_Ary[j])*(x_Ary[i]-x_Ary[j])+(y_Ary[i]-y_Ary[j])*(y_Ary[i]-y_Ary[j]);//ƽ��
            dbTemp=pow(dbTemp,0.5);//����
            g_Distance[i][j]=ROUND(dbTemp);//Round��������ָ����С��λ������������������Ľ��,������ľ������������뱣��
        }
    }

    //��ʼ��������Ϣ�أ��Ȱѳ��м����Ϣ�����ó�һ��
    //�������ó�1.0�����óɶ��ٶԽ��Ӱ�첻��̫�󣬶��㷨�����ٶ���ЩӰ��
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            g_Trial[i][j]=1.0;//i������j����֮�����Ϣ��
        }
    }

}
//���������»�����Ϣ��?????????????????????????????????????????????????????????????????????
void CTsp::Update_Trial()
{
    //��ʱ���飬�����ֻ�������������м������µ���Ϣ��

    double dbTemp_Ary[N_CITY_COUNT][N_CITY_COUNT];//ÿ���������������м������µ���Ϣ��
    memset(dbTemp_Ary,0,sizeof(dbTemp_Ary)); //��ȫ������Ϊ0
	//memset �������ڴ渳ֵ����,������ĳһ���ڴ�ռ���и�ֵ��

    //���������ӵ���Ϣ��,���浽��ʱ������

    int m=0;//m,n��ʾ����
    int n=0;
    for (int i=0;i<N_ANT_COUNT;i++) //����ÿֻ�������µ���Ϣ��,N_ANT_COUNT������
    {
            for (int j=1;j<N_CITY_COUNT;j++)
            {
                m=m_cAntAry[i].m_nPath[j];
                n=m_cAntAry[i].m_nPath[j-1];
                dbTemp_Ary[n][m]=dbTemp_Ary[n][m]+DBQ/m_cAntAry[i].m_dbPath_Length;
                dbTemp_Ary[m][n]=dbTemp_Ary[n][m];
            }

            //�����кͿ�ʼ����֮�����Ϣ��
            n=m_cAntAry[i].m_nPath[0];
            dbTemp_Ary[n][m]=dbTemp_Ary[n][m]+DBQ/m_cAntAry[i].m_dbPath_Length;
            dbTemp_Ary[m][n]=dbTemp_Ary[n][m];

    }

    //======================================================
    //���»�����Ϣ��
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            g_Trial[i][j]=g_Trial[i][j]*ROU+dbTemp_Ary[i][j]; //���µĻ�����Ϣ�� = �������Ϣ�� + �����µ���Ϣ��
        }
    }

}


void CTsp::Search()
{

    char cBuf[256]; //��ӡ��Ϣ��

    //�ڵ��������ڽ���ѭ��
    for (int i=0;i<N_IT_COUNT;i++) //N_IT_COUNT=1000; //��������
    {
        //ÿֻ��������һ��
        for (int j=0;j<N_ANT_COUNT;j++)
        {
            m_cAntAry[j].Search(); 
        }

        //������ѽ��
        for (int j=0;j<N_ANT_COUNT;j++)
        {
            if (m_cAntAry[j].m_dbPath_Length < m_cBestAnt.m_dbPath_Length)
            {
                m_cBestAnt=m_cAntAry[j];
            }
        }

        //���»�����Ϣ��
        Update_Trial();

        //���ĿǰΪֹ�ҵ�������·���ĳ���
        //sprintf(cBuf,"\n[%d] %.0f",i+1,m_cBestAnt.m_dbPath_Length);
        //printf(cBuf);
    }

}

int aco()
{
	int i;
	printf("��������к����꣺\n");
	for (i = 0; i <= N_CITY_COUNT; i++)
	{
		scanf("%lf", &x_Ary[i]);

	}
	printf("��������������꣺\n");
	for (i = 0; i <= N_CITY_COUNT; i++)
	{
		scanf("%lf", &y_Ary[i]);
	}
    //�õ�ǰʱ����ʼ��������ӣ���ֹÿ�����еĽ������ͬ
    //time_t tm;
    //time(&tm);
    //unsigned int nSeed=(unsigned int)tm;
    //srand(nSeed);

    //��ʼ����
    CTsp tsp;

    tsp.InitData(); //��ʼ��
    tsp.Search(); //��ʼ����

    //������
    printf("\n��̾��������˳��Ϊ:\n");


    char cBuf[128];
    for (i=0;i<N_CITY_COUNT;i++)
    {
        sprintf(cBuf,"%02d ",tsp.m_cBestAnt.m_nPath[i]+1);//sprintfָ�����ַ�����ʽ�������Ҫ�����ǰѸ�ʽ��������д��ĳ���ַ�����
        if (i % 20 == 0)
        {
            printf("\n");
        }
        printf(cBuf);
    }

    return 0;
}
