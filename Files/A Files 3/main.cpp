#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdlib.h>

using namespace std;
class SimpleIndex
{
	int next;//next free slot in index
	int nnext;
	int nnnext;
	int rrn;
	int headerSize;
	struct student
	{
		char Course_ID[6];		char Course_Name[20];	char Course_Instructor[20]; short Course_Weeks;
	};
	struct IndexRecord
	{
		char PK[6];		        int offset;
	};
    struct SecIndexRecord
	{
		char instructor[20];		int offset;
	};
	struct Inverted_IndexRecord
	{
		int p1 ; char PK[6];		int p2;
	};

	const static int indexSize = 100;
	IndexRecord index[indexSize];
	SecIndexRecord iindex[indexSize];
	Inverted_IndexRecord iiindex[indexSize];
	char dataFilePath[20] = "dataFile.txt";
	char indexFilePath[20] = "indexFile.txt";
	char secIndexFilePath[20] = "secIndexFile.txt";
	char index3FilePath[20] ="index3FilePath.txt";
public:
	SimpleIndex();
	int IndexBinarySearch(char key[]);
	int SecIndexBinarySearch(char instructor[]);
	int ToCheckIndexBinarySearch(char key[]);
	void printStudent(char PK[]);
	void insertStudent();
	void deleteStudent(char PK[6]);
	void saveIndex();
	void loadIndex();
	void ReconstructIndex();
	void ReconstructSecIndex();
	void sortIndex();
	void sortSecIndex();
	void ChangeIndexState(bool state);
	bool getIndexState();
	bool exists(char name[20]);
	void printIndex();
	void printByInstructor(char instructor[]);
	void DeletByInstructor(char instructor[]);
	void updateByInstructor(char instructor[]);
	void updateByID(char PK[]);
};
SimpleIndex::SimpleIndex()
{
	next = 0;
	nnext = 0;
	nnnext = 0;
	rrn = 0;
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
int SimpleIndex::SecIndexBinarySearch(char key[])
{

	int size = nnext;
	int low = 0, high = size - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (strcmp(iindex[middle].instructor,key) == 0)
        {
            return iindex[middle].offset;
        }
		else if (strcmp(iindex[middle].instructor,key) < 0)
			low = middle + 1;
		else
			high = middle - 1;

	}
	return -1;
}
int SimpleIndex::ToCheckIndexBinarySearch(char key[])
{

	int size = nnext;
	int low = 0, high = size - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (strcmp(iiindex[middle].PK,key) == 0)
        {
            return middle;
        }
		else if (atoi(iiindex[middle].PK)<atoi(key))
			low = middle + 1;
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
		cout << "\nthis student not exists";
		return;
	}
	short len;
	student s;
	fin.seekg(index[address].offset, ios::beg);
	fin.read((char*)&len, sizeof(len));

	char *buffer = new char[len];

	fin.read(buffer, len);
	istringstream stream(buffer);
	stream.getline(s.Course_ID, 5, '|');
	stream.getline(s.Course_Name, 20, '|');
	stream.getline(s.Course_Instructor, 20, '|');
	stream.getline((char*)&s.Course_Weeks,2, '|');
	cout << endl << "ID: " << s.Course_ID << "  Course name: " << s.Course_Name << "  Course instructor: " << s.Course_Instructor<< "  Number of weeks: " << s.Course_Weeks<<endl;
	fin.close(); delete [] buffer;

}
void SimpleIndex::printByInstructor(char instructor[20])
{
    int pos = SecIndexBinarySearch(instructor);
    if(pos == -1)
    {
        cout << "\nthis student not exists";
        return;
    }
    for (int i = 0;i < nnnext;i++)
    {
        if(pos == iiindex[i].p1 && iiindex[i].p2 == -1)
        {
                printStudent(iiindex[i].PK);
                return;
        }
        else if(pos == iiindex[i].p1 && iiindex[i].p2 != -1)
        {
            for(int j = i;j < nnnext;)
            {
                printStudent(iiindex[j].PK);
                j = iiindex[j].p2;
                if(j == -1)return;
            }
        }
    }
}
void SimpleIndex::DeletByInstructor(char instructor[20])
{
    int pos = SecIndexBinarySearch(instructor);
    if(pos == -1)
    {
        cout << "\nthis student not exists";
        return;
    }
    for (int i = 0;i < nnnext;i++)
    {
        if(pos == iiindex[i].p1 && iiindex[i].p2 == -1)
        {
                deleteStudent(iiindex[i].PK);
                return;
        }
        else if(pos == iiindex[i].p1 && iiindex[i].p2 != -1)
        {
            for(int j = i;j < nnnext;)
            {
                deleteStudent(iiindex[j].PK);
                j = iiindex[j].p2;
                if(j == -1)return;
            }
        }
    }
}
void SimpleIndex::insertStudent()
{
	ChangeIndexState(true);
	ofstream fout;
	fout.open(dataFilePath, ios::app | ios::out | ios::binary);

	student s;
	cout << "\nEnter ID: ";			     cin.getline(s.Course_ID, 5);
    cout << "\nEnter Instructor: ";	     cin.getline(s.Course_Instructor,20);
	cout << "\nEnter Name: ";		     cin.getline(s.Course_Name,20);
    cout << "\nEnter Number of weeks: "; cin.getline((char*)&s.Course_Weeks,2);



	char buffer[200];
	strcpy(buffer, s.Course_ID);			        strcat(buffer, "|");
    strcat(buffer,s.Course_Name);	                strcat(buffer, "|");
	strcat(buffer,s.Course_Instructor);		        strcat(buffer, "|");
	strcat(buffer,(char*)& s.Course_Weeks);	        strcat(buffer, "|");

	short len = strlen(buffer);

	fout.seekp(0, ios::end);
	int addess = fout.tellp();
	fout.write((char*)&len, sizeof(len));
	fout.write(buffer, len);

	IndexRecord temp,temp1;
	SecIndexRecord tmp;
	strcpy(temp.PK, s.Course_ID);
	temp.offset = addess;
	index[next] = temp;
    next++;
	sortIndex();


	if(SecIndexBinarySearch(s.Course_Instructor) == -1)
    {
        strcpy(tmp.instructor, s.Course_Instructor);
        tmp.offset = nnnext;
        iindex[nnext] = tmp;
        nnext++;

        iiindex[nnnext].p1 = nnnext;
        strcpy(iiindex[nnnext].PK, s.Course_ID);
        iiindex[nnnext].p2 = -1;
        nnnext++;

    }
    else
    {
        int pos = SecIndexBinarySearch(s.Course_Instructor);
        int tmp =iindex[pos].offset;
        if(iindex[pos].offset != -1)
        {
            for(int i = 0;i<nnnext;i++)
            {
                if(tmp == iiindex[i].p1 && iiindex[i].p2 == -1 )
                {
                    iiindex[i].p2 = nnnext;

                    iiindex[nnnext].p1 = nnnext;
                    strcpy(iiindex[nnnext].PK, s.Course_ID);
                    iiindex[nnnext].p2 = -1;
                    nnnext++;
                }
                else if(tmp == iiindex[i].p1 && iiindex[i].p2 != -1)
                {
                    tmp=iiindex[i].p2;
                }
            }

        }
    }
    sortIndex();
    sortSecIndex();
    fout.close();

}
void SimpleIndex::deleteStudent(char PK[6])
{
	ChangeIndexState(true);
	fstream fout(dataFilePath, ios::binary | ios::out | ios::in);
	int address = IndexBinarySearch(PK);
	if (address == -1)
	{
		cout << "\nStudent not found";
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
void SimpleIndex::saveIndex()
{
	ofstream fout(indexFilePath, ios::trunc);
	for (int i = 0; i<next; i++)
	{
		IndexRecord temp = index[i];
		fout.write((char*)&temp, sizeof(temp));
	}

	ofstream f1(secIndexFilePath, ios::trunc);
	for (int i = 0; i<nnext; i++)
	{
		SecIndexRecord temp = iindex[i];
        f1.write((char*)&temp, sizeof(temp));
	}

	ofstream f2(index3FilePath, ios::trunc);
	for (int i = 0; i< nnnext; i++)
	{
		Inverted_IndexRecord temp = iiindex[i];
		f2.write((char*)&temp, sizeof(temp));
	}
	fout.close();
	f1.close();
	f2.close();
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
			fin.read((char*)&temp, sizeof(temp)); if (fin.eof()) break;
			index[next] = temp;
			next++;
		}
		fin.close();

        ifstream f1;
		f1.open(secIndexFilePath);
		nnext = 0;
		while(true)
		{
			SecIndexRecord temp;
			f1.read((char*)&temp, sizeof(temp)); if (f1.fail()) break;
			iindex[nnext] = temp;
			nnext++;
		}
		f1.close();

        ifstream f2(index3FilePath);
        nnnext = 0;
		while(true)
		{
			Inverted_IndexRecord temp2;
			f2.read((char*)&temp2, sizeof(temp2));
			if (f2.eof()) break;
			iiindex[nnnext] = temp2;
			nnnext++;
		}
		f2.close();
	}
	else ReconstructIndex();
}
void SimpleIndex::ReconstructIndex()
{
	ifstream fin;
	fin.open("dataFile.txt", ios::in | ios::binary);
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
		fin.read(buffer, len);				if (buffer[0] == '*') continue;
		istringstream strbuf(buffer);
		strbuf.getline(temp.PK, 5, '|');	temp.offset = offset;
		index[next] = temp;
		next++;
	}
	fin.close(); delete []buffer;
	sortIndex();
}
void SimpleIndex::ReconstructSecIndex()
{
	ifstream fin;
	fin.open("dataFile.txt", ios::in | ios::binary);
	nnext = 0;
	fin.seekg(headerSize, ios::beg); //skip header
	char *buffer;
	while (true)
	{
		SecIndexRecord temp;
		int offset = fin.tellg();
		short len;
		fin.read((char*)&len, sizeof(len));
		if (fin.eof()) break;
		buffer = new char[len];
		fin.read(buffer, len);				if (buffer[0] == '*') continue;
		istringstream strbuf(buffer);
		strbuf.getline(temp.instructor, 20, '|');	temp.offset = nnnext;
		iindex[nnext] = temp;
		nnext++;
	}
	fin.close(); delete []buffer;
	sortSecIndex();
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
void SimpleIndex::sortSecIndex()
{
	int len = nnext - 1;
	SecIndexRecord temp;

    for (int i = 0; i<len; i++)
		for (int j = 0; j<len - i; j++)
		{
			if (atoi(iindex[j].instructor)>atoi(iindex[j + 1].instructor))
			{
				temp = iindex[j];
				iindex[j] = iindex[j + 1];
				iindex[j + 1] = temp;
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
	for(int i=0;i<nnext;i++)
	{
		cout<<iindex[i].instructor<<"  "<<iindex[i].offset<<endl;
	}
	cout <<"I_Index"<<endl;
	for(int i=0;i<nnnext;i++)
	{
		cout<<iiindex[i].p1<<"  "<<iiindex[i].PK<<"  "<<iiindex[i].p2<<endl;
	}
}
void SimpleIndex::updateByInstructor(char instructor[20])
{
    int pos = SecIndexBinarySearch(instructor);
    if(pos == -1)
    {
        cout << "\nthis student not exists";
        return;
    }
    for (int i = 0;i < nnnext;i++)
    {
        if(pos == iiindex[i].p1 && iiindex[i].p2 == -1)
        {
                cout << "\nPlease enter instructor name : ";
                char inst[20];
                cin.getline(inst,19);
                strcpy(iindex[i].instructor,inst);
                return;
        }
        else if(pos == iiindex[i].p1 && iiindex[i].p2 != -1)
        {
                cout << "\nPlease enter new instructor name : ";
                char inst[20];
                cin.getline(inst,19);
            for(int j = i;j < nnnext;)
            {

                strcpy(iindex[i].instructor,inst);
                j = iiindex[j].p2;
                if(j == -1)return;
            }
        }
    }
}
void SimpleIndex::updateByID(char PK[6])
{
    int pos =IndexBinarySearch(PK);
    int pos1 =ToCheckIndexBinarySearch(PK);
    if(pos == -1)
    {
        cout << "\nthis student not exists";
        return;
    }
    else
    {
        cout << "\nPlease enter new ID  : ";
        char ID[6];
        cin.getline(ID,5);
        strcpy(index[pos].PK,ID);
        strcpy(iiindex[pos1].PK,ID);
        int in = ToCheckIndexBinarySearch(ID);
        strcpy(iiindex[in].PK,ID);
        return;
    }

}
char menu()
{
    char choice;

    cout<<"Menu\n";
    cout<<"1) Add New course.\n";
    cout<<"2) Delete course (ID).\n";
    cout<<"3) Delete course (instructor name).\n";
    cout<<"4) Print course (ID).\n";
    cout<<"5) Print course (instructor name).\n";
    cout<<"6) update course (ID).\n";
    cout<<"7) update course (instructor name).\n";
    cout<<"8) Exit.\n";
    cin>>choice;
    cin.ignore();

    return choice;
}
int main()
{

	SimpleIndex i;
	i.loadIndex();


    char choice;
    char instructor[20];
    char PK[6];


    do{
            choice = menu();
            switch(choice)
            {
                case'1':cout<<"Please enter a student  :\n";
                        i.insertStudent();
                        i.printIndex();
                        break;
                case'2':cout<<"Enter ID to Delete :\n";
                        cin.getline(PK,5);
                        i.deleteStudent(PK);
                        i.printIndex();
                        break;
                case'3':cout<<"Enter Instructor to Delete :\n";
                        cin.getline(instructor,19);
                        i.DeletByInstructor(instructor);
                        i.printIndex();
                        break;
                case'4':cout<<"Enter ID to Print :\n";
                        cin.getline(PK,5);
                        i.printStudent(PK);
                        i.printIndex();
                        break;
                case'5':cout<<"Enter Instructor to Print :\n";
                        cin.getline(instructor,19);
                        i.printByInstructor(instructor);
                        i.printIndex();
                        break;
                case'6':cout<<"Enter ID to Update :\n";
                        cin.getline(PK,5);
                        i.updateByID(PK);
                        i.printIndex();
                        break;
                case'7':cout<<"Enter Instructor to Update :\n";
                        cin.getline(instructor,19);
                        i.updateByInstructor(instructor);
                        i.printIndex();
                        break;
                default:
                    cout<<"Exiting\n";
            }

    }while(choice != '8');
//	for (int n = 0; n < 5;n++)
//		i.insertStudent();
//	for (int n = 0; n < 4; n++)
//		i.printStudent();

	//i.deleteStudent();

//	for (int n = 0; n < 2; n++)
//	i.printStudent();

    //cout<<"Binary search sec index rrn "<<i.SecIndexBinarySearch("mnmn")<<endl;
//    i.printIndex();
//    char inst[20];
//    cin.getline(inst,19);
//    i.DeletByInstructor(inst);
//    cout<<endl;
//    cin.getline(inst,19);
//    i.DeletByInstructor(inst);
//    cout<<endl;
//    cin.getline(inst,19);
//    i.printByInstructor(inst);
//    cout<<endl;
//    cin.getline(inst,19);
//    i.printByInstructor(inst);
//    cout<<endl;
//    i.updateByID("100");
//    i.printIndex();
    i.saveIndex();



}
