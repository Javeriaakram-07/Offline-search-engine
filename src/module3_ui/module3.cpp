#include"engine.h"
#include<iostream>
#include<string>
using namespace std;

//clear screen
void clearScreen(){
    system("cls");

}
//wait for user to press enter
void waitForEnter(){
    cout<<"press enter to continue...";
    cin.ignore();
    cin.get();
}

//display welcome message
void showWelcome(){
    clearScreen();
    cout<<"=====================================\n";
    cout<<"       SIMPLE TEXT ENGINE            \n";
    cout<<"=====================================\n";
}

//show main menu and return user choice
int displayMainMenu(){
    int choice;
    cout<<"==========MAIN MENU==========\n";
    cout<<"what do you like to do?\n";
    cout<<"1.Search Text\n";
    cout<<"2.view Search history\n";
    cout<<"3.view file information\n";
    cout<<"4.exit program\n";
    cout<<endl;
    cin>>choice;
    if(choice<1 || choice>4)
        return -1;

       return choice;
}

// handle the search process
void searchMenu(string *dataLines,int lineCount){
    clearScreen();
    cout<<"====SEARCH===="<<endl;
    cout<<endl;
    if(dataLines==nullptr || lineCount==0){
        cout<<"No documents loaded.Please load files first."<<endl;
        waitForEnter();
        return;
        
    }
    cin.ignore();
    string keyword;
    cout<<"Enter word to search for:";
    getline(cin,keyword);
    keyword=toLowerCase(keyword);
    // check if word is valid
    if(keyword.empty()){
        cout<<"Search word cant be empty."<<endl;
        waitForEnter();
        return;
    }
    if(keyword.length()<2){
        cout<<"Please enter at least 2 characters."<<endl;
        waitForEnter();
        return;
    }
    cout<<"Searching for:"<<keyword<<endl;
    cout<<"Please wait..."<<endl;
    cout<<endl;

    string *results=nullptr;
    int resultCount=0;
    simpleSearch(dataLines,lineCount,keyword,results,resultCount);

    if(resultCount==0){
        partialMatchSearch(dataLines,lineCount,keyword,results,resultCount);
    }

    if(resultCount>0){
        displayResults(results,resultCount); // display results
        logSearch(keyword,resultCount);     // log search
        delete[]results;
    }
    else{
        displayNoResultMessage(keyword);
        logSearch(keyword,0);
    }

}

//display search results
void displayResults(string *results,int resultCount){
    cout<<"====SEARCH RESULTS===="<<endl;
    cout<<"Found"<<resultCount<<" matches:"<<endl;
    for(int i=0;i<resultCount;i++){
        cout<<"Result"<<(i+1)<<" :"<<results[i]<<endl;
    }
    waitForEnter();
}

// when no results found
void displayNoResultMessage(const string& keyword){
    cout<<"No Results Found for:"<< keyword<<endl;
    cout<<endl;
    cout<<"TIPS:"<<endl;
    cout<<"Check Spelling"<<endl;
    cout<<"Try different word"<<endl;
    cout<<"Try simple words"<<endl;
    cout<<endl;
    waitForEnter();
}

//show search history
void displayHistory(string *history,int historyCount){
    cout<<"==== SEARCH HISTORY===="<<endl;
    if(history==nullptr || historyCount==0){
        cout<<"No Search history yet."<<endl;
        cout<<"Your searches will appear here."<<endl;

    }else{
        cout<<"Your recent searches:"<<endl;
        cout<<"----------------------------"<<endl;
        for(int i=0;i<historyCount;i++){
            cout<<(i+1)<<"."<<history[i]<<endl;
        }
        cout<<"----------------------------"<<endl;
        cout<<"Total Searches:"<<historyCount<<endl;
    }
    cout<<endl;
    waitForEnter();
}