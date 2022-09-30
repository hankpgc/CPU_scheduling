// 
// OS_HW2
// 資訊三乙 10827234 彭桂綺
//

#include <iostream>
#include <string.h> // strcmp
#include <vector>
#include <fstream>	// open, is_open, close, write, ignore
#include <stdlib.h>  
#include <iomanip> // setw

using namespace std;

string inFileName = "";
const float EPSINON = 0.001; // 比較福點數是否相等用

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
                    'U', 'V', 'W', 'X', 'Y', 'Z'} ; // pID 對照表         // 30 - 35

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

    void Push(T data){ // 把資料放到最後端
        if ((back + 1) % capacity == front) // 如果每個位置都放滿了，把配置陣列大小變為2倍再放資料
        {
            DoubleCapacity();
        }
        if (isEmpty()) // 如果Queue為空，則Push時front要另外加1，讓front變為0
        {
            front++;
        }
        ptr[(back + 1) % capacity] = data;
        back = (back + 1) % capacity;

        
        //cout << "!!!!back: " << back << "\t!!!!front: " << front << endl ;
    } // push()

    void Pop(){ // 把頂端的資料移除
        if (isEmpty()){
            cout << "Queue is empty." << endl;
        } // if
        else if( getSize() == 1) { // 若刪除前只剩一個則刪除後為空
            front = -1 ;
            back = -1;
        }
        else{
            front = (front + 1) % capacity; // 只需要把front加1就行，因為之後如果有資料Push進來，會直接覆蓋掉原本的，不須花費成本再delete跟new
        } // else
    } // pop ()

    bool isEmpty() { // 回傳Queue是否為空
        return (front == -1);
    } // is Empty

    int getSize(){ // 回傳Queue大小
        if (isEmpty()){
            return 0;
        } // if
        else if (front == back){ // 如果front和back為同一個位置
            return 1;
        } // else if
        else if (front < back){ // 如果front在back前面
            return back - front + 1;
        }// else if
        else{ // 如果front在back後面
            return capacity - (front - back) + 1;
        } // else
    }

    T* getFront(){ // 回傳最前端的資料
        if (isEmpty()){
            cout << "Queue is empty." << endl;
            return nothing; // 回傳0是因為不知道T是什麼型態，如果回傳別的數值會很容易出錯
        } // if

        //cout << "front: " << front << endl ;
        return &ptr[front];
    } // getFront

    T* getBack() { // 回傳最後端的資料
        if (isEmpty()){
            cout << "Queue is empty." << endl;
            return nothing; // 回傳0是因為不知道T是什麼型態，如果回傳別的數值會很容易出錯
        } // if

        return &ptr[back];
    } // getBack

private:
    int front;            // 最前端的index
    int back;             // 最後端的index
    int capacity;         // 目前配置空間的大小
    T *ptr;               // 目前配置的陣列(第一個元素的指標)
    T *nothing;
    void DoubleCapacity(){ // 把配置的陣列變成兩倍的大小
        T *newptr = new T[capacity * 2];
        int j = front - 1;
        for (int i = 0; i < capacity; i++) // 把舊的陣列裡的資料複製到新的陣列
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
// 用 queue
public:
    QueueArray<pI> readyQueue;
    vector<tI> timeList;
    vector<char> ganttList;
    void initial() { 
        readyQueue.initial(); // queue 大小
        ganttList.clear();
        timeList.clear() ;
    };

    void scheduling(vector<processInfo> allProcessInfo);
} ;

class RR{ // methd 2
// 用 queue
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
// 不需要queue 用array 即可 readyArray裡面也不須排序  因為每次都是取最剩餘時間小的出來 不影響
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
    int findShortestProcess(vector<processInfo> remainProcess); // 回傳 對應的index
} ;

class PPRR{ // methd 4
// 不需要queue 用array 即可 readyArray裡面也不須排序  因為每次都是取最剩餘時間小的出來 不影響
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
    int findMinPriority(vector<processInfo> remainProcess) ; // 回傳 有最小priority 的process 的 index
} ;

class HRRN{ // methd 5
// 不需要queue 用array 即可 readyArray裡面也不須排序  因為每次都是取比率最小的出來 不影響
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
    int findHighestRatioProcess(vector<processInfo> remainProcess, int clock); // 回傳 最高反映速率對應的index
    float calResponseRation(processInfo tempInfo, int clock); // 計算反應速率
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

