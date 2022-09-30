// 
// OS_HW2
// ��T�T�A 10827234 �^�ۺ�
//

#include <iostream>
#include <string.h> // strcmp
#include <vector>
#include <fstream>	// open, is_open, close, write, ignore
#include <stdlib.h>  
#include <iomanip> // setw

using namespace std;

string inFileName = "";
const float EPSINON = 0.001; // ������I�ƬO�_�۵���

enum SortType{ arrivalTime = 22, processID = 33, priority = 44} ;

typedef struct pI{
    int pID;
    int cpuBurst;
    int arrivalTime;
    int priority;
    int exeTime;
    int eachTimeSlice;
} processInfo;

typedef struct tI{
    int pID;
    int waitingTime;
    int turnaroundTime;
} timeInfo; // 

char pIDList[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',   //  0 - 9
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',   // 10 - 19
                    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',   // 20 - 29
                    'U', 'V', 'W', 'X', 'Y', 'Z'} ; // pID ��Ӫ�         // 30 - 35

bool openFile( fstream &inFile ) ; // read records from a file
void writeFile( vector<vector<char>>, vector<vector<timeInfo>>, vector<string> ) ; // write datas  into a file
void NewBubbleSort(vector<pI>& vTemp, SortType type) ;
void NewBubbleSort(vector<tI>& vTemp, SortType type);

template <typename T>
class QueueArray {
public:

    QueueArray() : front(-1), back(-1), capacity(1) {
        ptr = new T[capacity];
    } // constructure

    void initial() {
        front  = -1, back = -1, capacity = 1 ;
        delete[] ptr;
        ptr = new T[capacity];
    }
    void deletePtr() {
        delete[] ptr;
    }

    void Push(T data){ // ���Ʃ��̫��
        if ((back + 1) % capacity == front) // �p�G�C�Ӧ�m���񺡤F�A��t�m�}�C�j�p�ܬ�2���A����
        {
            DoubleCapacity();
        }
        if (isEmpty()) // �p�GQueue���šA�hPush��front�n�t�~�[1�A��front�ܬ�0
        {
            front++;
        }
        ptr[(back + 1) % capacity] = data;
        back = (back + 1) % capacity;

        
        //cout << "!!!!back: " << back << "\t!!!!front: " << front << endl ;
    } // push()

    void Pop(){ // �⳻�ݪ���Ʋ���
        if (isEmpty()){
            cout << "Queue is empty." << endl;
        } // if
        else if( getSize() == 1) { // �Y�R���e�u�Ѥ@�ӫh�R���ᬰ��
            front = -1 ;
            back = -1;
        }
        else{
            front = (front + 1) % capacity; // �u�ݭn��front�[1�N��A�]������p�G�����Push�i�ӡA�|�����л\���쥻���A������O�����Adelete��new
        } // else
    } // pop ()

    bool isEmpty() { // �^��Queue�O�_����
        return (front == -1);
    } // is Empty

    int getSize(){ // �^��Queue�j�p
        if (isEmpty()){
            return 0;
        } // if
        else if (front == back){ // �p�Gfront�Mback���P�@�Ӧ�m
            return 1;
        } // else if
        else if (front < back){ // �p�Gfront�bback�e��
            return back - front + 1;
        }// else if
        else{ // �p�Gfront�bback�᭱
            return capacity - (front - back) + 1;
        } // else
    }

    T* getFront(){ // �^�ǳ̫e�ݪ����
        if (isEmpty()){
            cout << "Queue is empty." << endl;
            return nothing; // �^��0�O�]�������DT�O���򫬺A�A�p�G�^�ǧO���ƭȷ|�ܮe���X��
        } // if

        //cout << "front: " << front << endl ;
        return &ptr[front];
    } // getFront

