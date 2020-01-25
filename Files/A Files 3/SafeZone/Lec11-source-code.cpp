#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdlib.h>     /* atoi */

using namespace std;


class SimpleIndex
{
	int next;//next free slot in index
	int s_next;//next free slot in s_index
	int I_next;//next free slot in s_index
	int headerSize;
	struct Course
	{
		char ID[6];		char Name[10];	char I_Name[10]; char Weeks[4];
	};
	struct IndexRecord
	{
		char PK[6];		int offset;
	};
	struct Inverted_IndexRecord
	{
		int p1 ; char PK[6];		int p2;
	};
	const static int indexSize = 100;
	IndexRecord index[indexSize];
	IndexRecord s_index[indexSize];
    Inverted_IndexRecord I_index[indexSize];

	char dataFilePath[20] = "D:\\dataFile.txt";
	char indexFilePath[20] = "D:\\P_indexFile.txt";
	char S_indexFilePath[20] = "D:\\S_indexFile.txt";
	char Inverted_Path[20] = "D:\\Inv_File.txt";
public:
	SimpleIndex();
	int IndexBinarySearch(char key[]);
	int s_IndexBinarySearch(char key[]);
	void printStudent(char PK[]);
	void print_I_Name(char name[]);
	void insertStudent();
	void deleteStudent(char PK[]);
	void delete_I_name(char PK[]);
	void saveIndex();
	void loadIndex();
	void ReconstructIndex();
	void sortIndex();
	void sorts_Index();
	void ChangeIndexState(bool state);
	bool getIndexState();
	bool exists(char name[20]);
	void printIndex();
	void Updata_PK();
	void Updata_I_Name();
};

SimpleIndex::SimpleIndex()
{
	next = 0;
	s_next = 0;
	I_next = 0;
	headerSize = 1;
}

bool SimpleIndex::exists(char name[20] )
{
	ifstream f(name);
	if (f.fail())
	{		return false;	}
	else
	{		f.close(); 		return true;	}
}

int SimpleIndex::IndexBinarySearch(char key[])
{

	int size = next;
	int low = 0, high = size - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (strcmp(index[middle].PK, key) == 0)
			return middle;
		else if (atoi(index[middle].PK)<atoi(key))
			low = middle + 1;
		else
			high = middle - 1;
	}
	return -1;
}

int SimpleIndex::s_IndexBinarySearch(char key[])
{

	int size = s_next;
	int low = 0, high = size - 1;
	while (low <= high)
	{

		int middle = (low + high) / 2;
		if (strcmp(s_index[middle].PK, key) == 0)
			{
                return middle;
			}
		else if (atoi(s_index[middle].PK)<atoi(key))
			{
                low = middle + 1;
			}
		else
			high = middle - 1;

	}

	return -1;
}

void SimpleIndex::printStudent(char PK[6])
{

	ifstream fin;
	fin.open(dataFilePath, ios::binary | ios::in);
	int address = IndexBinarySearch(PK);

	if (address == -1)
	{
		cout << "\nthis Course not exists\n";
		return;
	}
	short len;
	Course s;
	fin.seekg(index[address].offset, ios::beg);
	fin.read((char*)&len, sizeof(len));

	char *buffer = new char[len];

	fin.read(buffer, len);
	istringstream stream(buffer);
	stream.getline(s.ID, 5, '|');
	stream.getline(s.Name, 9, '|');
	stream.getline(s.I_Name, 9, '|');
	stream.getline(s.Weeks, 3, '|');
	cout << endl << "ID: " << s.ID << "  Name: " << s.Name << "  I_Name: " << s.I_Name<< "  Weeks: " << s.Weeks;
	fin.close(); delete [] buffer;

}

void SimpleIndex::print_I_Name(char name[])
{
    int pos= s_IndexBinarySearch(name);

    for(int i =pos;i<I_next;i++)
    {
        if(I_index[i].p2 != -1)
            {
                printStudent(I_index[i].PK);
                i=I_index[i].p2;
                i--;
            }
            else
            {
                printStudent(I_index[i].PK);
                break;
            }
    }
}

void SimpleIndex::printIndex()
{
    cout <<"P_Index"<<endl;
	for(int i=0;i<next;i++)
	{
		cout<<index[i].PK<<"  "<<index[i].offset<<endl;
	}
	cout <<"S_Index"<<endl;
	for(int i=0;i<s_next;i++)
	{
		cout<<s_index[i].PK<<"  "<<s_index[i].offset<<endl;
	}
	cout <<"I_Index"<<endl;
	for(int i=0;i<I_next;i++)
	{
		cout<<I_index[i].p1<<"  "<<I_index[i].PK<<"  "<<I_index[i].p2<<endl;
	}
}