        wantExe = openFile(inputFile); // 輸入 "0" return false 否則持續讀入直到正確開啟檔案

        if ( wantExe ) { 
            inputFile >> method >> timeSlice ;// read records from a file
            
            getline(inputFile, stringTemp) ; // '\n'
            getline(inputFile, stringTemp) ; //將第二列讀掉

            // 儲存 process Info
            processInfo tempInfo;
            while(inputFile >> tempInfo.pID){
                inputFile  >> tempInfo.cpuBurst >> tempInfo.arrivalTime >> tempInfo.priority ;
                tempInfo.exeTime = 0 ;
                tempInfo.eachTimeSlice = 0 ;
                allProcessInfo.push_back(tempInfo);
            } // while
            
            NewBubbleSort( allProcessInfo, processID); // 以 pID 排序
            NewBubbleSort( allProcessInfo, arrivalTime); // 以 arrival time 排序
            // 先以 pID 排再以 arrival time 則可以確保queue 從最前面拿出的順序為 時間最小  如時間一樣則pID 小

            int time = 0 ; // time 模擬 cpuTime 每次回圈 + 一秒
            switch (method) {
            case 1 :
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 1: [FCFS]     ║" << endl;
                cout << "╚══════════════════════════╝" << endl;
                
                fcfsItem.scheduling(allProcessInfo);
                NewBubbleSort(fcfsItem.timeList, processID);
                methods.push_back("FCFS");
                allGantt.push_back(fcfsItem.ganttList);
                allTime.push_back(fcfsItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 2 :
                cout << "╔══════════════════════════╗" << endl;
                cout << "║      Method 2: [RR]      ║" << endl;
                cout << "╚══════════════════════════╝" << endl;
                
                rrItem.scheduling(allProcessInfo, timeSlice) ;
                NewBubbleSort(rrItem.timeList, processID);
                methods.push_back("RR");
                allGantt.push_back(rrItem.ganttList);
                allTime.push_back(rrItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 3:
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 3: [SRTF]     ║" << endl;
                cout << "╚══════════════════════════╝" << endl;

                srtfItem.scheduling(allProcessInfo) ;
                NewBubbleSort(srtfItem.timeList, processID);
                methods.push_back("SRTF");
                allGantt.push_back(srtfItem.ganttList);
                allTime.push_back(srtfItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 4:
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 4: [PPRR]     ║" << endl;
                cout << "╚══════════════════════════╝" << endl;

                pprrItem.scheduling(allProcessInfo, timeSlice) ;
                NewBubbleSort(pprrItem.timeList, processID);
                methods.push_back("PPRR");
                allGantt.push_back(pprrItem.ganttList);
                allTime.push_back(pprrItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;

            case 5:
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 5: [HRRN]     ║" << endl;
                cout << "╚══════════════════════════╝" << endl;

                hrrnItem.scheduling(allProcessInfo);
                NewBubbleSort(hrrnItem.timeList, processID);
                methods.push_back("HRRN");
                allGantt.push_back(hrrnItem.ganttList);
                allTime.push_back(hrrnItem.timeList);

                writeFile(allGantt, allTime, methods) ;
                cout << "\n## execution SUCCEESS ##" << endl ;
                break;
            
            case 6:
                cout << "╔══════════════════════════╗" << endl;
                cout << "║     Method 6: [ALL]      ║" << endl;
                cout << "╚══════════════════════════╝" << endl;
                
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

    } while (method != 0 && wantExe); // method != 0 & 想繼續執行

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
            temp << allGantt.at(i).at(x) ; // 輸出第 i 個甘特圖
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
    
    
    // 數值
    temp << "\n===========================================================" << endl;
    for( int i = 0 ; i < allTime.at(0).size() ; i++) { // 有幾個pId 就跑幾次
        
        temp << allTime.at(0).at(i).pID; // 先印 第 i 個 pID
        for( int j = 0 ; j < allTime.size() ; j++) { // 有幾個 method(s) 就跑幾次
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
    
    // 數值
    temp << "\n===========================================================" << endl;
    for( int i = 0 ; i < allTime.at(0).size() ; i++) { // 有幾個pId 就跑幾次
        
        temp << allTime.at(0).at(i).pID; // 先印 第 i 個 pID
        for( int j = 0 ; j < allTime.size() ; j++) { // 有幾個 method(s) 就跑幾次
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
    processInfo *currentProcess ; // 暫存 running 中的 process
    int  processCount = 0, index = 0; // index 表示目前 allProcessInfo中 第幾個 process
    int clock = 0;
    while(1) {
        for( index; index < allProcessInfo.size() ; index ++ ){ // 此時此刻 將 process 放進 ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // 目前時間有東西抵達
                this->readyQueue.Push(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        // non-preemptive
        if(!this->readyQueue.isEmpty()){ // reagy queue 中有東西
            currentProcess = this->readyQueue.getFront(); // 將最前面的 process 拿出來 
            currentProcess->exeTime++ ;
                // 給他跑了1秒
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // 繪製甘特圖 將pID 換成代碼

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // 此 process 已經執行玩了
                this->readyQueue.Pop() ; // 刪掉前面的
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;
        
                this->timeList.push_back(times);
                // 計算 waiting time turnaround time
                
            } //if
        } // if
        else {
            this->ganttList.push_back('-') ; // 此時沒有 process 以 '-' 代替
        } // else

        if( this->readyQueue.isEmpty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // 時間加一

    } // while()

    this->readyQueue.initial() ; // 必須 delete ptr 要不然做第二次會錯
} // FCFS

void RR::scheduling(vector<processInfo> allProcessInfo, int timeSlice) { // NEW
    processInfo *currentProcess = NULL ; // 暫存 running 中的 process
    int  processCount = 0, index = 0; // index 表示目前 allProcessInfo中 第幾個 process
    int clock = -1;
    bool continueReamin = false; // process timeOut了  還有東西要處理
    while(1) {
        // 同一時間 讓新進的process enter queue then 讓 timeout 的process 進入queue
        // preemptive
        currentProcess = NULL;
        if(!this->readyArray.empty()){ // ready queue 中有東西
            currentProcess = &this->readyArray.at(0); // 將最前面的 process 拿出來 
            currentProcess->exeTime++ ;
                // 給他跑了1秒
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // 繪製甘特圖 將pID 換成代碼

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // 此 process 已經執行完了
                
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin()) ; // 刪掉前面的
                // 計算 waiting time turnaround time
            } //if

            else if ((currentProcess->exeTime % timeSlice) == 0){ // timeOut 刪掉前面 丟到queue 後面
                continueReamin = true ; // 先讓新 process　進 queue 再處理後續
            } // else if
                
        } // if
        else  { // ready queue 中沒有東西
            this->ganttList.push_back('-') ; // 此時沒有 process ,以 '-' 代替?????????????????????????????????????
        } // else
        
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // 時間加一

        for( index; index < allProcessInfo.size() ; index ++ ){ // 此時此刻 將 process 放進 ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // 目前時間有東西抵達
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        if( continueReamin){ // 接續處理後續 
            this->readyArray.push_back(this->readyArray.front()); // 所指向的實體 丟進 queue 後面
            this->readyArray.erase(this->readyArray.begin()) ; // 刪掉前面的
            
            continueReamin = false ;
        } // if
        
    } // while()

    this->ganttList.erase(ganttList.begin()) ;
    //this->readyQueue.initial() ; // 必須 delete ptr 要不然做第二次會錯
} // RR

void SRTF::scheduling(vector<processInfo> allProcessInfo) {
    processInfo *currentProcess ; // 暫存 running 中的 process
    int  processCount = 0, index = 0; // index 表示目前 allProcessInfo中 第幾個 process
    int clock = 0, minimunIndex = 0;
    while(1) {
        
        for( index; index < allProcessInfo.size() ; index ++ ){ // 此時此刻 將 process 放進 ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // 目前時間有東西抵達
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        // preemptive
        if(!this->readyArray.empty()){ // ready queue 中有東西
            minimunIndex = this->findShortestProcess(readyArray); // 找出目前剩餘時間最小的process 的 index
            currentProcess = &readyArray.at(minimunIndex) ; // 使 pointer 指向剩餘時間最小的 process
            currentProcess->exeTime++ ;
                // 給他跑了1秒
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // 繪製甘特圖 將pID 換成代碼

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // 此 process 已經執行完了
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin() + minimunIndex) ; // 刪掉目前的 // 要放到後面刪除 因為currentProcess 指向此東西 要等計算完才可把本體刪掉
                // 計算 waiting time turnaround time
            } //if
                
        } // if
        else  { // ready queue 中沒有東西
            if( 1) {
                this->ganttList.push_back('-') ; // 此時沒有 process ,以 '-' 代替
            } // if
        } // else 
        
        //if( clock == 60)
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // 時間加一

    } // while()

} // SRTF

int SRTF::findShortestProcess(vector<processInfo> remainProcess){ // 回傳 對應的index
    int index = 0, minimize = 0;
    if(remainProcess.size() == 1){
        return 0 ;
    } // if
    else {
        minimize = remainProcess.at(0).cpuBurst - remainProcess.at(0).exeTime ; // 第一個 process 的剩餘時間
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
    processInfo *currentProcess ; // 暫存 running 中的 process
    int  processCount = 0, index = 0; // index 表示目前 allProcessInfo中 第幾個 process
    int clock = 0, MaximunIndex = 0;
    bool finished = true;
    while(1) {
        for( index; index < allProcessInfo.size() ; index ++ ){ // 此時此刻 將 process 放進 ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // 目前時間有東西抵達
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if
            else 
                break ;
        } // while

        // Non-preemptive
        if(!this->readyArray.empty() ){ // ready queue 中有東西
            if( finished){  // 因為是 nonpreemptive 前一個process 結束執行才繼續
                MaximunIndex = this->findHighestRatioProcess(readyArray, clock); // 找出目前剩餘時間最小的process 的 index
                finished = false ;
            } // if
            
            // 每次都要重新要求實體 因為 pushback 會改變記憶體位址
            currentProcess = &readyArray.at(MaximunIndex) ; // 使 pointer 指向剩餘時間最小的 process
            currentProcess->exeTime++ ;
                // 給他跑了1秒
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // 繪製甘特圖 將pID 換成代碼

            if(currentProcess->exeTime == currentProcess->cpuBurst){ // 此 process 已經執行完了
                finished = true ; // 執行完了 可以換下個process 
                timeInfo times;
                times.pID = currentProcess->pID;
                times.turnaroundTime = clock - currentProcess->arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - currentProcess->cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin() + MaximunIndex) ; // 刪掉目前的 // 要放到後面刪除 因為currentProcess 指向此東西 要等計算完才可把本體刪掉
                // 計算 waiting time turnaround time
            } //if
                
        } // if
        else  { // ready queue 中沒有東西
            if( clock >= 0) {
                this->ganttList.push_back('-') ; // 此時沒有 process ,以 '-' 代替
            } // if
        } // else 
        
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
            break ;
        else 
            clock++ ; // 時間加一

    } // while()

} // HRRN

int HRRN::findHighestRatioProcess(vector<processInfo> remainProcess, int clock){ // 回傳 對應的index
    int index = 0;
    float Maximun = 0.0, tempRatio = 0.0;
    if(remainProcess.size() == 1){
        return 0 ;
    } // if
    else {
        Maximun = calResponseRation(remainProcess.at(0), clock) ; // 第一個 process 的Response Ratio
        for( int i = 1 ; i < remainProcess.size() ; i++) {
            tempRatio = calResponseRation(remainProcess.at(i), clock);
            if( abs(tempRatio - Maximun) < EPSINON ) // 兩幅點數相等
                ;
            else if( tempRatio > Maximun){
                Maximun = tempRatio;
                index = i ;
            } // else if

        } // for
    } // else

    return index ;
} // HRRN

float HRRN::calResponseRation(processInfo tempInfo, int clock){ // 計算反應速率
    return( float((clock - tempInfo.arrivalTime + tempInfo.cpuBurst)) / float(tempInfo.cpuBurst));
} // HRRN

void PPRR::scheduling(vector<processInfo> allProcessInfo, int timeSlice) {
    processInfo *currentProcess, lastProcess ; // 暫存 running 中的 process
    int  processCount = 0, index = 0; // index 表示目前 allProcessInfo中 第幾個 process
    int clock = -1, minimunIndex = 0, lastIndex = 0; // priority 小 表示先處理
    bool continueReamin = false; // process timeOut了  還有東西要處理
    int lastPriority = 0; // 紀錄上一次的 priority
    while(1) {
        // 同一時間 讓新進的process enter queue then 讓 timeout 的process 進入queue
        // preemptive
        if(!this->readyArray.empty()){ // ready array 中有東西
            minimunIndex = this->findMinPriority(readyArray); // 拿priority 最高的
            
            currentProcess = &readyArray.at(minimunIndex) ; // 使 pointer 指向剩餘時間最小的 process
            currentProcess->exeTime++ ;
            currentProcess->eachTimeSlice++ ; // 每次 time out 都要重新計算
                // 給他跑了1秒
            this->ganttList.push_back( pIDList[ currentProcess->pID ]); // 繪製甘特圖 將pID 換成代碼

            if( !continueReamin &&  readyArray.at(minimunIndex).priority < lastPriority ) { // 如被(高優先的)插隊 則將他丟到柱列最後面
                this->readyArray.push_back(readyArray.at(lastIndex)); // 所指向的實體 丟進 array 後面
                this->readyArray.erase(this->readyArray.begin() + lastIndex) ; // 刪掉目前的

                if( lastIndex < minimunIndex) // 如果前面的被移動g(刪除) 則將 index -1
                    minimunIndex--;
                
            } // if
            
            lastProcess = readyArray.at(minimunIndex); // 紀錄上一次執行的 process

            continueReamin = false ;
            if( readyArray.at(minimunIndex).exeTime == readyArray.at(minimunIndex).cpuBurst){ // 此 process 已經執行完了
                timeInfo times;
                times.pID = readyArray.at(minimunIndex).pID;
                times.turnaroundTime = clock - readyArray.at(minimunIndex).arrivalTime + 1 ;
                times.waitingTime = times.turnaroundTime - readyArray.at(minimunIndex).cpuBurst;

                this->timeList.push_back(times);
                this->readyArray.erase(this->readyArray.begin() + minimunIndex) ; // 刪掉目前的 // 要放到後面刪除 因為currentProcess 指向此東西 要等計算完才可把本體刪掉
                // 計算 waiting time turnaround time
            } //if

            else if ( readyArray.at(minimunIndex).eachTimeSlice % timeSlice == 0){ // timeOut 刪掉正在執行的 丟到array 後面
                readyArray.at(minimunIndex).eachTimeSlice = 0;
                continueReamin = true ; // 先讓新 process　進 queue 再處理後續
            } // else if
            
        } // if
        else  { // ready queue 中沒有東西
            if( clock >= 0) {
                this->ganttList.push_back('-') ; // 此時沒有 process ,以 '-' 代替
            } // if
        } // else 
        
        // 控制時間的部分 搬到中間!!!!!!!!!!!!!!!!
        if( this->readyArray.empty() && (processCount == allProcessInfo.size()))
        //if( clock == 100 )
            break ;
        else 
            clock++ ; // 時間加一

        for( index; index < allProcessInfo.size() ; index ++ ){ // 此時此刻 將 process 放進 ready queue
            if( allProcessInfo.at(index).arrivalTime == clock){ // 目前時間有東西抵達
                this->readyArray.push_back(allProcessInfo.at(index));
                processCount ++ ;
            } // if 
            else 
                break ;
        } // while
        
        if( continueReamin){ // 接續處理後續 
            this->readyArray.push_back(this->readyArray.at(minimunIndex)); // 所指向的實體 丟進 array 後面
            this->readyArray.erase(this->readyArray.begin() + minimunIndex) ; // 刪掉目前的
        } // if

        lastPriority = lastProcess.priority; // 上一次的priority
        lastIndex = minimunIndex;

    } // while()

} // PPRR

int PPRR::findMinPriority(vector<processInfo> remainProcess) {
    int index = 0, minimize = 0;
    if(remainProcess.size() == 1){ //  array 中只有一個東西 直接回傳第一個的index
        return 0 ;
    } // if
    else {
        minimize = remainProcess.at(0).priority ; // 第一個 process 的剩餘時間
        for( int i = 1 ; i < remainProcess.size() ; i++) {
            if( remainProcess.at(i).priority < minimize){
                minimize = remainProcess.at(i).priority;
                index = i ;
            } // if

        } // for
    } // else

    return index ;

} // pprr
