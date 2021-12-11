//#include <bits/stdc++.h> // Зачем? в первый раз вижу такое
// Все делают, как показано ниже
#include <iostream>	//Это то что вам нужно (cout & cin)
#include <cstdlib>	//Это system
#include <string>

using namespace std; // Так делать можно тока cpp файлах (И то есть люди, которые на это будут бузить(В main.cpp обычно всем прощают))


// Что с отступами? Используйте tab!


// Я бы сделал enum
enum arg_options {open_file, edit_file, create_file, delete_file, exit_option, options_count};

void printmenu () {
 	system("cls");
 	cout << "What do you want to do?" << endl;
 	// Есть такие чтуки как макросы позволяющие не дублировать код
 	// Ниже пример
#define PRINT_OPTION(opt, str) \
 cout << to_string(opt) << str << endl;
 	// Таким образом если вы поменяете порядок в arg_options, то вам не придется переписывать этот участок кода
 	PRINT_OPTION(open_file, ". Open file")
 	PRINT_OPTION(edit_file, ". Edit file")
 	PRINT_OPTION(create_file, ". Create file")
 	PRINT_OPTION(delete_file, ". Delete file")
 	PRINT_OPTION(exit_option, ". Exit")
}

arg_options inputcheck () { // cnt больше не нужен
	int var = -1; // надо бы задать значение перед тем как сравнивать (некоторые компиляторы ругались бы и вообще плохая практика)
	// -1 часто используется как невалидное значение 
 	cin >> var; // Почему бы не получить значение здесь?
	while (!(0 <= var && var < options_count)) { // Гораздо красивее и понятнее ограничивать отрезок без бесконечных концов
    		cout << "Error, try again" << endl; // Бац и if исчез
    		cin >> var;
  	}
 	return static_cast<arg_options>(var); // Явно скажем комплятору, что мы так и хотели (иначе он будет сыпать варнинги)
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
	arg_options var;
	do {
    		printmenu();
    		var = inputcheck();
    		switch(var) {
    		case open_file:
        		openfile();
        		break;
    		case edit_file:
        		writefile();
        		break;
    		case create_file:
        		makefile();
        		break;
    		case delete_file:
        		rmfile();
        		break;
		default:
			break;
    		}
	} while (var != exit_option);
       	system("pause"); // Почему бы не вынести сюда?

	return 0;
} 
