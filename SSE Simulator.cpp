#include<iostream>
#include <fstream>
#include<ctime>
#include<cmath>
#include<windows.h>
#include<string.h>

using namespace std;
typedef int Status;  
# define MAXSIZE 256
class lib{
public:
	string BuyOrSell;
	int BuyOrSellStock;
	double BuyOrSellAt;
	double BuyOrSellLots;
	//初始化 
	lib(string BOrS,int code,double At,double Lots){
		BuyOrSell=BOrS;
		BuyOrSellStock=code;
		BuyOrSellAt=At;
		BuyOrSellLots=Lots;
	}
	lib(){
		;
	}
}; 
typedef lib ElemType;

//队的类定义 
class que{					
	ElemType data[MAXSIZE];
	int front;
	int rear;
public: 
	que(){				
	front=0;
	rear=0;
	}

	bool isEmpty(){			//判空 
	if(front==rear)
		return 1;
	else
		return 0;
	}

	bool isFull(){				//判满 
	if((rear+1)%MAXSIZE==front)
		return 1;
	else
		return 0;
}

	Status in(ElemType e){	//入队 
	if(isFull())
		return 0;
	else{
		data[rear]=e;
		if(rear==MAXSIZE)
			rear=0;
		else
			rear++;
	}
	return 1;
	}

	ElemType out(){			//出队 
	if(isEmpty())
		;
	else{
		ElemType e=data[front];
		if(front==MAXSIZE)
			front=0;
		else
			front++;
		return e;
	}
	}
};
//哈希表节点 
class HashItem{
    int key;
	double val;
public:
    HashItem(int k, double v): key(k), val(v){}//构造方法 
    const int& getKey(){
        return key;
    }
    const double& getVal(){
        return val;
    }
};
//哈希表 
class HashTable{
    static const int SIZE = 256;
    HashItem ** table;      // 指针的指针，指向HashItem*
public:
	
    HashTable(){
        table  = new HashItem*[SIZE]();    
    }
    
    void set(int key, double val){
   		int idx = key%SIZE;
    	while(table[idx] && table[idx]->getKey() != key)
     	   idx = (idx+1)%SIZE;          
  	  	if(table[idx]) 
			delete table[idx];
  		table[idx] = new HashItem(key, val);
	}
    const double get(int key){
    	int idx = key%SIZE;
    	while(table[idx] && table[idx]->getKey() != key)
        	idx = (idx+1)%SIZE;          
    	return table[idx] ? table[idx]->getVal() : -1;      // key不存在返回-1 
	}
	void show(){
		cout<<"\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
		cout<<"@@@@@@@@@@@Today`s SSE@@@@@@@@@@@\n";
		cout<<"Code:114514\tPrice:"<<get(114514)<<endl;
		cout<<"Code:310430\tPrice:"<<get(310430)<<endl;
		cout<<"Code:434107\tPrice:"<<get(434107)<<endl;
		cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
		cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";
	}
    ~HashTable(){
        for(int i=0; i<SIZE; i++)
            if(table[i]) delete table[i];
        delete[] table;                     
    }
};

//询问当前处理器频率和计数器，保证随机数种子在1s内也有变化 
int randEx(){
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);
	return rand();
}
//随机游走 
double randomwalk(double mae){
	double ato=mae;
	double delta;
	srand(unsigned(time(NULL)));
    if (randEx()%10 > 4)
    	delta = (randEx() % int(mae*100))*0.001 ;
    else 
    	delta = (randEx() % int(mae*100))*-0.001 ;
    delta = (double)round(delta*100)/100;
	ato=ato+delta;
//	cout<<ato;
	return ato;
}

//账户类 
class account{
public:	//偷懒，将所有元素设为public，好孩子不要学 
	int balance;//余额。
	int cap;//持仓市值 
	HashTable position;//哈希表存储该账户持仓情况 
	
	account(){//构造方法 
		balance=cap=1000000;
		position.set(114514,0);
		position.set(310430,0);
		position.set(434107,0);
	}
	void show(){
		cout<<"\n===================================\n";
		cout<<"=============Positions=============\n";
		cout<<"Code:114514\t"<<"Pos.:"<<position.get(114514)<<endl;
		cout<<"Code:310430\t"<<"Pos.:"<<position.get(310430)<<endl;
		cout<<"Code:434107\t"<<"Pos.:"<<position.get(434107)<<endl<<endl;
		cout<<"Balance:"<<balance<<endl;
		cout<<"Client Equity:"<<cap<<endl;
		cout<<"===================================\n";
		cout<<"===================================\n\n";
	}
};

