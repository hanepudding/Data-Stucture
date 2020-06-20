#include <iostream>
#include <fstream>
using namespace std;
int main()
{
   	string a;        //要加密的密码
   	string b;     //密钥
   	int i;
   	
   	ifstream fin("key.txt");
    int ii=0;
    char ch;
    while(fin.good()){
        fin >> ch;
        b+=ch ;
        ii++; 
    }
	fin.close();
	
   	ifstream fin2("input.txt");
    ii=0;
    while(fin2.good()){
        fin2 >> ch;
        a+=ch ;
        ii++; 
    }
	fin2.close();
   
  
 	for(i=0;a[i];i++)
     	a[i]=a[i]^b[i];
   	cout<<"完成,请查看output.txt";
   	ofstream fout;
    fout.open("output.txt");
   	fout<<a;
   	fout.close();

return 0;
}

