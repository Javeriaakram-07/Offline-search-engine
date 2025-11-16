#include<iostream>
#include"engine.h"
using namespace std;
int main(){
    //data storage
    string *filePaths=nullptr;
    int fileCount=0;
    string *dataLines=nullptr;
    int lineCount=0;
    string *searchHistory=nullptr;
    int historyCount=0;
    
    //start program
    showWelcome();
    //load files
    cout<<"Loading files..."<<endl;
    if(loadFileList("data/filelist.txt",filePaths,fileCount)){
        if(loadDataFiles(filePaths,fileCount,dataLines,lineCount)){
            cout<<"Success!Loaded"<<fileCount<<" files."<<endl;
            showFileSummary(filePaths,fileCount,lineCount);
        }
    }
    //Load search history
    readSearchHistory(searchHistory,historyCount);
    waitForEnter();
    //main program loop
    bool programRunning=true;
    
    while(programRunning){
        showWelcome();
        int userChoice=displayMainMenu();
        
        switch(userChoice){
            case 1:
                searchMenu(dataLines,lineCount);
                break;
            case 2:
                displayHistory(searchHistory,historyCount);
                break;  
            case 3:
                showFileSummary(filePaths,fileCount,lineCount);
                break; 
            case 4:
                programRunning=false;
                break;
            default:
                cout<<"Invalid choice.Please enter 1-4."<<endl;
                waitForEnter();
                break;
        }
    }
    
    //clean up memory
    clearData(dataLines,lineCount);
    delete[] filePaths;
    delete[] searchHistory;
    
    //goodbye message
    clearScreen();
    cout<<"=========================================="<<endl;
    cout<<"  Thank you for using our Search Engine!"<<endl;
    cout<<"              Goodbye!"<<endl;
    cout<<"=========================================="<<endl;
}