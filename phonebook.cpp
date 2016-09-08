/* This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details. */

// Author: Vladimir Tsepelev, tsepelev at openmailbox.org. September 2016.

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
    std::cout << "Имя: ";
    std::cin >> name;
  }

void abonent::edit_surname()
  {
    std::cout << "Фамилия: ";
    std::cin >> surname;
  }

void abonent::edit_phone1()
  {
    std::cout << "Основной номер телефона: ";
    std::cin >> phone1;
  }

void abonent::edit_phone2()
  {
    std::cout << "Дополнительный номер телефона: ";
    std::cin >> phone2;
  }

void abonent::edit_email1()
  {
    std::cout << "Основной адрес электронной почты: ";
    std::cin >> email1;
  }

void abonent::edit_email2()
  {
    std::cout << "Дополнительный адрес электронной почты: ";
    std::cin >> email2;
  }

// Данные контактов хранятся в скрытом текстовом файле .contacts.txt в директории с бинарником программы.

void abonent::save_to_file(std::string surname, std::string name, std::string phone1, std::string phone2, std::string email1, std::string email2)
  {
    std::ofstream file(".contacts.txt", std::ios_base::app);
    file << surname << std::endl << name << std::endl << phone1 << std::endl << phone2 << std::endl << email1 << std::endl << email2 << std::endl;
    file.close();
  }

void abonent::create_contact()
  {
    char dialog1, dialog2;
    edit_name();
    edit_surname();
    edit_phone1();
    repeat1: std::cout << "Указать дополнительный номер? [y/n]" << std::endl;
    std::cin >> dialog1;
    if (dialog1 == 'y')
      {
        edit_phone2();
      }
    else if (dialog1 == 'n')
      {
	phone2 = "отсутствует";
      }
    else
      {
	std::cout << "Введите y или n." << std::endl;
	goto repeat1;
      }
    edit_email1();
    repeat2: std::cout << "Указать дополнительный email? [y/n]" << std::endl;
    std::cin >> dialog2;
    if (dialog2 == 'y')
      {
        edit_email2();
      }
    else if (dialog2 == 'n')
      {
	email2 = "отсутствует";
      }
    else
      {
	std::cout << "Введите y или n." << std::endl;
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
    std::cout << "Введите фамилию для поиска: ";
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
	    std::cout << current_string << std::endl << line1 << std::endl << line2 << std::endl << line3 << std::endl << line4 << std::endl << line5 << std::endl;
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
    std::cout << "Введите фамилию удаляемого контакта: ";
    std::cin >> search_for;    
    std::ifstream old_file(".contacts.txt", std::ios_base::in);
    std::ofstream new_file(".contacts.txt~", std::ios_base::out);    
    while (std::getline(old_file, current_string))
      {
	if (search_for.compare(current_string) == 0)
	  {
	    std::getline(old_file, current_string);
	    std::getline(old_file, current_string);
	    std::getline(old_file, current_string);
	    std::getline(old_file, current_string);
	    std::getline(old_file, current_string);	    
	  }
	new_file << current_string << std::endl;	
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
    std::cout << "Введите фамилию редактируемого контакта: ";
    std::cin >> search_for;
    field: std::cout << "Какое поле следует редактировать? ";
    std::cin >> field;
    std::ifstream old_file(".contacts.txt", std::ios_base::in);
    std::ofstream new_file(".contacts.txt~", std::ios_base::out);    
    if (field.compare(line1) == 0)
      {
	std::cout << "Укажите новую фамилию: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << data << std::endl;
		std::getline(old_file, current_string);
	      }
	    new_file << current_string << std::endl;
	  }	
      }
      else if (field.compare(line2) == 0)
      {
	std::cout << "Укажите новое имя: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << data << std::endl;
		std::getline(old_file, current_string);
	      }
	    new_file << current_string << std::endl;
	  }
      }
    else if (field.compare(line3) == 0)
      {
	std::cout << "Укажите новый номер: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);	
		new_file << data << std::endl;
		std::getline(old_file, current_string);		
	      }
	    new_file << current_string << std::endl;
	  }
      }
    else if (field.compare(line4) == 0)
      {
	std::cout << "Укажите новый номер: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << data << std::endl;
		std::getline(old_file, current_string);
	      }
	    new_file << current_string << std::endl;
	  }
      }
    else if (field.compare(line5) == 0)
      {
	std::cout << "Укажите новый адрес: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << data << std::endl;
		std::getline(old_file, current_string);
	      }
	    new_file << current_string << std::endl;
	  }
      }
    else if (field.compare(line6) == 0)
      {
	std::cout << "Укажите новый адрес: ";
	std::cin >> data;
	while(std::getline(old_file, current_string))
	  {
	    if (search_for.compare(current_string) == 0)
	      {
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);		
		new_file << current_string << std::endl;
		std::getline(old_file, current_string);
		new_file << data << std::endl;
		std::getline(old_file, current_string);		
	      }
	    if (old_file.eof() == true)
	      ;
	    else
	      new_file << current_string << std::endl;
	  }
      }
    else
      {
	std::cout << "Доступные для редактирования поля имеют номер от 1 до 6." << std::endl;
	goto field;
      }
    old_file.close();
    new_file.close();
    remove(".contacts.txt");
    rename(".contacts.txt~", ".contacts.txt");    
  }