void SimpleIndex::insertStudent()
{
	ChangeIndexState(true);
	ofstream fout;
	fout.open(dataFilePath, ios::app | ios::out | ios::binary);

	Course s;
	cout << "\nEnter ID: ";			cin.getline(s.ID, 5);
	cout << "\nEnter Name: ";		cin.getline(s.Name, 9);
	cout << "\nEnter I_Name: ";	cin.getline(s.I_Name, 9);
	cout << "\nEnter Weeks: ";	cin.getline(s.Weeks, 3);

	char buffer[200];
	strcpy(buffer, s.ID);			strcat(buffer, "|");
	strcat(buffer, s.Name);		strcat(buffer, "|");
	strcat(buffer, s.I_Name);	strcat(buffer, "|");
	strcat(buffer, s.Weeks);	strcat(buffer, "|");

	short len = strlen(buffer);

	fout.seekp(0, ios::end);
	int addess = fout.tellp();
	fout.write((char*)&len, sizeof(len));
	fout.write(buffer, len);
    IndexRecord temp;
	strcpy(temp.PK, s.ID);
	temp.offset = addess;
	index[next] = temp;
	next++;
    //hnsearch we lo msh mogod abl keda
    int x = s_IndexBinarySearch(s.I_Name);

    cout <<"x:" <<x;

    if(x == -1)  ///not in secondary index so add it in secondary & inverted index
    {
        ///add to inverted index
        I_index[I_next].p1 = I_next;
        strcpy(I_index[I_next].PK, s.ID);
        I_index[I_next].p2 = -1;


        strcpy(s_index[s_next].PK, s.I_Name);

        s_index[s_next].offset = I_next;


        s_next++;
        I_next++;

    }
    else
    {
        int tmp =s_index[x].offset;
        if(s_index[x].offset != -1)
        {
            for(int i = 0;i<I_next;i++)
            {
                if(tmp == I_index[i].p1 && I_index[i].p2 == -1 ) ///the last occerance in the I_list
                {
                    I_index[i].p2 = I_next; /// -1 ==> another offset

                    I_index[I_next].p1 = I_next;
                    strcpy(I_index[I_next].PK, s.ID);
                    I_index[I_next].p2 = -1;
                    I_next++;
                }
                else if(tmp == I_index[i].p1 && I_index[i].p2 != -1)
                {
                   // i=I_index[i].p2;
                    tmp=I_index[i].p2;
                }
            }
        }
    }
	sortIndex();
	sorts_Index();


	fout.close();
}

void SimpleIndex::deleteStudent(char PK[6])
{
	ChangeIndexState(true);

	fstream fout(dataFilePath, ios::binary | ios::out | ios::in);
	int address = IndexBinarySearch(PK);
	if (address == -1)
	{
		cout << "\nCourse not found";
		return;
	}

	fout.seekp(index[address].offset + 2, ios::beg);
	fout << '*';

	//shift up to delete from index
	for (int i = address; i<next - 1; i++)
		index[i] = index[i + 1];

	next--;

	fout.close();

}

void SimpleIndex::delete_I_name(char name[])
{
    int pos= s_IndexBinarySearch(name);
    s_index[pos].offset =-1;
    for(int i =pos;i<I_next;i++)
    {


        if(I_index[i].p2 != -1)
            {
                deleteStudent(I_index[i].PK);
                I_index[i].PK[0]='#';
                i=I_index[i].p2;
                i--;
            }else
            {
                deleteStudent(I_index[i].PK);
                break;
            }
    }
}

void SimpleIndex::saveIndex()
{
	ofstream fout(indexFilePath, ios::trunc);
	for (int i = 0; i<next; i++)
	{
		IndexRecord temp = index[i];
		fout.write((char*)&temp, sizeof(temp)); //fixed len record, fixed len field
	}

	ofstream f2out(S_indexFilePath, ios::trunc);
	for (int i = 0; i<s_next; i++)
	{
		IndexRecord temp = s_index[i];
		f2out.write((char*)&temp, sizeof(temp)); //fixed len record, fixed len field
	}

	ofstream f3out(Inverted_Path, ios::trunc);
	for (int i = 0; i<I_next; i++)
	{
		Inverted_IndexRecord temp = I_index[i];
		f3out.write((char*)&temp, sizeof(temp)); //fixed len record, fixed len field
	}
	fout.close();
	f2out.close();
	f3out.close();
	ChangeIndexState(false);
}

