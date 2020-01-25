#include <iostream>
#include <bits/stdc++.h>
using namespace std;
short headerlist;
int ctr1=0;
int arr_ind=0;
short rrns[10];
bool t=1;

short readdelheader(fstream &f,short go)
{
    f.seekg(((go*50)+2),ios::beg);
    f.ignore();
    f.read((char*)&go,sizeof(go));
    return go;

}
void spaces(fstream & writeF,int n)
{
    for(int i=n;i<50;i++)
    {
      writeF<<'-';
    }
}
void write_listheader(fstream &f,short rrn)
{
    rrns[arr_ind]=rrn;
    arr_ind++;
    ctr1++;
    short s=-1;
if(ctr1==2){
        int n=50;
        f.seekp(0);
        f.ignore();
        f.ignore();
        f.seekp((rrn*n),ios::cur);
        f<<'*';
        f.write((char*)&rrns[arr_ind-1],sizeof(rrn));
        cout<<"Done"<<rrns[arr_ind-1]<<endl;
        f.seekp(0);
        f.write((char*)&rrns[arr_ind-1],sizeof(rrn));
    }
    f.clear();
}
struct books
{
    char ISBN[5];
    char title[20];
    char authorName[20];
    int price;
    int year,numPage;
};
short listheader(fstream & writeF)
{
    writeF.seekg(0);
    writeF.read((char*)&headerlist,sizeof(headerlist));
    return headerlist;
}
bool writefirstheader(fstream &f,short s=-1)
{
        if(t==1)
        {
        f.seekp(0,ios::beg);
        f.write((char*)&s,sizeof(s));
        t==0;
        }
        return 1;
}
void addrecord(fstream & writeF,books & book)
{
    cout<<"Enter ISBN :"<<endl;
    cin.getline(book.ISBN,5);

    cout<<"Enter Title :"<<endl;
    cin.getline(book.title,20);

    cout<<"Enter Author Name :"<<endl;
    cin.getline(book.authorName,20);

    cout<<"Enter Price :"<<endl;
    cin>>book.price;

    cout<<"Enter Year :"<<endl;
    cin>>book.year;

    cout<<"Enter Number of Pages :"<<endl;
    cin>>book.numPage;
    cin.ignore();

    writeF <<book.ISBN;
    writeF<<'|';

    writeF <<book.title;
    writeF<<'|';

    writeF <<book.authorName;
    writeF<<'|';

    writeF.write((char*)&book.price,sizeof(book.price));
    writeF<<'|';

    writeF.write((char*)&book.year,sizeof(book.year));
    writeF<<'|';

    writeF.write((char*)&book.numPage,sizeof(book.numPage));
    writeF<<'|';
    int strln=strlen(book.title)+strlen(book.authorName)+strlen(book.ISBN)+18;
    spaces(writeF,strln);



}
void inseert(fstream &f)
{
        f.close();
        f.open("file1.txt",ios::in|ios::out);
        short go=listheader(f);
        cout<<"GOGO "<<go<<endl;
        books b;
        if(go==-1){f.close();f.open("file1.txt",ios::in|ios::out|ios::app);addrecord(f,b);}
        else{
        f.seekp(((go*50)+2),ios::beg);
        addrecord(f,b);
        f.seekg(((go*50)+2),ios::beg);
        f.ignore();
        f.read((char*)&go,sizeof(go));
        f.seekp(0);
        f.write((char*)&go,sizeof(go));
        }
}
void delet(fstream & f,string isbn)
{
    f.close();
    f.open("file1.txt",ios::in|ios::out);

    books b;int counter1=1,rrn=0,n=50,cer,tmp;
    short tmp2=listheader(f);
    char dl;
    f.seekg(0);
    f.ignore();
    f.ignore();
    while(!f.eof()){
    f.getline(b.ISBN,5,'|');
    f.clear();
    cout<<"ISBN????"<<b.ISBN<<endl;
    if(f.fail())return;
    if(b.ISBN==isbn)
    {
        ctr1++;
        cer=f.tellg();
        tmp=cer;
        while(tmp>=n)
        {
            tmp-=50;
            rrn+=1;
        }

    }

    if(b.ISBN==isbn)
    {
        f.seekp(0);
        f.ignore();
        f.ignore();
        f.seekp((rrn*n),ios::cur);
        f<<'*';
        write_listheader(f,rrn);
        break;
    }
    f.seekg(0);
    f.ignore();
    f.ignore();
    f.seekg(((counter1*n)),ios::cur);
    counter1++;
    if(f.fail())return;
    }
}
void readbook(fstream & f)
{
    books b;int counter=1,n=50;
    f.seekg(0);
    f.ignore();
    f.ignore();
    while(!f.eof()){
    f.getline(b.ISBN,5,'|');
    if(f.fail())return;
    f.getline(b.title,20,'|');
    f.getline(b.authorName,20,'|');
    f.getline((char*)&b.price,5,'|');
    f.getline((char*)&b.year,5,'|');
    f.getline((char*)&b.numPage,5,'|');
    cout<<"ISBN            :"<<b.ISBN<<endl;
    cout<<"Title           :"<<b.title<<endl;
    cout<<"Author Name     :"<<b.authorName<<endl;
    cout<<"Price           :"<<b.price<<endl;
    cout<<"Year            :"<<b.year<<endl;
    cout<<"Number Of Pages :"<<b.numPage<<endl;
    f.seekg(0);
    f.ignore();
    f.ignore();
    f.seekg((counter*n),ios::cur);
    counter++;
    if(f.fail())return;
    }
    f.clear();
}


int main()
{
    fstream f;
    f.open("file1.txt",ios::in|ios::out|ios::app);
    writefirstheader(f);
    books b;int n;
    cout<<"How Many Record Do You Wanna Add ? "<<endl;
    cin>>n;
    cin.ignore();
    for(int i=0;i<n;i++)
        {
           addrecord(f,b);
        }
    readbook(f);
int y;
string d;
cout<<"how many deletion ?"<<endl;
cin>>y;
for(int i=0;i<y;i++)
{
    cin>>d;
    delet(f,d);
}
int x;
cout<<"how many insertion :";
cin>>x;
cin.ignore();
for(int i=0; i<x;i++)
{
    inseert(f);
}


for(int i=0;i<3;i++)
{
    cout<<"Arr of deleted >> "<<rrns[i];
}
    f.close();

    return 0;
}