int main(){
	HashTable SSE;
	//初始化三支股票
	SSE.set(114514,100);
	SSE.set(310430,10);
	SSE.set(434107,20);
	//初始化记录
	que record;
	ofstream fout;
	fout.open("record.txt");
    fout<<"          本次模拟记录\nB/S\tStock Code\tPrice\tLots\n";
    fout.close();
	//初始化一个股票账户 
	account Harune;
	//用户界面 
	cout<<"############Welcome to SSE simulator############";
	int cmd;	
	for(int i=1;i<=10;i++){
		cout<<"\n\n\n\n\n\n\n\n             DAY "<<i<<"\n";
		SSE.show();
		Harune.show();
		while(1){
			cout<<"What`re you going to do?\n1.Buy\n2.Sell\n3.Query\n4.End today\nYour choice:";
			cin>>cmd;
			if(cmd == 4)
				break;
			if(cmd == 1){
				cout<<"Which stock would you like to BUY?\nCode:";
				int code;
				cin>>code;
				//如果股票代码不存在 
				if(SSE.get(code) == -1){
					cout<<"\nCode:"<<code<<" NOT FOUND!\n\n";
					continue;
				} 
				//如果余额为0 
				if(Harune.balance==0)
					cout<<"You can`t buy any!Sell some of your positions maybe?\n";
				else{
					cout<<"You can buy at most "<<Harune.balance/SSE.get(code)<<" shares.\n";
					cout<<"Lots:";
					int bullish;
					cin>>bullish; 
					//如果买入数量超过最大可购数量，或买入数量<0 
					if(bullish < 0 or bullish > Harune.balance / SSE.get(code)){
						cout<<"Invalid entrust!\n";
					}else{
						cout<<"Entrust accepted\n";
						Harune.balance -= bullish * SSE.get(code);
						Harune.position.set(code , Harune.position.get(code) + bullish);
						Harune.show();
						//创建记录并入队 
						lib temp("Buy",code,SSE.get(code),bullish);
						record.in(temp);
					}
				}
			}else if(cmd == 2){
				cout<<"Which stock would you like to SELL?\nCode:";
				int code;
				cin>>code;
				cout<<"Code:"<<code<<"\tYou are holding "<<Harune.position.get(code)<<" shares.\n";
				cout<<"Lots:";
				int bearish;
				cin>>bearish;
				if(bearish<0){
					cout<<"Invalid entrust!\n";
				}else{
					cout<<"Entrust accepted\n";
					Harune.balance += bearish * SSE.get(code);
					Harune.position.set(code , Harune.position.get(code) - bearish);
					Harune.show();
					//创建记录并入队 
					lib temp("Sell",code,SSE.get(code),bearish);
					record.in(temp);
				}
			}else if(cmd ==  3){
				SSE.show();
			}
		}
		//一天结束，更新明日股票价格，并更新账户市值 
		SSE.set(114514,randomwalk(SSE.get(114514)));
		SSE.set(310430,randomwalk(SSE.get(310430)));
		SSE.set(434107,randomwalk(SSE.get(434107)));	
		Harune.cap=Harune.balance
					+SSE.get(114514)*Harune.position.get(114514)
					+SSE.get(310430)*Harune.position.get(310430)
					+SSE.get(434107)*Harune.position.get(434107); 
	}
	cout<<"\n******Settlement Statement*******\n";
	SSE.show();
	Harune.show();
	cout<<"*********************************\n";
	cout<<"********You earned:"<<double(Harune.cap)/10000-100<<"%********";
	cout<<"\n*********************************\n";
	ofstream fout1;
	fout1.open("record.txt",ios_base::app);
	//出队直至队列为空 
	while(not record.isEmpty()){
		lib temp;
		temp=record.out();
		fout1<<temp.BuyOrSell<<"\t"<<temp.BuyOrSellStock<<"\t\t"<<temp.BuyOrSellAt<<"\t"<<temp.BuyOrSellLots<<endl;
	}
	fout1.close();
	system("pause");
	return 0;
}