void SimpleIndex::loadIndex()
{
	if(!exists(dataFilePath))
	{
		next = 0;
		ofstream fout;
		fout.open(dataFilePath, ios::app | ios::out | ios::binary);
		fout << 0;
		fout.close();
	}

	else if (!getIndexState())
	{
		ifstream fin(indexFilePath);
		next = 0;
		while(true)
		{
			IndexRecord temp;
			fin.read((char*)&temp, sizeof(temp));
			if (fin.eof()) break;
			index[next] = temp;
			next++;
		}
		fin.close();


        ifstream f2in(S_indexFilePath);
		s_next = 0;
		while(true)
		{
			IndexRecord temp2;
			f2in.read((char*)&temp2, sizeof(temp2));
			if (f2in.eof()) break;
			s_index[s_next] = temp2;
			s_next++;
		}
		f2in.close();


        ifstream f3in(Inverted_Path);
		I_next = 0;
		while(true)
		{
			Inverted_IndexRecord temp2;
			f3in.read((char*)&temp2, sizeof(temp2));
			if (f3in.eof()) break;
			I_index[I_next] = temp2;
			I_next++;
		}
		f3in.close();
	}
	else ReconstructIndex();
}
void SimpleIndex::ReconstructIndex()
{
	ifstream fin;
	fin.open("D:\\dataFile.txt", ios::in | ios::binary);
	next = 0;
	fin.seekg(headerSize, ios::beg); //skip header
	char *buffer;
	while (true)
	{
		IndexRecord temp;
		int offset = fin.tellg();
		short len;
		fin.read((char*)&len, sizeof(len));
		if (fin.eof()) break;
		buffer = new char[len];
		fin.read(buffer, len);
		if (buffer[0] == '*') continue;

		istringstream strbuf(buffer);
		strbuf.getline(temp.PK, 5, '|');
		temp.offset = offset;
		index[next] = temp;
		next++;
	}
	fin.close(); delete []buffer;
	sortIndex();
}

void SimpleIndex::ChangeIndexState(bool state)
{
	fstream fout(dataFilePath, ios::out | ios::in);
	fout.seekp(0, ios::beg);
	fout << state;
	fout.close();
}
bool SimpleIndex::getIndexState()
{
	bool state;
	ifstream fout(dataFilePath);
	fout.seekg(0, ios::beg);
	fout >> state;
	fout.close();
	return state;
}
void SimpleIndex::sortIndex()
{
	int len = next - 1;
	IndexRecord temp;
	for (int i = 0; i<len; i++)
		for (int j = 0; j<len - i; j++)
		{
			if (atoi(index[j].PK)>atoi(index[j + 1].PK))
			{
				temp = index[j];
				index[j] = index[j + 1];
				index[j + 1] = temp;
			}
		}
}

void SimpleIndex::sorts_Index()
{
	int len = s_next - 1;
	IndexRecord temp;
	for (int i = 0; i<len; i++)
		for (int j = 0; j<len - i; j++)
		{
			if (atoi(s_index[j].PK)>atoi(s_index[j + 1].PK))
			{
				temp = s_index[j];
				s_index[j] = s_index[j + 1];
				s_index[j + 1] = temp;
			}
		}
}
void SimpleIndex::Updata_PK()
{
    cout << "\nEnter PK to Update  \n";
    char PK[6];
    cin.getline(PK,5);
	deleteStudent(PK);
	insertStudent();
}
void SimpleIndex::Updata_I_Name()
{
    cout << "\nEnter I_Name to Update  \n";
    char PK[6];
    cin.getline(PK,5);
	delete_I_name(PK);
	insertStudent();

}
int main()
{

	SimpleIndex i;
	i.loadIndex();
	for (int n = 0; n < 3;n++)
		i.insertStudent();
	for (int n = 0; n < 3; n++)
        {
            char PK[6];
            cout << "\nenter PK you want to display \n";
            cin.getline(PK,5);
            i.printStudent(PK);
        }
//    cout << "\nenter PK to delete  \n";
//    char PK[6];
//    cin.getline(PK,5);
//	i.deleteStudent(PK);
//
//    cout << "\nenter I_Name to delete  \n";
//    char name[6];
//    cin.getline(name,5);
//	i.delete_I_name(name);
//    cout << "\nenter I_Name to Print  \n";
//    char namee[6];
//    cin.getline(namee,5);
//    i.print_I_Name(namee);
    i.Updata_PK();
    	for (int n = 0; n < 3; n++)
        {
            char PK[6];
            cout << "\nenter PK you want to display \n";
            cin.getline(PK,5);
            i.printStudent(PK);
        }
    i.Updata_I_Name();
    	for (int n = 0; n < 3; n++)
        {
            char PK[6];
            cout << "\nenter PK you want to display \n";
            cin.getline(PK,5);
            i.printStudent(PK);
        }
    i.printIndex();
	i.saveIndex();



}
