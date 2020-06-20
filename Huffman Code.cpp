#include<iostream>
#include<string.h>
#include <fstream>
#define EOS 0
#define EOL 0
using namespace std;             
typedef struct                 
{
    char data;
    int parent;
    int lc;
    int rc;
    int w;
}node;				
typedef node Huff[16];
typedef struct
{
    int hajime;
    char bit[16]; 
}HCode;
typedef HCode hcode[8];
void wof(char *a, char *b, int *w, int &n) {  //统计字频 
    int i = 0, j, k = 0;
    while (a[i] != EOS){
        for (j = 0; j<k; j++){
            if (a[i] == b[j]){
                w[j]++;
                break;           
            }
        }
        if (j == k){
            b[k] = a[i];
            w[k]++;
            k++;
        }
        i++;
    }
    n = k;
    b[k] = EOL;
}

void createT(Huff h, int l, int *w, char *tmikw){
    int i, j;
    for (i = 0; i<l; i++){
        (h + i)->data = tmikw[i];
        (h + i)->parent = -1;
        (h + i)->lc = -1;
        (h + i)->rc = -1;
        (h + i)->w = *(w + i);              
    }
    for (i = l; i<l * 2 - 1; i++){
        (h + i)->parent = -1;
        (h + i)->lc = -1;
        (h + i)->rc = -1;
        (h + i)->w = 0;
    }
    cout << endl;
    for (i = 0; i<l - 1; i++){
        int ta, tb;
        int ta_mae, tb_mae;
        ta = tb = 65536;                
        ta_mae = tb_mae = 0;

        for (j = 0; j<l + i; j++){
            if ((h + j)->w<ta && (h + j)->parent == -1){
                tb = ta;
                ta = (h + j)->w;
                tb_mae = ta_mae;
                ta_mae = j;
            }
            else if ((h + j)->w<tb && (h + j)->parent == -1){
                tb = (h + j)->w;
                tb_mae = j;
            }
        }
        (h + l + i)->parent = -1;        
        (h + l + i)->lc = ta_mae;    
        (h + l + i)->rc = tb_mae;     
        (h + ta_mae)->parent = l + i;
        (h + tb_mae)->parent = l + i;
        (h + l + i)->w = ta + tb;
    }
    cout << endl;
}


void encode(Huff h, hcode code, int l){
    int i, p, c;
    HCode *hf;
    hf = new HCode;

    hf->bit[15] = EOL;//到此结束 
    for (i = 0; i<l; i++){
        c = i;
        p = h[i].parent;
        hf->hajime = 16;
        while (p != -1){
            if (h[p].lc == c){
                hf->bit[--hf->hajime] = '0';
            }
            else{
                hf->bit[--hf->hajime] = '1';
            }
            c = p;
            p = h[c].parent;
        }
        strcpy(code[i].bit, &hf->bit[hf->hajime]);
        cout << endl<< i + 1 << "号叶" << (h + i)->data << "，其编码是";
        cout << (code + i)->bit;
    }
    cout << endl;
    delete hf;
}
void tocode(hcode code, char *ECD, char *tmikw, char *DCD){
    int ta, tb = 0;
    int i = 0, j;
    while (ECD[i] != EOL) {
        j = 0;
        while (tmikw[j] != EOL){
            if (ECD[i] == tmikw[j]){
                ta = 0;
                while (code[j].bit[ta] != EOL)
                    DCD[tb++] = code[j].bit[ta++];
                break;                
            }
            j++;
        }
        i++;
    }
    DCD[tb++] = EOL;
}
void towrd(Huff h, int l, char *DCD){
    int T = 2 * l - 1;        
    int m = T - 1;
    ofstream fout;
   
    fout.open("decode.txt",ios_base::app);
    for (int i = 0; DCD[i] != EOL; i++){
        if (h[m].lc == -1){
            fout << h[m].data;
            m = T - 1;
        }
        if (DCD[i] == '0')
            m = h[m].lc;
        else
            m = h[m].rc;
    }

    fout <<h[m].data << endl;
 	fout.close();
}
int main(){
    Huff HNPDN;
    hcode HNPDC;
    char ECD[1000], tmikw[1000], DCD[1000], BM[1000];
    int w[1000], n, i; 
	         
    ifstream fin("origin.txt");
    char ch;
    int ii=0;
    while(fin.good()){
        fin >> ch;
        ECD[ii] = ch ;
        ii++; 
    }
	fin.close();

    for (i = 0; i<100; i++)    
		w[i] = 0;
    wof(ECD, tmikw, w, n);
    createT(HNPDN, n, w, tmikw);    
    encode(HNPDN, HNPDC, n);            
    tocode(HNPDC, ECD, tmikw, DCD);
   // cout << "请输入密文：";
   ifstream fin2("01code.txt");
    ii=0;
    while(fin2.good()){
        fin2 >> ch;
        BM[ii] = ch ;
        ii++; 
    }
	fin2.close();
  //  cin >> BM;
    ofstream fout;
   
    fout.open("decode.txt");
    fout<<"解码结果：";
    fout.close();
    towrd(HNPDN, n, BM);
    cout<<"解码结束，请查看decode.txt" ; 
    return 0;
}
