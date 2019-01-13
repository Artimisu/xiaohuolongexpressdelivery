#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>
#include <math.h> 
#include <time.h>

#define NUM 5 //设置库存容量 

 //一个商品的结构体 
struct item
{
	 char brand[20];  //品牌 
	 char id[10];
	 float in_price;
	 float out_price;
	 int storage;  //库存量 
};

//想要加入购物车的物品的结构体 
struct item_node
{
	 struct item wanted;
	 int amount;
	 struct item_node *next;
};


int menu();  //菜单函数，选择功能 
void establish(); //建立库存信息函数  
void dis_all();  //显示所有商品的库存信息
void shop_cart();  //购物车功能集合 
int cart_menu();  //购物车功能菜单键 
void add();  //向购物车中添加商品 
void display();  //显示当前购物列表 
void calculate();  //付款 
void sales(); //销售统计表函数
void out_of_stock(int i,int n); //生成物品i的缺货消息
void showoos(); //显示缺货信息表 
//void stock(); //进货函数 
void buyed(); // 显示已经购买的货物
void address(); // 用户添加地址
int rnd(int nLow,int nUpper);//返回指定范围内的随机整数
double rnd(double dbLow,double dbUpper);//返回指定范围内的随机浮点数
double ROUND(double dbA);//返回浮点数四舍五入取整后的浮点数
void Init();//函数：初始化
void Update_Trial();//函数：更新环境信息素
void Search();//找到目前为止的最优路径长度
int aco();//蚁群算法 


struct item goods[NUM];  //商品数组 
struct item_node *cart;
struct item_node *oos; //缺货清单
//struct item_node *stk; //进货清单 
struct item_node *buy; // 保存已经结算后的购买的商品
char addr[100]; // 地址
const double ALPHA=1.0; //启发因子，信息素的重要程度
const double BETA=2.0;   //期望因子，城市间距离的重要程度
const double ROU=0.5; //信息素残留参数

const int N_ANT_COUNT=34; //蚂蚁数量
const int N_IT_COUNT=1000; //迭代次数
const int N_CITY_COUNT=5; //城市数量

const double DBQ=100.0; //总的信息素
const double DB_MAX=10e9; //一个标志数，10的9次方

double g_Trial[N_CITY_COUNT][N_CITY_COUNT]; //两两城市间信息素
double g_Distance[N_CITY_COUNT][N_CITY_COUNT]; //两两城市间距离

//eil51.tsp城市坐标数据
double x_Ary[N_CITY_COUNT];//横坐标

double y_Ary[N_CITY_COUNT];//纵坐标

 
int main()
{
	 item_node *p;
	 printf("***********************************\n");
	 printf("  欢迎进入购物管理系统\n");
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
		 	printf("感谢使用，再见!\n");
		 	free(p);
		 }
		 
		 exit(0);
	    }
	 }
}
 
//菜单函数，选择功能 
int menu()
{
	 char str[7];
	 int select;
	 printf("\n\n请选择数字进行操作\n");
	 printf("1.建立库存信息\n");
	 printf("2.显示所有商品的库存信息\n");
	 printf("3.购物车\n");
	 printf("4.结算\n");
	 printf("5.查看销售记录\n");
	 printf("6.查看缺货情况\n");
	 printf("7.查看购买记录\n");	
	 printf("8.添加用户地址\n");
	 printf("9.寻找最短配送路径\n");
	 printf("10.退出\n");
	 printf("请选择对应数字\n");
	 while(1)
	 {
	 //fflush(stdin);  //清空输入缓冲区，为了确保不影响后面的数据读取，
	                  //但有的编译器不支持，用下面代码代替
					   //rewind(stdin); 
					   //“指向键盘(一个文件)的指针从当前位置挪到键盘(一个文件)开头”，
					   //相当于抛弃了当前键盘缓冲区中的数据，重新开始输入。  
		 rewind(stdin); 
		 gets(str);
		 select=atoi(str); //将字符串str转换成一个整数
		 printf("\n");
		 if(select<1||select>10)
		 printf("输入错误，请重新输入:");
		 else
		 break;
	  
	 }
	 return select;
  
}
 
 //建立库存信息文件
 void establish()
 {
	 FILE *fp;
	 int i; 
	 printf("请依次输入货物信息:\n");
	 printf("----------------------------\n");
	 for(i=0;i<NUM;i++)
	 {
		 printf("品名: ");
		 //fflush(stdin);
		 rewind(stdin);  
		 gets(goods[i].brand);
		 printf("货号: ");
		 rewind(stdin); 
		 gets(goods[i].id);
		 printf("进价: ");
		 rewind(stdin); 
		 scanf("%f",&goods[i].in_price);
		 printf("售价: ");
		 rewind(stdin); 
		 scanf("%f",&goods[i].out_price);
		 printf("数量: ");
		 rewind(stdin); 
		 scanf("%d",&goods[i].storage);
		 printf("\n");
	 }
	 if((fp=fopen("goods","w"))==NULL)
	 {
		 printf("创建文件失败.\n");
		 return ;
	 }
	 fwrite(goods,sizeof(struct item),NUM,fp);
	 //用"w"打开的文件只能向该文件写入，若打开的文件不存在，则以指定的文件名建立该文件，
	 //若打开的文件已经存在，则将将删去该文件，重建一个新文件。 
	 fclose(fp);
	 return ;
}
 
