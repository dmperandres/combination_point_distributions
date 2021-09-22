//LIC

#ifndef MAP_LIST_H
#define MAP_LIST_H

#include <opencv.hpp>
#include <string>
#include <stdexcept>
#include <iostream>
#include <QMessageBox>

//HEA
// aux class for doing search by name or position

template <class Type>
class _map_list
{
public:

  int size(){return Data_by_string.size();}
  void clear();

  void set(std::string Name,Type Data1);
  Type get_data(std::string Name);
  Type get_data(int Position);
  int get_position(std::string Name);
  std::string get_string(int Position);

  void show();
  
  std::map<std::string,Type> Data_by_string;
  std::map<int,Type> Data_by_position;
  std::map<std::string,int> Key_by_string;
  std::map<int, std::string> Key_by_position;
};

//HEA

template <class Type>
void _map_list<Type>::clear()
{
  Data_by_string.clear();
  Data_by_position.clear();
  Key_by_string.clear();
  Key_by_position.clear();
}

//HEA

template <class Type>
void _map_list<Type>::set(std::string Name,Type Data1)
{
  try{
    Data_by_string.at(Name);
  }
  catch (const std::out_of_range) {

    int Position=Data_by_string.size();

    Data_by_string[Name]=Data1;
    Data_by_position[Position]=Data1;
    Key_by_string[Name]=Position;
    Key_by_position[Position]=Name;
    return;
  }

  std::cout << "name " << Name << std::endl;

  show();

  QString Error("Error: the name is duplicated:  ");
  Error+=Name.c_str();
  QMessageBox MsgBox;
  MsgBox.setText(Error);
  MsgBox.exec();
}


template <class Type>
void _map_list<Type>::show()
{
  for (auto it=Data_by_string.begin(); it!=Data_by_string.end(); ++it)
      std::cout << it->first << std::endl;
}
//HEA

template <class Type>
Type _map_list<Type>::get_data(std::string Name)
{
  try{
    Data_by_string.at(Name);
  }
  catch (const std::out_of_range) {
    QString Error("Error in the definition of filter with this text:  ");
    Error+=Name.c_str();
    QMessageBox MsgBox;
    MsgBox.setText(Error);
    MsgBox.exec();
    return (Type(nullptr));
  }
  return Data_by_string.at(Name);
}

//HEA

template <class Type>
Type _map_list<Type>::get_data(int Position)
{
  try{
    Data_by_position.at(Position);
  }
  catch (const std::out_of_range) {
    QString Error("Error: the position does not exist:  ");
    QMessageBox MsgBox;
    MsgBox.setText(Error);
    MsgBox.exec();
    return (Type(nullptr));
  }
  return Data_by_position.at(Position);
}

//HEA

template <class Type>
int _map_list< Type>::get_position(std::string Name)
{
  try{
    Key_by_string.at(Name);
  }
  catch (const std::out_of_range) {
    QString Error("Error: the name does not exist:  ");
    Error+=Name.c_str();
    QMessageBox MsgBox;
    MsgBox.setText(Error);
    MsgBox.exec();
    return (-1);
  }
  return Key_by_string.at(Name);
}

//HEA

template <class Type>
std::string _map_list< Type>::get_string(int Position)
{
  try{
    Key_by_position.at(Position);
  }
  catch (const std::out_of_range) {
    QString Error("Error: the position does not exist");
    QMessageBox MsgBox;
    MsgBox.setText(Error);
    MsgBox.exec();
    return ("");
  }
  return Key_by_position.at(Position);
}

#endif