    T* getBack() { // �^�ǳ̫�ݪ����
        if (isEmpty()){
            cout << "Queue is empty." << endl;
            return nothing; // �^��0�O�]�������DT�O���򫬺A�A�p�G�^�ǧO���ƭȷ|�ܮe���X��
        } // if

        return &ptr[back];
    } // getBack

private:
    int front;            // �̫e�ݪ�index
    int back;             // �̫�ݪ�index
    int capacity;         // �ثe�t�m�Ŷ����j�p
    T *ptr;               // �ثe�t�m���}�C(�Ĥ@�Ӥ���������)
    T *nothing;
    void DoubleCapacity(){ // ��t�m���}�C�ܦ��⭿���j�p
        T *newptr = new T[capacity * 2];
        int j = front - 1;
        for (int i = 0; i < capacity; i++) // ���ª��}�C�̪���ƽƻs��s���}�C
        {
            newptr[i] = ptr[(++j) % capacity];
        } // for
        delete[] ptr;
        ptr = newptr;
        front = 0;
        back = capacity - 1;
        capacity *= 2;
    } // DoubleCapacity
};

class FCFS{ // methd 1
// �� queue
public:
    QueueArray<pI> readyQueue;
    vector<tI> timeList;
    vector<char> ganttList;
    void initial() { 
        readyQueue.initial(); // queue �j�p
        ganttList.clear();
        timeList.clear() ;
    };

    void scheduling(vector<processInfo> allProcessInfo);
} ;

class RR{ // methd 2
// �� queue
public:
    vector<pI> readyArray;
    vector<tI> timeList;
    vector<char> ganttList;
    void initial() { 
        readyArray.clear() ;
        ganttList.clear();
        timeList.clear() ;
    };

    void scheduling(vector<processInfo> allProcessInfo, int timeSlice);
} ;

class SRTF{ // methd 3
// ���ݭnqueue ��array �Y�i readyArray�̭��]�����Ƨ�  �]���C�����O���̳Ѿl�ɶ��p���X�� ���v�T
public:
    vector<pI> readyArray;
    vector<tI> timeList;
    vector<char> ganttList;
    void initial() { 
        readyArray.clear();
        ganttList.clear();
        timeList.clear() ;
    };

    void scheduling(vector<processInfo> allProcessInfo);
    int findShortestProcess(vector<processInfo> remainProcess); // �^�� ������index
} ;

class PPRR{ // methd 4
// ���ݭnqueue ��array �Y�i readyArray�̭��]�����Ƨ�  �]���C�����O���̳Ѿl�ɶ��p���X�� ���v�T
public:
    vector<pI> readyArray;
    vector<tI> timeList;
    vector<char> ganttList;
    void initial() { 
        readyArray.clear() ;
        ganttList.clear();
        timeList.clear() ;
    };

    void scheduling(vector<processInfo> allProcessInfo, int timeSlice);
    int findMinPriority(vector<processInfo> remainProcess) ; // �^�� ���̤ppriority ��process �� index
} ;

class HRRN{ // methd 5
// ���ݭnqueue ��array �Y�i readyArray�̭��]�����Ƨ�  �]���C�����O����v�̤p���X�� ���v�T
public:
    vector<pI> readyArray;
    vector<tI> timeList;
    vector<char> ganttList;
    void initial() { 
        readyArray.clear();
        ganttList.clear();
        timeList.clear() ;
    };

    void scheduling(vector<processInfo> allProcessInfo);
    int findHighestRatioProcess(vector<processInfo> remainProcess, int clock); // �^�� �̰��ϬM�t�v������index
    float calResponseRation(processInfo tempInfo, int clock); // �p������t�v
} ;

