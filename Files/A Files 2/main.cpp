#include <iostream>
#include <bits/stdc++.h>
using namespace std;
short headerlist;
int ctr1=0;
int arr_ind=0;
short rrns[10];
bool t=1;

short listheader(fstream & writeF)
{
    writeF.seekg(0);
    writeF.read((char*)&headerlist,sizeof(headerlist));
    return headerlist;
}
bool writefirstheader(fstream &f,short s=-1)
{
        bool t2;
        if(t==1)
        {
        f.seekp(0,ios::beg);
        f.write((char*)&s,sizeof(s));
        t=0;
        t2=0;
        }
        t2=1;
        return t2;
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
    short s=-1;
    int n=50;

        f.seekp(0);
        f.write((char*)&rrns[ctr1],sizeof(rrn));

    if(ctr1==0)
    {
        f.seekp(0);
        f.seekp(((rrn*n)+2),ios::cur);
        cout<<"seek to delet this >> "<<f.tellp()<<endl;
        f.put('*');
        f.write((char*)&s,sizeof(s));
        cout<<"Done"<<rrns[ctr1]<<endl;
    }
    else {
        f.seekp(0);
        f.ignore();
        f.ignore();
        f.seekp((rrn*n),ios::cur);
        f<<'*';
        f.write((char*)&rrns[ctr1-1],sizeof(rrn));
        cout<<"Done"<<rrns[ctr1-1]<<endl;
        }
        ctr1++;
    f.flush();
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
        short next;
        f.close();
        f.open("file1.txt",ios::in|ios::out);
        short go=listheader(f);
        cout<<"GOGO "<<go<<endl;
        books b;
        if(go==-1){f.close();f.open("file1.txt",ios::in|ios::out|ios::app);addrecord(f,b);}
        else{
        f.seekg(((go*50)+2),ios::beg);
        f.ignore();
        f.read((char*)&next,sizeof(next));
        f.seekp(0);
        f.write((char*)&next,sizeof(next));
        f.seekp(((go*50)+2),ios::beg);
        addrecord(f,b);
        }
        f.close();
}
void delet(fstream & f,string isbn)
{
    f.close();
    f.open("file1.txt",ios::in|ios::out);

    books b;int counter1=1,rrn=0,n=50,cer,tmp;
    f.seekg(0);
    f.ignore();
    f.ignore();
    while(!f.eof()){
    f.getline(b.ISBN,5,'|');
    f.flush();
    f.clear();
    cout<<"ISBN????"<<b.ISBN<<endl;
    if(f.fail())return;
    if(b.ISBN==isbn)
    {

        cer=f.tellg();
        tmp=cer;
        while(tmp>=n)
        {
            tmp-=50;
            rrn+=1;
        }

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
    f.close();
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
    f.flush();
    f.clear();
}
void updatebyisbn(fstream &f,string isbn)
{
    f.close();
    f.open("file1.txt",ios::in|ios::out);

    books b;int counter1=1,rrn=0,n=50,cer,tmp;
    f.seekg(0);
    f.ignore();
    f.ignore();
    while(!f.eof()){

    f.getline(b.ISBN,5,'|');
    f.flush();
    f.clear();

    if(f.fail())return;

    if(b.ISBN==isbn)
    {
        cout<<"ISBN????"<<b.ISBN<<endl;
        cer=f.tellg();
        tmp=cer;
        while(tmp>=n)
        {
            tmp-=50;
            rrn+=1;
        }

        f.seekp(0);
        f.ignore();
        f.ignore();
        f.seekp((rrn*n),ios::cur);
        addrecord(f,b);
        f.close();
        break;
    }
    f.seekg(0);
    f.ignore();
    f.ignore();
    f.seekg(((counter1*n)),ios::cur);
    counter1++;
    if(f.fail())return;
    }
    f.close();
}
void compactFile(fstream &f)
{
    vector<books> data;
    int ctr1;
    char check;
    books b;int counter=0,n=50;
    f.seekg(0);
    f.ignore();
    f.ignore();
    while(!f.eof()){
    f.read(&check,1);
    cout<<">>>> "<<check<<endl;
    cout<<">>>> "<<f.tellg()<<endl;
    f.seekg(-1,ios::cur);
    cout<<">>>> "<<f.tellg()<<endl;
    if(check=='*'){
        counter++;
        f.seekg(0);
        f.seekg(((counter*n)+2),ios::beg);
        check='&';
        continue;
    }
    f.getline(b.ISBN,5,'|');
    if(f.fail()||f.eof())break;
    f.getline(b.title,20,'|');
    f.getline(b.authorName,20,'|');
    f.getline((char*)&b.price,5,'|');
    f.getline((char*)&b.year,5,'|');
    f.getline((char*)&b.numPage,5,'|');
    f.flush();
    data.push_back(b);
    cout<<"ISBN            :"<<b.ISBN<<endl;
    cout<<"Title           :"<<b.title<<endl;
    cout<<"Author Name     :"<<b.authorName<<endl;
    cout<<"Price           :"<<b.price<<endl;
    cout<<"Year            :"<<b.year<<endl;
    cout<<"Number Of Pages :"<<b.numPage<<endl;
    f.seekg(0);
    f.ignore();
    f.ignore();
    counter++;
    f.seekg((counter*n),ios::cur);
    if(f.fail())break;
    f.flush();
    }


        f.close();
        f.open("file1.txt",ios::in|ios::out|ios::trunc);
        short s=-1;
        ctr1=0;
        f.seekp(0,ios::beg);
        f.write((char*)&s,sizeof(s));
        for(int i=0;i<data.size();i++)
        {
        cout<<"isa"<<endl;

        f <<data[ctr1].ISBN;
        f<<'|';

        f <<data[ctr1].title;
        f<<'|';

        f <<data[ctr1].authorName;
        f<<'|';

        f.write((char*)&data[ctr1].price,sizeof(data[ctr1].price));
        f<<'|';

        f.write((char*)&data[ctr1].year,sizeof(data[ctr1].year));
        f<<'|';

        f.write((char*)&data[ctr1].numPage,sizeof(data[ctr1].numPage));
        f<<'|';
        int strln=strlen(data[ctr1].title)+strlen(data[ctr1].authorName)+strlen(data[ctr1].ISBN)+18;
        spaces(f,strln);
        ctr1++;

        }

    f.close();
    f.flush();
}
void printbyISBN(fstream &f,string isbn)
{
    f.close();
    f.open("file1.txt",ios::in|ios::out);

    books b;int counter1=1,rrn=0,n=50,cer,tmp;
    f.seekg(0);
    f.ignore();
    f.ignore();
    while(!f.eof()){

    f.getline(b.ISBN,5,'|');
    f.flush();
    f.clear();

    if(f.fail())return;

    if(b.ISBN==isbn)
    {
        cer=f.tellg();
        tmp=cer;
        while(tmp>=n)
        {
            tmp-=50;
            rrn+=1;
        }

        f.seekp(0);
        f.ignore();
        f.ignore();
        f.seekp((rrn*n),ios::cur);
        f.getline(b.ISBN,5,'|');
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
bool Z;
cout<<"Do U wanna Update 0 or 1 ? ";cin>>Z;cin.ignore();
if(Z==1)
    {
        string m;
        cout<<"what record to update ? ";cin>>m;cin.ignore();
        updatebyisbn(f,m);
    }

bool B;
cout<<"Do U wanna Compact 0 or 1 ? ";cin>>B;cin.ignore();
if(B==1){compactFile(f);}


bool K;
cout<<"Do U wanna Print record 0 or 1 ? ";cin>>K;cin.ignore();
if(K==1)
    {
        string L;
        cout<<"what record to print ? ";cin>>L;cin.ignore();
        printbyISBN(f,L);
    }
    f.close();

    return 0;
}
