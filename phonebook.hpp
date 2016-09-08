/* This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details. */

// Author: Vladimir Tsepelev, tsepelev at openmailbox.org. September 2016.

#ifndef PHONEBOOK_H
#define PHONEBOOK_H

class abonent
{
 private:
  std::string name, surname, phone1, phone2, email1, email2;
  friend void phonebook_program(abonent&); // функция, осуществляющая интерактивное взаимодействие пользователя с программой (ожидание ввода и распознавание команд)

 public:
  void edit_name(); // ввод имени контакта
  void edit_surname(); // ввод фамилии контакта
  void edit_phone1(); // ввод основного телефонного номера
  void edit_phone2(); // ввод второго телефонного номера (при необходимости)
  void edit_email1(); // ввод адреса электронной почты
  void edit_email2(); // ввод дополнительного адреса электронной почты (при необходимости)
  void save_to_file(std::string surname, std::string name, std::string phone1, std::string phone2, std::string email1, std::string email2); // данные контактов хранятся в скрытом текстовом файле .contacts.txt в директории с бинарником программы
  void create_contact(); // создание нового контакта
  void view_info(); // просмотр информации о выбранном контакте
  void list_contacts(); // просмотр всех доступных контактов
  void remove_contact(); // удаление выбранного контакта
  void edit_contact(); // редактирование информации о выбранном контакте
};

#endif
