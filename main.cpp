#include <bits/stdc++.h>
using namespace std;

void printmenu () {
 system("cls");
 cout << "What do you want to do?" << endl;
 cout << "1. Open file" << endl;
 cout << "2. Edit file" << endl;
 cout << "3. Create file" << endl;
 cout << "4. Delete file" << endl;
 cout << "5. Exit" << endl;
}

int inputcheck (int cnt) {
 int var;
 while (var <1 || var > cnt) {
    cin >> var;
    if (var <1 || var > cnt)
        cout << "Error, try again" << endl;
 }
 return var;
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
    var=inputcheck(5);
    switch(var) {
    case 1:
        openfile();
        break;
    case 2:
        writefile();
        break;
    case 3:
        makefile();
        break;
    case 4:
        rmfile();
        break;
    }
    if (var != 5)
        system("pause");
} while (var != 5);

return 0;
}
