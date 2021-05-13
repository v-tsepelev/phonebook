/* This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details. */

// Author: Vladimir Tsepelev. First version: September 2016, edited: May 2021.

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include "phonebook.hpp"

void abonent::edit_name()
  {
    std::cout << "First name: ";
    std::cin >> name;
  }

void abonent::edit_surname()
  {
    std::cout << "Last name: ";
    std::cin >> surname;
  }

void abonent::edit_phone1()
  {
    std::cout << "Primary phone number: ";
    std::cin >> phone1;
  }

void abonent::edit_phone2()
  {
    std::cout << "Alternative phone number: ";
    std::cin >> phone2;
  }

void abonent::edit_email1()
  {
    std::cout << "Primary e-mail: ";
    std::cin >> email1;
  }

void abonent::edit_email2()
  {
    std::cout << "Alternative e-mail: ";
    std::cin >> email2;
  }

// Данные контактов хранятся в скрытом текстовом файле .contacts.txt в директории с бинарником программы.

void abonent::save_to_file(std::string surname, std::string name, std::string phone1, std::string phone2, std::string email1, std::string email2)
  {
    std::ofstream file(".contacts.txt", std::ios_base::app);
    file << surname << "\n" << name << "\n" << phone1 << "\n" << phone2 << "\n" << email1 << "\n" << email2 << "\n";
    file.close();
  }

void abonent::create_contact()
  {
    char dialog1, dialog2;
    edit_name();
    edit_surname();
    edit_phone1();
    repeat1: std::cout << "Do you want to provide additional phone number? [y/n]" << "\n";
    std::cin >> dialog1;
    if (dialog1 == 'y')
      {
        edit_phone2();
      }
    else if (dialog1 == 'n')
      {
	phone2 = "none";
      }
    else
      {
	std::cout << "Enter y or n." << "\n";
	goto repeat1;
      }
    edit_email1();
    repeat2: std::cout << "Do you want to provide additional e-mail? [y/n]" << "\n";
    std::cin >> dialog2;
    if (dialog2 == 'y')
      {
        edit_email2();
      }
    else if (dialog2 == 'n')
      {
	email2 = "none";
      }
    else
      {
	std::cout << "Enter y or n." << "\n";
	goto repeat2;
      }    
    save_to_file(surname, name, phone1, phone2, email1, email2);
  }

/*
Каждый контакт записывается в файл следующим образом:
фамилия
имя
основной телефонный номер
дополнительный телефонный номер (или пометка "отсутствует")
основной адрес электронной почты
дополнительный адрес электронной почты (или пометка "отсутствует")

Чтобы просмотреть всю указанную информацию, ищем в файле строку с фамилией, после чего выводим её и пять последующих строк.
*/

void abonent::view_info()
  {
    std::string search_for, current_string, line1, line2, line3, line4, line5;
    std::cout << "Enter last name to search for: ";
    std::cin >> search_for;
    std::ifstream file(".contacts.txt", std::ios_base::in);
    while (std::getline(file, current_string))
      {
	;
	if (search_for.compare(current_string) == 0)
	  {
	    std::getline(file, line1);
	    std::getline(file, line2);
	    std::getline(file, line3);
	    std::getline(file, line4);
	    std::getline(file, line5);
	    std::cout << current_string << "\n" << line1 << "\n" << line2 << "\n" << line3 << "\n" << line4 << "\n" << line5 << "\n";
	  }
      }
    file.close();
  }

// Каждая шестая строка файла содержит фамилию (смотри комментарий перед функцией view_info). Записываем в вектор конкатенцию строк с именем и фамилией для всех контактов, после чего упорядочиваем элементы вектора в алфавитном порядке по фамилиям и печатаем на экран. 

void abonent::list_contacts()
  {
    std::ifstream file(".contacts.txt", std::ios_base::in);
    std::vector<std::string> vector;
    std::string current_string, line1;
    int i = 0;
    while (std::getline(file, current_string))
      {
        list: ++i;
	if (i % 6 == 1)
	  {
	    std::getline(file, line1);
	    vector.push_back(current_string + " " + line1);
	    goto list;
	  }	
      }
    file.close();
    std::sort(vector.begin(), vector.end()); // сортируем фамилии контактов в алфавитном порядке
    std::copy(vector.begin(), vector.end(), std::ostream_iterator<std::string>(std::cout, "\n")); // печатаем содержимое вектора на экран
  }

// Для удаления контакта создаём новый файл, в которой переписываем всё содержимое старого, кроме данных удаляемого контакта, после чего удаляем старый файл и переименовываем новый.

void abonent::remove_contact()
  {
    std::string search_for, current_string;
    std::cout << "Enter last name of the contact to be deleted: ";
    std::cin >> search_for;    
    std::ifstream old_file(".contacts.txt", std::ios_base::in);
    std::ofstream new_file(".contacts.txt~", std::ios_base::out);    
    while (std::getline(old_file, current_string))
      {
	if (search_for.compare(current_string) == 0)
	  {
	    for (int i{0}; i < 5; ++i)
	      {
		std::getline(old_file, current_string);
	      }
	  }
	new_file << current_string << "\n";	
      }    
    old_file.close();
    new_file.close();
    remove(".contacts.txt");
    rename(".contacts.txt~", ".contacts.txt");
  }