int main() {
    int method = 0, timeSlice = 0;
    bool wantExe = false ;
    vector<processInfo> allProcessInfo;

    vector<vector<char>> allGantt;
    vector<vector<timeInfo>> allTime;
    vector<string> methods;
    string stringTemp = "\0" ;
    
    FCFS fcfsItem;
    RR rrItem;
    SRTF srtfItem ;
    PPRR pprrItem;
    HRRN hrrnItem;
    
    do {
        cout << endl << "*******************  Scheduling  ********************";
        cout << endl << "* 1. [FCFS] --> First Come First Serve              *";
        cout << endl << "* 2. [RR] ----> Round Robin                         *";
        cout << endl << "* 3. [SRTF] --> Shortest Remaining Time First       *";
        cout << endl << "* 4. [PPRR] --> Preemptive Priority + Round Robin   *";
        cout << endl << "* 5. [HRRN] --> Highest Response Ratio Next         *";
        cout << endl << "* 6. [ALL] ---> Execute ALL method                  *";
        cout << endl << "*****************************************************" << endl;

        fstream inputFile;
        /* Initialize */ allProcessInfo.clear() ; 
        /* Initialize */ fcfsItem.initial(); rrItem.initial(); srtfItem.initial() ; pprrItem.initial(); hrrnItem.initial();
        /* Initialize */ allGantt.clear() ; allTime.clear(); methods.clear() ;

        wantExe = openFile(inputFile); // ��J "0" return false �_�h����Ū�J���쥿�T�}���ɮ�

        if ( wantExe ) { 
            inputFile >> method >> timeSlice ;// read records from a file
            
            getline(inputFile, stringTemp) ; // '\n'
            getline(inputFile, stringTemp) ; //�N�ĤG�CŪ��

            // �x�s process Info
            processInfo tempInfo;
            while(inputFile >> tempInfo.pID){
                inputFile  >> tempInfo.cpuBurst >> tempInfo.arrivalTime >> tempInfo.priority ;
                tempInfo.exeTime = 0 ;
                tempInfo.eachTimeSlice = 0 ;
                allProcessInfo.push_back(tempInfo);
            } // while
            
            NewBubbleSort( allProcessInfo, processID); // �H pID �Ƨ�
            NewBubbleSort( allProcessInfo, arrivalTime); // �H arrival time �Ƨ�
            // ���H pID �ƦA�H arrival time �h�i�H�T�Oqueue �q�̫e�����X�����Ǭ� �ɶ��̤p  �p�ɶ��@�˫hpID �p

            int time = 0 ; // time ���� cpuTime �C���^�� + �@��
            switch (method) {
            case 1 :
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 1: [FCFS]     ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;
                
                fcfsItem.scheduling(allProcessInfo);
                NewBubbleSort(fcfsItem.timeList, processID);
                methods.push_back("FCFS");
                allGantt.push_back(fcfsItem.ganttList);
                allTime.push_back(fcfsItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 2 :
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��      Method 2: [RR]      ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;
                
                rrItem.scheduling(allProcessInfo, timeSlice) ;
                NewBubbleSort(rrItem.timeList, processID);
                methods.push_back("RR");
                allGantt.push_back(rrItem.ganttList);
                allTime.push_back(rrItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 3:
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 3: [SRTF]     ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;

                srtfItem.scheduling(allProcessInfo) ;
                NewBubbleSort(srtfItem.timeList, processID);
                methods.push_back("SRTF");
                allGantt.push_back(srtfItem.ganttList);
                allTime.push_back(srtfItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 4:
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 4: [PPRR]     ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;

                pprrItem.scheduling(allProcessInfo, timeSlice) ;
                NewBubbleSort(pprrItem.timeList, processID);
                methods.push_back("PPRR");
                allGantt.push_back(pprrItem.ganttList);
                allTime.push_back(pprrItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 5:
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 5: [HRRN]     ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;

                hrrnItem.scheduling(allProcessInfo);
                NewBubbleSort(hrrnItem.timeList, processID);
                methods.push_back("HRRN");
                allGantt.push_back(hrrnItem.ganttList);
                allTime.push_back(hrrnItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;
            
            case 6:
                cout << "�ݢ�����������������������������������������������������" << endl;
                cout << "��     Method 6: [ALL]      ��" << endl;
                cout << "�㢤����������������������������������������������������" << endl;
                
                fcfsItem.scheduling(allProcessInfo); 
                
                rrItem.scheduling(allProcessInfo, timeSlice) ;
                srtfItem.scheduling(allProcessInfo);
                pprrItem.scheduling(allProcessInfo, timeSlice) ;
                hrrnItem.scheduling(allProcessInfo);
                
                methods.push_back("FCFS");
                methods.push_back("RR");
                methods.push_back("SRTF");
                methods.push_back("PPRR");
                methods.push_back("HRRN");

                allGantt.push_back(fcfsItem.ganttList);
                allGantt.push_back(rrItem.ganttList);
                allGantt.push_back(srtfItem.ganttList);
                allGantt.push_back(pprrItem.ganttList);
                allGantt.push_back(hrrnItem.ganttList);

                NewBubbleSort(fcfsItem.timeList, processID);
                NewBubbleSort(rrItem.timeList, processID);
                NewBubbleSort(srtfItem.timeList, processID);
                NewBubbleSort(pprrItem.timeList, processID);
                NewBubbleSort(hrrnItem.timeList, processID);

                allTime.push_back(fcfsItem.timeList);
                allTime.push_back(rrItem.timeList);
                allTime.push_back(srtfItem.timeList);
                allTime.push_back(pprrItem.timeList);
                allTime.push_back(hrrnItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 0:break;
            default: cout << endl << "Command does not exist!" << endl;
            } // switch(command)
        } // if

        inputFile.close();

    } while (method != 0 && wantExe); // method != 0 & �Q�~�����

    return 0 ;
} // main()

bool openFile( fstream &inFile ) { // read records from a file
	string fileName = "";

    do {
        cout << endl << "Input a file number(eg.input1) ([0]:quit!):";
        cin >> inFileName;

        if(strcmp(inFileName.c_str(), "0") == 0 )
            break ;

        fileName = inFileName + ".txt";
        inFile.open(fileName, ios::in); 
        if( inFile )
            return true ;
        else 
            cout << endl << "### " << fileName << " does not exist! ###" << endl ;

    }while( !inFile ) ;

	return false ; // return  
} // openInputFile() 

void writeFile( vector<vector<char>> allGantt, vector<vector<timeInfo>> allTime, vector<string> methods) { // write datas  into a file { // write datas into a file
	string fileName = "out_" + inFileName + ".txt" ;
	fstream temp ;  
	
	temp.open( fileName, ios :: out ) ; 

    if( methods.size() == 1) {
        if( strcmp(methods.at(0).c_str(), "PPRR") == 0)
            temp << "Priority RR" << endl ;
        else
            temp << methods.at(0) << endl;
    }
    else
        temp << "All" << endl ;

    for( int i = 0 ; i < allGantt.size() ; i ++) {
        temp << "==" << setw(12) << methods.at(i) << "==" << endl;
        for( int x = 0 ; x < allGantt.at(i).size() ; x ++) {
            temp << allGantt.at(i).at(x) ; // ��X�� i �ӥ̯S��
        } // for

        temp << endl ;
    } // for
    temp << "===========================================================" << endl; // '=' x 59

    // waitingTime
    // ID	FCFS	RR	SRTF	PPRR	HRRN
    temp << "\nWaiting Time" << endl << "ID" ; 
    for( int i = 0 ; i < allTime.size() ; i++) {
        temp << "\t" << methods.at(i) ;
    } // for
    
    
    // �ƭ�
    temp << "\n===========================================================" << endl;
    for( int i = 0 ; i < allTime.at(0).size() ; i++) { // ���X��pId �N�]�X��
        
        temp << allTime.at(0).at(i).pID; // ���L �� i �� pID
        for( int j = 0 ; j < allTime.size() ; j++) { // ���X�� method(s) �N�]�X��
            temp << "\t" << allTime.at(j).at(i).waitingTime ;
        } // for

        temp << endl ;
    } // for
    temp << "===========================================================" << endl;

    // turnaroundTime
    // ID	FCFS	RR	SRTF	PPRR	HRRN
    temp << "\nTurnaround Time" << endl << "ID" ; 
    for( int i = 0 ; i < allTime.size() ; i++) {
        temp << "\t" << methods.at(i);
    } // for
    
    // �ƭ�
    temp << "\n===========================================================" << endl;
    for( int i = 0 ; i < allTime.at(0).size() ; i++) { // ���X��pId �N�]�X��
        
        temp << allTime.at(0).at(i).pID; // ���L �� i �� pID
        for( int j = 0 ; j < allTime.size() ; j++) { // ���X�� method(s) �N�]�X��
            temp << "\t" << allTime.at(j).at(i).turnaroundTime ;
        } // for

        temp << endl ;
    } // for
    temp << "===========================================================" << endl << endl ;

    temp.close() ;

	return ;
} // WriteFile()

void NewBubbleSort(vector<pI>& vTemp, SortType type) {
    bool sorted = true;
    unsigned int n = vTemp.size() - 1;

    switch(type){
    case arrivalTime :
        for (unsigned int i = 0; i <= n && sorted; i++) {
            sorted = false;
            for( unsigned int j = 1 ; j <= n ; j++) {
                if (vTemp.at(j-1).arrivalTime > vTemp.at(j).arrivalTime) {
                    swap(vTemp[j], vTemp[j - 1]);
                    sorted = true;
                } // end if()
            } // inner for
        } // out for
        break ;

    case processID:
        for (unsigned int i = 0; i <= n && sorted; i++) {
            sorted = false;
            for( unsigned int j = 1 ; j <= n ; j++) {
                if (vTemp.at(j-1).pID > vTemp.at(j).pID) {
                    swap(vTemp[j], vTemp[j - 1]);
                    sorted = true;
                } // end if()
            } // inner for
        } // out for
        break ;

    case priority:
        for (unsigned int i = 0; i <= n && sorted; i++) {
            sorted = false;
            for( unsigned int j = 1 ; j <= n ; j++) {
                if (vTemp.at(j-1).priority > vTemp.at(j).priority) {
                    swap(vTemp[j], vTemp[j - 1]);
                    sorted = true;
                } // end if()
            } // inner for
        } // out for
        break ;

    default: break;
    } // switch
    
    return;
} // BubbleSort()

void NewBubbleSort(vector<tI>& vTemp, SortType type){
    bool sorted = true;
    unsigned int n = vTemp.size() - 1;

    switch(type){
    case processID:
        for (unsigned int i = 0; i <= n && sorted; i++) {
            sorted = false;
            for( unsigned int j = 1 ; j <= n ; j++) {
                if (vTemp.at(j-1).pID > vTemp.at(j).pID) {
                    swap(vTemp[j], vTemp[j - 1]);
                    sorted = true;
                } // end if()
            } // inner for
        } // out for
        break ;

    default: break;
    } // switch
    
    return;
}

void FCFS::scheduling(vector<processInfo> allProcessInfo) {
    processInfo *currentProcess ; // �Ȧs running ���� process
    int  processCount = 0, index = 0; // index ��ܥثe allProcessInfo�� �ĴX�� process
    int clock = 0;
    while(1) {
        for( index; index < allProcessInfo.size() ; index ++ ){ // ���ɦ��� �N process ��i ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // �ثe�ɶ����F���F
                this->readyQueue.Push(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        // non-preemptive
        if(!this->readyQueue.isEmpty()){ // reagy queue �����F��
            currentProcess = this->readyQueue.getFront(); // �N�̫e���� process ���X�� 
            currentProcess->exeTime++ ;
                // ���L�]�F1��
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // ø�s�̯S�� �NpID �����N�X

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // �� process �w�g���檱�F
                this->readyQueue.Pop() ; // �R���e����
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;
        
                this->timeList.push_back(times);
                // �p�� waiting time turnaround time
                
            } //if
        } // if
        else {
            this->ganttList.push_back('-') ; // ���ɨS�� process �H '-' �N��
        } // else

        if( this->readyQueue.isEmpty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // �ɶ��[�@

    } // while()

    this->readyQueue.initial() ; // ���� delete ptr �n���M���ĤG���|��
} // FCFS

void RR::scheduling(vector<processInfo> allProcessInfo, int timeSlice) { // NEW
    processInfo *currentProcess = NULL ; // �Ȧs running ���� process
    int  processCount = 0, index = 0; // index ��ܥثe allProcessInfo�� �ĴX�� process
    int clock = -1;
    bool continueReamin = false; // process timeOut�F  �٦��F��n�B�z
    while(1) {
        // �P�@�ɶ� ���s�i��process enter queue then �� timeout ��process �i�Jqueue
        // preemptive
        currentProcess = NULL;
        if(!this->readyArray.empty()){ // ready queue �����F��
            currentProcess = &this->readyArray.at(0); // �N�̫e���� process ���X�� 
            currentProcess->exeTime++ ;
                // ���L�]�F1��
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // ø�s�̯S�� �NpID �����N�X

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // �� process �w�g���槹�F
                
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin()) ; // �R���e����
                // �p�� waiting time turnaround time
            } //if

            else if ((currentProcess->exeTime % timeSlice) == 0){ // timeOut �R���e�� ���queue �᭱
                continueReamin = true ; // �����s process�@�i queue �A�B�z����
            } // else if
                
        } // if
        else  { // ready queue ���S���F��
            this->ganttList.push_back('-') ; // ���ɨS�� process ,�H '-' �N��?????????????????????????????????????
        } // else
        
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // �ɶ��[�@

        for( index; index < allProcessInfo.size() ; index ++ ){ // ���ɦ��� �N process ��i ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // �ثe�ɶ����F���F
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        if( continueReamin){ // ����B�z���� 
            this->readyArray.push_back(this->readyArray.front()); // �ҫ��V������ ��i queue �᭱
            this->readyArray.erase(this->readyArray.begin()) ; // �R���e����
            
            continueReamin = false ;
        } // if
        
    } // while()

    this->ganttList.erase(ganttList.begin()) ;
    //this->readyQueue.initial() ; // ���� delete ptr �n���M���ĤG���|��
} // RR

void SRTF::scheduling(vector<processInfo> allProcessInfo) {
    processInfo *currentProcess ; // �Ȧs running ���� process
    int  processCount = 0, index = 0; // index ��ܥثe allProcessInfo�� �ĴX�� process
    int clock = 0, minimunIndex = 0;
    while(1) {
        
        for( index; index < allProcessInfo.size() ; index ++ ){ // ���ɦ��� �N process ��i ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // �ثe�ɶ����F���F
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        // preemptive
        if(!this->readyArray.empty()){ // ready queue �����F��
            minimunIndex = this->findShortestProcess(readyArray); // ��X�ثe�Ѿl�ɶ��̤p��process �� index
            currentProcess = &readyArray.at(minimunIndex) ; // �� pointer ���V�Ѿl�ɶ��̤p�� process
            currentProcess->exeTime++ ;
                // ���L�]�F1��
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // ø�s�̯S�� �NpID �����N�X

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // �� process �w�g���槹�F
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin() + minimunIndex) ; // �R���ثe�� // �n���᭱�R�� �]��currentProcess ���V���F�� �n���p�⧹�~�i�⥻��R��
                // �p�� waiting time turnaround time
            } //if
                
        } // if
        else  { // ready queue ���S���F��
            if( 1) {
                this->ganttList.push_back('-') ; // ���ɨS�� process ,�H '-' �N��
            } // if
        } // else 
        
        //if( clock == 60)
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // �ɶ��[�@

    } // while()

} // SRTF

int SRTF::findShortestProcess(vector<processInfo> remainProcess){ // �^�� ������index
    int index = 0, minimize = 0;
    if(remainProcess.size() == 1){
        return 0 ;
    } // if
    else {
        minimize = remainProcess.at(0).cpuBurst - remainProcess.at(0).exeTime ; // �Ĥ@�� process ���Ѿl�ɶ�
        for( int i = 1 ; i < remainProcess.size() ; i++) {
            if( (remainProcess.at(i).cpuBurst - remainProcess.at(i).exeTime ) < minimize){
                minimize = remainProcess.at(i).cpuBurst - remainProcess.at(i).exeTime;
                index = i ;
            } // if
        } // for
    } // else

    return index ;
} // SRTF 

void HRRN::scheduling(vector<processInfo> allProcessInfo) {
    processInfo *currentProcess ; // �Ȧs running ���� process
    int  processCount = 0, index = 0; // index ��ܥثe allProcessInfo�� �ĴX�� process
    int clock = 0, MaximunIndex = 0;
    bool finished = true;
    while(1) {
        for( index; index < allProcessInfo.size() ; index ++ ){ // ���ɦ��� �N process ��i ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // �ثe�ɶ����F���F
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        // Non-preemptive
        if(!this->readyArray.empty() ){ // ready queue �����F��
            if( finished){  // �]���O nonpreemptive �e�@��process ��������~�~��
                MaximunIndex = this->findHighestRatioProcess(readyArray, clock); // ��X�ثe�Ѿl�ɶ��̤p��process �� index
                finished = false ;
            } // if
            
            // �C�����n���s�n�D���� �]�� pushback �|���ܰO�����}
            currentProcess = &readyArray.at(MaximunIndex) ; // �� pointer ���V�Ѿl�ɶ��̤p�� process
            currentProcess->exeTime++ ;
                // ���L�]�F1��
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // ø�s�̯S�� �NpID �����N�X

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // �� process �w�g���槹�F
                finished = true ; // ���槹�F �i�H���U��process 
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin() + MaximunIndex) ; // �R���ثe�� // �n���᭱�R�� �]��currentProcess ���V���F�� �n���p�⧹�~�i�⥻��R��
                // �p�� waiting time turnaround time
            } //if
                
        } // if
        else  { // ready queue ���S���F��
            if( clock >= 0) {
                this->ganttList.push_back('-') ; // ���ɨS�� process ,�H '-' �N��
            } // if
        } // else 
        
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // �ɶ��[�@

    } // while()

} // HRRN

int HRRN::findHighestRatioProcess(vector<processInfo> remainProcess, int clock){ // �^�� ������index
    int index = 0;
    float Maximun = 0.0, tempRatio = 0.0;
    if(remainProcess.size() == 1){
        return 0 ;
    } // if
    else {
        Maximun = calResponseRation(remainProcess.at(0), clock) ; // �Ĥ@�� process ��Response Ratio
        for( int i = 1 ; i < remainProcess.size() ; i++) {
            tempRatio = calResponseRation(remainProcess.at(i), clock);
            if( abs(tempRatio - Maximun) < EPSINON ) // ��T�I�Ƭ۵�
                ;
            else if( tempRatio > Maximun){
                Maximun = tempRatio;
                index = i ;
            } // else if

        } // for
    } // else

    return index ;
} // HRRN

float HRRN::calResponseRation(processInfo tempInfo, int clock){ // �p������t�v
    return( float((clock - tempInfo.arrivalTime + tempInfo.cpuBurst)) / float(tempInfo.cpuBurst));
} // HRRN

void PPRR::scheduling(vector<processInfo> allProcessInfo, int timeSlice) {
    processInfo *currentProcess, lastProcess ; // �Ȧs running ���� process
    int  processCount = 0, index = 0; // index ��ܥثe allProcessInfo�� �ĴX�� process
    int clock = -1, minimunIndex = 0, lastIndex = 0; // priority �p ��ܥ��B�z
    bool continueReamin = false; // process timeOut�F  �٦��F��n�B�z
    int lastPriority = 0; // �����W�@���� priority
    while(1) {
        // �P�@�ɶ� ���s�i��process enter queue then �� timeout ��process �i�Jqueue
        // preemptive
        if(!this->readyArray.empty()){ // ready array �����F��
            minimunIndex = this->findMinPriority(readyArray); // ��priority �̰���
            
            currentProcess = &readyArray.at(minimunIndex) ; // �� pointer ���V�Ѿl�ɶ��̤p�� process
            currentProcess->exeTime++ ;
            currentProcess->eachTimeSlice++ ; // �C�� time out ���n���s�p��
                // ���L�]�F1��
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // ø�s�̯S�� �NpID �����N�X

            if( !continueReamin &&  readyArray.at(minimunIndex).priority < lastPriority ) { // �p�Q(���u����)���� �h�N�L���W�C�̫᭱
                this->readyArray.push_back(readyArray.at(lastIndex)); // �ҫ��V������ ��i array �᭱
                this->readyArray.erase(this->readyArray.begin() + lastIndex) ; // �R���ثe��

                if( lastIndex < minimunIndex) // �p�G�e�����Q����g(�R��) �h�N index -1
                    minimunIndex--;
                
            } // if
            
            lastProcess = readyArray.at(minimunIndex); // �����W�@�����檺 process

            continueReamin = false ;
            if( readyArray.at(minimunIndex).exeTime == readyArray.at(minimunIndex).cpuBurst){ // �� process �w�g���槹�F
                timeInfo times;
                times.pID = readyArray.at(minimunIndex).pID;
                times.turnaroundTime = clock - readyArray.at(minimunIndex).arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - readyArray.at(minimunIndex).cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin() + minimunIndex) ; // �R���ثe�� // �n���᭱�R�� �]��currentProcess ���V���F�� �n���p�⧹�~�i�⥻��R��
                // �p�� waiting time turnaround time
            } //if

            else if ( readyArray.at(minimunIndex).eachTimeSlice % timeSlice == 0){ // timeOut �R�����b���檺 ���array �᭱
                readyArray.at(minimunIndex).eachTimeSlice = 0;
                continueReamin = true ; // �����s process�@�i queue �A�B�z����
            } // else if
            
        } // if
        else  { // ready queue ���S���F��
            if( clock >= 0) {
                this->ganttList.push_back('-') ; // ���ɨS�� process ,�H '-' �N��
            } // if
        } // else 
        
        // ����ɶ������� �h�줤��!!!!!!!!!!!!!!!!
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
        //if( clock == 100 )
            break ;
        else 
            clock++ ; // �ɶ��[�@

        for( index; index < allProcessInfo.size() ; index ++ ){ // ���ɦ��� �N process ��i ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // �ثe�ɶ����F���F
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if 
            else 
                break ;
        } // while
        
        if( continueReamin){ // ����B�z���� 
            this->readyArray.push_back(this->readyArray.at(minimunIndex)); // �ҫ��V������ ��i array �᭱
            this->readyArray.erase(this->readyArray.begin() + minimunIndex) ; // �R���ثe��
        } // if

        lastPriority = lastProcess.priority; // �W�@����priority
        lastIndex = minimunIndex;

    } // while()

} // PPRR

int PPRR::findMinPriority(vector<processInfo> remainProcess) {
    int index = 0, minimize = 0;
    if(remainProcess.size() == 1){ //  array ���u���@�ӪF�� �����^�ǲĤ@�Ӫ�index
        return 0 ;
    } // if
    else {
        minimize = remainProcess.at(0).priority ; // �Ĥ@�� process ���Ѿl�ɶ�
        for( int i = 1 ; i < remainProcess.size() ; i++) {
            if( remainProcess.at(i).priority < minimize){
                minimize = remainProcess.at(i).priority;
                index = i ;
            } // if

        } // for
    } // else

    return index ;

} // pprr