//显示所有商品的库存信息
void dis_all() 
{
	 int i;
	 FILE *fp;
	 fp=fopen("goods","r");
	 for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++)
	    //goods数组中，每次读取一个item大小的区域（也就是读取一个商品），
		//如果读到的 区域 ≠0（也就是说商品未读完），那么继续读取goods数组的数据 
	 {
	 printf("---------------------------------\n");
	 printf("货品    品名     单价     库存量\n");
	 printf("%s%7s%7.2f%8d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
	  
	 }
	 fclose(fp);
	 return; 
}
  
//购物车功能集合 
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

//购物车菜单键 
int cart_menu()
{
	 char str[5];
	 int select;
	 printf("\n请选择操作\n");
	 printf("-----------------------\n");
	 printf("1.显示当前购物列表\n");
	 printf("2.添加商品\n");
	 printf("3.退出\n");
	 printf("-----------------------\n\n");
	 while(1)
	 {
		 rewind(stdin);
		 gets(str);
		 select=atoi(str);
		 if(select<1||select>3)
		 	printf("输入错误，请重新输入:");
		 else
		 	break;
	 }
	 return select;
}
 
 // 添加物品到购物车函数 
 void add()
{
	 FILE *fp;
	 int i,n,n1;
	 char str[20];
	 char choice1,choice2; //两个选项框中用户的选择 
	 struct item_node *p,*p1; 
	 // do...while 
	 do
	 {
		 printf("输入想购买物品的名称或货号: ");
		 rewind(stdin); 
		 gets(str);
		 if((fp=fopen("goods","r"))==NULL)
		 {
			 printf("打开文件失败\n");  //goods应该是库存信息文件 
			 continue;
		 }
		 for(i=0;fread(goods+i,sizeof(struct item),1,fp)!=0;i++)
		 {
			 if((strcmp(goods[i].brand,str)==0||strcmp(goods[i].id,str)==0)&&goods[i].storage!=0)
			 {
			 	 //strcmp(str1,str2)：str1和str2对比 
				 printf("已经找到想购买的物品: \n");
				 printf("---------------------\n");
				 printf("货号    品名   单价   库存量\n");
				 printf("%s     %6s    %3.2f    %4d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
				 printf("----------------------------------\n");
				 printf("请输入想购买的数量: ");
				 scanf("%d",&n);
				 if(n>goods[i].storage)
				 {
					printf("库存不足\n");
					n1 = n-goods[i].storage;
					out_of_stock(i,n1);
					break;
				 }
				 printf("\n是否购买?(Y/N)");
				 rewind(stdin); 
				 choice1=getchar();
				 if(choice1=='Y'||choice1=='y')
				 {
					p1=(struct item_node*)malloc(sizeof(struct item_node));
					if(p1==NULL)
					{
					    printf("购买申请失败!\n");
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
		 printf("未找到所需物品\n");
		 fclose(fp);
		 printf("是否继续购物?(Y/N)");
		 rewind(stdin);
		 choice2=getchar();
	 }while(choice2=='Y'||choice2=='y');
}
 
 
//显示当前购物列表
void display()
{
	 struct item_node *p=cart;
	 if(p==NULL)
	 {
		 printf("购物车为空\n");
		 return ;
	 }
	 while(p!=NULL)
	 {
		 printf("----------------------------------\n");
		 printf("货号    品名   单价   数量\n");
		 printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		 p=p->next;
	 }
}
 

 //付款函数 
void calculate()
{
	 float total=0,pay;
	 struct item_node *p;
	 int i;
	 FILE *fp;
	 printf("以下是购物清单: \n");
	 display();
	 if((fp=fopen("goods","r"))==NULL)
	 {
		 printf("打开文件失败: \n");
		 return;
	 }
	 for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++);  //不知道这一句的意义是什么 
	 	fclose(fp);
	 p=cart;
	 while(p!=NULL)
	 {
		 total+=p->wanted.out_price*p->amount;
		 for(i=0;strcmp(goods[i].id,p->wanted.id)!=0;i++); 
		 goods[i].storage-=p->amount;//改变该商品库存量 
		 p=p->next;
	 }
	 printf("总计 %7.2f",total);
	 printf("\n输入实付金额: ");
	 scanf("%f",&pay);
	 // 初始化单链表buy,设立头结点
	 buy=(struct item_node*)malloc(sizeof(struct item_node));
	 buy->next=NULL; 

	 // 只有实际付款大于总价才能够购买
	 if(pay>=total)
	 {
	 	printf("实付:   %7.2f 找零:   %7.2f",pay,pay-total);
	 	// 遍历单链表
	 	p=cart;
	 	while(p!=NULL)
	 	{
	 		struct item_node *s;
	 		s=(struct item_node*)malloc(sizeof(struct item_node));
	 		s->wanted=p->wanted;
	 		s->amount=p->amount;
	 		s->next=NULL;
	 		// 头插法
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
	 	printf("金额不足");
	 }
	 if((fp=fopen("goods","w"))==NULL)
	 {
		 printf("打开文件失败.\n");
		 return;
	 }
	 fwrite(goods,sizeof(struct item),NUM,fp);
	 fclose(fp);
}

//显示销售表 
void sales()
{
	 struct item_node *p=cart;
	 float total=0;
	 if(p==NULL)
	 {
		 printf("本月销售额为:0\n");
		 return ;
	 }
	 while(p!=NULL)
	 {
		 printf("----------------------------------\n");
		 printf("货号    品名   单价   数量\n");
		 printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		 total+=p->wanted.out_price*p->amount;
		 p=p->next;
	 }
	 printf("----------------------------------\n");
	 printf("本月总共的销售额为%：%7.2f\n",total);
	 return; 
}

//生成物品i的缺货消息
void out_of_stock(int i,int n)
{
	struct item_node *oos1,*oos2;
	FILE *fp;
	fp=fopen("goods","r");
	oos2=(struct item_node*)malloc(sizeof(struct item_node));
	if(oos2==NULL)
	{
	    printf("缺货消息生成失败!\n");
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

//显示缺货清单 
void showoos()
{
	 struct item_node *p=oos;
	 char chose,id[20];
	 int num,i; 
	 FILE *fp;
 	 fp=fopen("goods","r+");
	 if(p==NULL)
	 {
		 printf("无缺货清单\n");
		 return ;
	 }
	 while(p!=NULL)
	 {
		 printf("----------------------------------\n");
		 printf("货号    品名   单价   数量\n");
		 printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		 p=p->next;
	 }
	 printf("----------------------------------\n");
	 printf("是否想要补货？（Y/N）");
	     rewind(stdin); 
		 chose=getchar();
		 if(chose=='Y'||chose=='y')
		 {
		 	printf("输入想要补货的商品货号和数量：\n");
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
	printf("请输入您的家庭地址：\n");
	gets(addr);
	printf("这是您家庭地址：%s\n",addr);
	printf("是否确认？（Y/N）:");
	char ch=getchar();
	if( (ch=='y') ||( ch=='Y') )
	{
		printf("成功！");
		addr[99]='0';
	}
	else
	{
		printf("是否继续填写？\n");
		char ch=getchar();
		if((ch=='y') ||( ch=='Y'))
		{
			printf("请重新输入您的家庭地址：\n");
			address();
		}
		else
		{
			return;
		}
	}
}

// 显示已经购买的清单
void buyed()
{
	struct item_node *p=buy->next;
	float total=0;
	printf("---------------------------------\n");
	printf("|            购物清单           |\n");
	printf("---------------------------------\n");

	if(p==NULL)
	{
		printf("本月您未购买货品噢~\n");
		return ;
	}
	while(p!=NULL)
	{
		printf("  货号  品名   单价   数量\n");
		printf("  %3s   %3s %7.2f %6d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		total+=p->wanted.out_price*p->amount;
		p=p->next;
	}
	printf("---------------------------------\n");
	printf("本月您总共购买了：%7.2f\n",total);
 	printf("---------------------------------\n");
 	if(addr[99]=='0')
	{
		printf("您的收货地址是：%s\n",addr);
 	}
 	else
 	{
		printf("您的收货地址尚未补全噢！\n");
 	}
 	printf("---------------------------------\n");


	return; 
} 

//-----------------------以下为配送部分------------------------------- 

//返回指定范围内的随机整数
int rnd(int nLow,int nUpper)
{
    return nLow+(nUpper-nLow)*rand()/(RAND_MAX+1);
}

//返回指定范围内的随机浮点数
double rnd(double dbLow,double dbUpper)
{
    double dbTemp=rand()/((double)RAND_MAX+1.0);
    return dbLow+dbTemp*(dbUpper-dbLow);
}

//返回浮点数四舍五入取整后的浮点数
double ROUND(double dbA)
{
    return (double)((int)(dbA+0.5));
}

//定义蚂蚁类
class CAnt
{
public:
    CAnt(void);
    ~CAnt(void);

public:

    int m_nPath[N_CITY_COUNT]; //蚂蚁走的路径,路径
    double m_dbPath_Length; //蚂蚁走过的路径长度，长度

    int m_nAllowed_City[N_CITY_COUNT]; //未访问过的城市
    int m_nCur_City_No; //当前所在城市编号
    int m_nMoved_City_Count; //已经去过的城市数量，禁忌表

public:

    int Choose_Next_City(); //选择下一个城市
    void Init(); //初始化
    void Move(); //蚂蚁在城市间移动
    void Search(); //搜索路径
    void Cal_Path_Length(); //计算蚂蚁走过的路径长度

};

//构造函数
CAnt::CAnt(void)
{
}

//析构函数
CAnt::~CAnt(void)
{
}


//函数：初始化
void CAnt::Init()
{

    for (int i=0;i<N_CITY_COUNT;i++)
    {
        m_nAllowed_City[i]=1; //设置全部城市为没有去过,m_nAllowed_City[i]表示未访问过的城市
        m_nPath[i]=0; //蚂蚁走的路径全部设置为0
    }

    //蚂蚁走过的路径长度设置为0
    m_dbPath_Length=0.0; 

    //随机选择一个出发城市
    m_nCur_City_No=rnd(0,N_CITY_COUNT);
	//rnd函数返回一个小于1但大于或等于0的值
    //m_nCur_City_No 当前所在城市编号

    //把出发城市保存入路径数组中
    m_nPath[0]=m_nCur_City_No;

    //标识出发城市为已经去过了
    m_nAllowed_City[m_nCur_City_No]=0; 
    //m_nAllowed_City[i]未访问过的城市,m_nCur_City_No当前所在城市编号

    //已经去过的城市数量设置为1
    m_nMoved_City_Count=1; 

}


//函数：选择下一个城市
//返回值 为城市编号
int CAnt::Choose_Next_City()
{
    int nSelected_City=-1; //返回结果，先暂时把其设置为-1
//======================================================
    //计算当前城市和没去过的城市之间的信息素总和

    double dbTotal=0.0;//和先设置为0    
    double prob[N_CITY_COUNT]; //保存各个城市被选中的概率

    for (int i=0;i<N_CITY_COUNT;i++)
    {
        if (m_nAllowed_City[i] == 1) //m_nAllowed_City[i]未访问过的城市,表示城市没去过
        {
            prob[i]=pow(g_Trial[m_nCur_City_No][i],ALPHA)*pow(1.0/g_Distance[m_nCur_City_No][i],BETA); //该城市和当前城市间的信息素，两两城市
            //pow(float x, float y) 计算x的y次幂
			dbTotal=dbTotal+prob[i]; //累加信息素，得到总和
        }
        else //如果城市去过了，则其被选中的概率值为0
        {
            prob[i]=0.0;
        }
    }
//======================================================
    //进行轮盘选择
    double dbTemp=0.0;//所选择的0~1之间的随机数作为轮盘赌的依据
    if (dbTotal > 0.0) //总的信息素值大于0
    {
        dbTemp=rnd(0.0,dbTotal); //取一个随机数

        for (int i=0;i<N_CITY_COUNT;i++)
        {
            if (m_nAllowed_City[i] == 1) //城市没去过
            {
                dbTemp=dbTemp-prob[i]; //这个操作相当于转动轮盘，如果对轮盘选择不熟悉，仔细考虑一下
                if (dbTemp < 0.0) //轮盘停止转动，记下城市编号，直接跳出循环
                 {
                    nSelected_City=i;
                    break;
                }
            }
        }
    }

    //======================================================
    //如果城市间的信息素非常小(小到比double能够表示的最小的数字还要小 )
    //那么由于浮点运算的误差原因，上面计算的概率总和可能为0
    //会出现经过上述操作，没有城市被选择出来
    //出现这种情况，就把第一个没去过的城市作为返回结果
    //题外话：刚开始看的时候，下面这段代码困惑了我很长时间，想不通为何要有这段代码，后来才搞清楚。
    if (nSelected_City == -1)
    {
        for (int i=0;i<N_CITY_COUNT;i++)
        {
            if (m_nAllowed_City[i] == 1) //城市没去过
            {
                nSelected_City=i;
                break;
            }
        }
    }
//======================================================
    //返回结果，就是城市的编号
    return nSelected_City;
}


//函数：蚂蚁在城市间移动
void CAnt::Move()
{
    int nCity_No=Choose_Next_City(); //选择下一个城市,nCity_No下一个城市的标号

    m_nPath[m_nMoved_City_Count]=nCity_No; //将下一个城市保存进蚂蚁走的路径
    m_nAllowed_City[nCity_No]=0;//把下一个城市设置成已经访问过的
    m_nCur_City_No=nCity_No; //改变当前所在城市为所选择的城市（下一个的）
	//m_nCur_City_No 当前所在城市编号
    m_nMoved_City_Count++; //已经去过的城市数量加1
}

//函数：蚂蚁进行搜索一次
void CAnt::Search()
{
    Init(); //调用函数；蚂蚁搜索前，先初始化,

    //如果蚂蚁去过的城市数量小于城市数量，就继续移动
    while (m_nMoved_City_Count < N_CITY_COUNT)
    {
        Move();//调用函数
    }

    //完成搜索后计算走过的路径长度
    Cal_Path_Length();//调用函数
}


//函数：计算蚂蚁走过的路径长度
void CAnt::Cal_Path_Length()
{

    m_dbPath_Length=0.0; //初始化：先把路径长度置0
    int m=0;
    int n=0;

    for (int i=1;i<N_CITY_COUNT;i++)
    {
        m=m_nPath[i];//加进当前城市之后，这一次的路径
        n=m_nPath[i-1];//加进当前城市之前，上一次的路径
        m_dbPath_Length=m_dbPath_Length+g_Distance[m][n];
    }

    //加上从最后城市返回出发城市的距离
    n=m_nPath[0];
    m_dbPath_Length=m_dbPath_Length+g_Distance[m][n];    

}

//tsp类
class CTsp
{
public:
    CTsp(void);
    ~CTsp(void);

public:
    CAnt m_cAntAry[N_ANT_COUNT]; //蚂蚁数组
    CAnt m_cBestAnt; //定义一个蚂蚁变量，用来保存搜索过程中的最优结果
                                        //该蚂蚁不参与搜索，只是用来保存最优结果

public:

    //初始化数据
    void InitData(); 

    //开始搜索
    void Search(); 

    //更新环境信息素
    void Update_Trial();


};


//构造函数
CTsp::CTsp(void){}
CTsp::~CTsp(void){}

//函数：初始化数据
void CTsp::InitData() 
{
    //先把最优蚂蚁的路径长度设置成一个很大的值
    m_cBestAnt.m_dbPath_Length=DB_MAX; 
    //计算两两城市间距离
    double dbTemp=0.0;//两两城市间距离
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            dbTemp=(x_Ary[i]-x_Ary[j])*(x_Ary[i]-x_Ary[j])+(y_Ary[i]-y_Ary[j])*(y_Ary[i]-y_Ary[j]);//平方
            dbTemp=pow(dbTemp,0.5);//开方
            g_Distance[i][j]=ROUND(dbTemp);//Round函数按照指定的小数位数进行四舍五入运算的结果,将计算的距离结果四舍五入保存
        }
    }

    //初始化环境信息素，先把城市间的信息素设置成一样
    //这里设置成1.0，设置成多少对结果影响不是太大，对算法收敛速度有些影响
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            g_Trial[i][j]=1.0;//i城市与j城市之间的信息素
        }
    }

}
//函数：更新环境信息素?????????????????????????????????????????????????????????????????????
void CTsp::Update_Trial()
{
    //临时数组，保存各只蚂蚁在两两城市间新留下的信息素

    double dbTemp_Ary[N_CITY_COUNT][N_CITY_COUNT];//每个蚂蚁在两两城市间新留下的信息素
    memset(dbTemp_Ary,0,sizeof(dbTemp_Ary)); //先全部设置为0
	//memset 函数是内存赋值函数,用来给某一块内存空间进行赋值的

    //计算新增加的信息素,保存到临时数组里

    int m=0;//m,n表示城市
    int n=0;
    for (int i=0;i<N_ANT_COUNT;i++) //计算每只蚂蚁留下的信息素,N_ANT_COUNT蚂蚁数
    {
            for (int j=1;j<N_CITY_COUNT;j++)
            {
                m=m_cAntAry[i].m_nPath[j];
                n=m_cAntAry[i].m_nPath[j-1];
                dbTemp_Ary[n][m]=dbTemp_Ary[n][m]+DBQ/m_cAntAry[i].m_dbPath_Length;
                dbTemp_Ary[m][n]=dbTemp_Ary[n][m];
            }

            //最后城市和开始城市之间的信息素
            n=m_cAntAry[i].m_nPath[0];
            dbTemp_Ary[n][m]=dbTemp_Ary[n][m]+DBQ/m_cAntAry[i].m_dbPath_Length;
            dbTemp_Ary[m][n]=dbTemp_Ary[n][m];

    }

    //======================================================
    //更新环境信息素
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            g_Trial[i][j]=g_Trial[i][j]*ROU+dbTemp_Ary[i][j]; //最新的环境信息素 = 留存的信息素 + 新留下的信息素
        }
    }

}


void CTsp::Search()
{

    char cBuf[256]; //打印信息用

    //在迭代次数内进行循环
    for (int i=0;i<N_IT_COUNT;i++) //N_IT_COUNT=1000; //迭代次数
    {
        //每只蚂蚁搜索一遍
        for (int j=0;j<N_ANT_COUNT;j++)
        {
            m_cAntAry[j].Search(); 
        }

        //保存最佳结果
        for (int j=0;j<N_ANT_COUNT;j++)
        {
            if (m_cAntAry[j].m_dbPath_Length < m_cBestAnt.m_dbPath_Length)
            {
                m_cBestAnt=m_cAntAry[j];
            }
        }

        //更新环境信息素
        Update_Trial();

        //输出目前为止找到的最优路径的长度
        //sprintf(cBuf,"\n[%d] %.0f",i+1,m_cBestAnt.m_dbPath_Length);
        //printf(cBuf);
    }

}

int aco()
{
	int i;
	printf("请输入城市横坐标：\n");
	for (i = 0; i <= N_CITY_COUNT; i++)
	{
		scanf("%lf", &x_Ary[i]);

	}
	printf("请输入城市纵坐标：\n");
	for (i = 0; i <= N_CITY_COUNT; i++)
	{
		scanf("%lf", &y_Ary[i]);
	}
    //用当前时间点初始化随机种子，防止每次运行的结果都相同
    //time_t tm;
    //time(&tm);
    //unsigned int nSeed=(unsigned int)tm;
    //srand(nSeed);

    //开始搜索
    CTsp tsp;

    tsp.InitData(); //初始化
    tsp.Search(); //开始搜索

    //输出结果
    printf("\n最短距离的配送顺序为:\n");


    char cBuf[128];
    for (i=0;i<N_CITY_COUNT;i++)
    {
        sprintf(cBuf,"%02d ",tsp.m_cBestAnt.m_nPath[i]+1);//sprintf指的是字符串格式化命令，主要功能是把格式化的数据写入某个字符串中
        if (i % 20 == 0)
        {
            printf("\n");
        }
        printf(cBuf);
    }

    return 0;
}
