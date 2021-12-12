#include <bits/stdc++.h>
using namespace std;

enum options {open=1, write, make, del, close, cnt_opts};

void printmenu () {
 system("cls");
 cout << "What do you want to do?" << endl;
 cout << "1. Open file" << endl;
 cout << "2. Edit file" << endl;
 cout << "3. Create file" << endl;
 cout << "4. Delete file" << endl;
 cout << "5. Exit" << endl;
}

options inputcheck () {
 int var = -1;
 cin >> var;
 while (!(open <= var && var < cnt_opts)) { // Иначе не работало бы
    cout << "Error, try again" << endl;
    cin >> var;
 }
 return static_cast<options>(var);
}

int openfile(){
cout << "The file has been opened" << endl;
return 0;
}

int writefile(){
cout << "The file is ready for editing" << endl;
return 0;
}

int makefile() {
cout << "You've successfully created a file" << endl;
return 0;
}

int rmfile()  {
cout << "The file has been deleted" << endl;
return 0;
}

int main() {
int var;
do {
    printmenu();
    var=inputcheck();
    switch(var) {
    case open:
        openfile();
        break;
    case write:
        writefile();
        break;
    case make:
        makefile();
        break;
    case del:
        rmfile();
        break;
    default:
        break;
    }
    if (var != close)
        system("pause");
} while (var != close);


return 0;
} 