// Принцип работы как и у функции remove_contact.

void abonent::edit_contact()
  {
    std::string search_for, current_string, field, data, line1("1"), line2("2"), line3("3"), line4("4"), line5("5"), line6("6");
    std::cout << "Enter last name of the contact to be edited: ";
    std::cin >> search_for;
    std::cout << "Which field should be edited?\n";
    std::cout << "Enter 'last_name' to edit Last name\n";
    std::cout << "Enter 'first_name' to edit First name\n'";
    std::cout << "Enter 'phone1' to edit primary phone number\n";
    std::cout << "Enter 'phone2' to edit second (optional) phone number\n";
    std::cout << "Enter 'email1' to edit primary e-mail\n";
    std::cout << "Enter 'email2' to edit second (optional) e-mail\n";
    std::cout << "Enter command: ";
    std::cin >> field;
    do
      {
	std::cout << "Invalid field! Try again: ";
	std::cin >> field;	
      }
    while ((field != "last_name") && (field != "first_name") && (field != "phone1") && (field != "phone2") && (field != "email1") && (field != "email2"));
    std::ifstream old_file(".contacts.txt", std::ios_base::in);
    std::ofstream new_file(".contacts.txt~", std::ios_base::out);    
    if (field == "last_name")
      {
	std::cout << "Enter new last name: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << data << "\n";
		std::getline(old_file, current_string);
	      }
	    new_file << current_string << "\n";
	  }	
      }
      else if (field == "first_name")
      {
	std::cout << "Enter new first name: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		for (int i{0}; i < 2; ++i)
		  {
		    new_file << current_string << "\n";
		    std::getline(old_file, current_string);
		  }
	      }
	    new_file << current_string << "\n";
	  }
      }
    else if (field == "phone1")
      {
	std::cout << "Enter new phone number: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		for (int i{0}; i < 3; ++i)
		  {
		    new_file << current_string << "\n";
		    std::getline(old_file, current_string);
		  }	
	      }
	    new_file << current_string << "\n";
	  }
      }
    else if (field == "phone2")
      {
	std::cout << "Enter new phone number: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		for (int i{0}; i < 4; ++i)
		  {
		    new_file << current_string << "\n";
		    std::getline(old_file, current_string);
		  }
	      }
	    new_file << current_string << "\n";
	  }
      }
    else if (field == "email1")
      {
	std::cout << "Enter new e-mail: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		for (int i{0}; i < 5; ++i)
		  {
		    new_file << current_string << "\n";
		    std::getline(old_file, current_string);
		  }
	      }
	    new_file << current_string << "\n";
	  }
      }
    else if (field == "email2")
      {
	std::cout << "Enter new e-mail: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		for (int i{0}; i < 6; ++i)
		  {
		    new_file << current_string << "\n";
		    std::getline(old_file, current_string);
		  }	
	      }
	    if (old_file.eof() == true)
	      ;
	    else
	      new_file << current_string << "\n";
	  }
      }
    old_file.close();
    new_file.close();
    remove(".contacts.txt");
    rename(".contacts.txt~", ".contacts.txt");    
  }

void info()
  {
    std::cout << "===========\nHELP MANUAL\n===========\n\nquit -- terminate the program\ncreate -- create new entry\nview -- view entry\nlist -- list all entries\nremove -- remove entry\nedit -- edit entry\ninfo -- view entry\nabout -- about phonebook\n";
  }

void about()
  {
    std::cout << "=========================================================\nThis is a plain console phone book for GNU/Linux systems.\n                Author: Vladimir Tsepelev\n=========================================================" << "\n";
  }

// Дружественная используемому классу функция, которая ожидает пользовательский ввод, обрабатывает команду и вызывает соответствующий метод класса или другую функцию.

void phonebook_program(abonent& object)
  {
    std::string user_command, command1("quit"), command2("create"), command3("view"), command4("list"), command5("remove"), command6("edit"), command7("info"), command8("about");
    std::cout << "Enter 'info' for help or 'quit' to exit the program." << "\n";
    std::cout << "Enter command: ";
    std::cin >> user_command;
    while (user_command != "quit")
      {
	if (user_command == "create")
	  {
	    object.create_contact();     
	  }
	else if (user_command == "view")
	  {
	    object.view_info();
	  }
	else if (user_command == "list")
	  {
	    object.list_contacts();
	  }
	else if (user_command == "remove")
	  {
	    object.remove_contact();
	  }
	else if (user_command == "edit")
	  {
	    object.edit_contact();
	  }
	else if (user_command == "info")
	  {
	    info();
	  }
	else if (user_command == "about")
	  {
	    about();
	  }
	else
	  {
	    std::cout << "Invalid command!" << "\n";
	  }
	std::cout << "Enter command: ";
	std::cin >> user_command;
      }
  }
