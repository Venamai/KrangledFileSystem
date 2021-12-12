#include <bits/stdc++.h>
#include "filesys.h"
using namespace std;

struct File * tempfile = NULL;
enum options {open=1, read, write, flist, del, close, mexit, cnt_opts};

void printmenu () {
 system("cls");
 cout << "KrangledFileSystem ver0.0.1 |" << endl << "----------------------------" << endl;
 cout << "What do you want to do?" << endl;
 cout << "1. Open file" << endl;
 cout << "2. Read file" << endl;
 cout << "3. Write file" << endl;
 cout << "4. List of files" << endl;
 cout << "5. Delete file" << endl;
 cout << "6. Close file" << endl;
 cout << "7. Exit" << endl;
 if (tempfile){
    cout << "File " << (char*)(tempfile->filename) << " is opened" << " in mode " << tempfile->mode << endl;
 }
}

options inputcheck () {
 char var = '0';
 cin >> var;

 while (!('0' < var && var < '8')) {
    cout << "Error, try again, var = " << var << endl;
    cin >> var;
 }
 return static_cast<options>((int)(var-'0'));
}

void filelist(){
printdir();
}

int writefile(){
    char buf[4096];
    cout << "Enter data" << endl;
    cin >> buf;
    if (filewrite((void*)buf, strlen(buf), tempfile) < 0){
        cout<< "error" << endl;
        return -1;
    }
    cout << "Written " << strlen(buf) << " bytes to file " << tempfile->filename << endl;
return 0;
}
void readfile(){
char buf[4096];
memset(buf, 0, 4096);
int ret;
while (tempfile->endof == 0) {
    ret = fileread((void*)buf,4096, tempfile);
    if (ret < 0){
        cout << "readerror=" << ret << endl;
        return;
    }
    cout << buf;
}
cout << endl;
}

int main() {
int var;
do {
    printmenu();
    var=inputcheck();
    char filename[512];
    switch(var) {
    case open:
        if (tempfile != NULL){
            cout << "Close the opened file"<< endl;
            break;
        }
        int mode;
        cout << "Enter file name" << endl;
        cin >> filename;
        cout << "Enter mode (1=r, 2=w)" << endl;
        cin >> mode;
        if ((tempfile = openfile((char*)filename, mode)) == NULL ) {
            cout << "File has not been found" << endl;
            break;
        }
        cout << "Opened successfully" << endl;
        break;
    case read:
        readfile();
        break;
    case write:
        writefile();
        break;
    case flist:
        filelist();
        break;
    case del:
        cout << "Enter file name" << endl;
        cin >> filename;
        rmfile(filename);
        cout << "File " << filename << " has been deleted" << endl;
        break;
    case close:
        closefile(tempfile);
        tempfile = NULL;
        cout << "File has been closed" << endl;
        break;
    }
    if (var != mexit)
        system("pause");
} while (var != mexit);

return 0;
}