void info()
  {
    std::cout << "===================\nСПРАВКА ПО КОМАНДАМ\n===================\n\nquit -- покинуть программу\ncreate -- создать новый контакт\nview -- просмотреть подробную информацию о контакте\nlist -- показать список всех контактов\nremove -- удалить контакт\nedit -- редактировать контакт\ninfo -- справка по командам\nabout -- информация о программе\n\nПри редактировании данных контакта (edit) поле 1 соответствует фамилии, поле 2 -- имени, 3 -- номеру телефона, 4 -- дополнительному номеру телефона (если есть), 5 -- адресу электронной почты, 6 -- дополнительному адресу электронной почты (если есть)." << std::endl;
  }

void about()
  {
    std::cout << "=======================================================\nThis is plain console phone book for GNU/Linux systems.\nAuthor: Vladimir Tsepelev (tsepelev at openmailbox.org)\n=======================================================" << std::endl;
  }

// Дружественная используемому классу функция, которая ожидает пользовательский ввод, обрабатывает команду и вызывает соответствующий метод класса или другую функцию.

void phonebook_program(abonent& object)
  {
    std::string user_command, command1("quit"), command2("create"), command3("view"), command4("list"), command5("remove"), command6("edit"), command7("info"), command8("about");
    std::cout << "Если Вы пользуетесь программой впервые, то введите info для получения справки по командам или quit для выхода." << std::endl;
    beginning: std::cout << "Ввод команды: ";
    std::cin >> user_command;
    if (user_command.compare(command1) == 0)
      {
        goto ending;
      }
    else if (user_command.compare(command2) == 0)
      {
        object.create_contact();
        goto beginning;      
      }
    else if (user_command.compare(command3) == 0)
      {
        object.view_info();
        goto beginning;
      }
    else if (user_command.compare(command4) == 0)
      {
        object.list_contacts();
        goto beginning;
      }
    else if (user_command.compare(command5) == 0)
      {
        object.remove_contact();
        goto beginning;
      }
    else if (user_command.compare(command6) == 0)
      {
        object.edit_contact();
        goto beginning;
      }
    else if (user_command.compare(command7) == 0)
      {
	info();
	goto beginning;
      }
    else if (user_command.compare(command8) == 0)
      {
	about();
	goto beginning;
      }
    else
      {
        std::cout << "Несуществующая команда!" << std::endl;
        goto beginning;
      }
    ending: ;
  }
