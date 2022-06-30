#include "project_management_system.hpp"

#include <iostream>
#include <string>
#include <vector>

// ---------------------------------- Date

Date::Date() : _day{}, _month{}, _year{}{
}

Date::Date(int day, int month, int year)
    : _day{day}, _month{month}, _year{year}{
}

Date::Date(std::string date){
    _day = std::stoi(date.substr(0, 2));
    _month = std::stoi(date.substr(3, 2));
    _year = std::stoi(date.substr(6, 2));
}

void Date::set_day(int day){
    _day = day;
}

int Date::get_day() const{
    return _day;
}

void Date::set_month(int month){
    _month = month;
}

int Date::get_month() const{
    return _month;
}

void Date::set_year(int year){
    _year = year;
}

int Date::get_year() const{
    return _year;
}

std::string Date::to_string() const{
    std::string result;
    result += _day / 10;
    result += _day % 10;
    result += ".";
    result += _month / 10;
    result += _month % 10;
    result += _year / 1000;
    result += (_year / 100) % 10;
    result += (_year / 10) % 10;
    result += _year % 10;
    return result;
}

int Date::to_int() const{
    return _day + _month * 100 + _year * 10'000;
}

// ------------------------------ Task

Task::Task() : _state{TODO}{
}

Task::Task(const std::string& line){
    int size = line.size();
    int i[4]{}; // an array of seperaors' indexes
    for(int k{}, j{}; k < size; ++k){
        if(line[k] == '|'){
            i[j++] = k;
        }
    }
    _description = line.substr(i[0] + 2, i[1] - i[0] - 3);
    _assignee = line.substr(i[1] + 2, i[2] - i[1] - 3);
    _deadline = Date(line.substr(i[2] + 2, i[3] - i[2] - 3));
    std::string state_in_string;
    state_in_string = line.substr(i[3] + 2, size - i[3] - 1);
    if(state_in_string == "TODO")
        _state = TODO;
    else if(state_in_string == "IN_PROGRESS")
        _state = IN_PROGRESS;
    else if(state_in_string == "IN_REVIEW")
        _state = IN_REVIEW;
    else _state = DONE;
}

void Task::set_description(const std::string& desc){
    _description = desc;
}

std::string Task::get_description() const{
    return _description;
}

void Task::set_assignee(const std::string& as){
    _assignee = as;
}

std::string Task::get_assignee() const{
    return _assignee;
}

void Task::set_deadline(Date date){
    _deadline = date;
}

Date Task::get_deadline() const{
    return _deadline;
}

void Task::set_state(State state){
    _state = state;
}

State Task::get_state() const{
    return _state;
}

std::string Task::to_string() const{
    std::string result = _description;
    result += " | ";
    result += _assignee;
    result += " | ";
    result += _deadline.to_string();
    result += " | ";
    switch(_state){
        case 0: result += "TODO"; break;
        case 1: result += "IN_PROGRESS"; break;
        case 2: result += "IN_REVIEW"; break;
        case 3: result += "DONE"; break;
    }
    return result;
}

// ----------------------------- Project

Project::Project(){
}

Project::Project(const std::vector<std::string>& strings){
    if(strings[0].substr(0,8) != "project "){
        std::cout << "smth went wrong";
    } // this part should be cut in the end
    int strings_size = strings.size();
    int title_size = strings[0].size();
    _title = strings[0].substr(8, title_size - 8);
    for(int i{1}; i < strings_size; ++i){
        _tasks.push_back(Task(strings[i]));
    }
}

void Project::set_title(const std::string& title){
    _title = title;
}

std::string Project::get_title() const{
    return _title;
}

void Project::add_task(const Task& task){
    _tasks.push_back(task);
}

void Project::remove_task(unsigned index){
    _tasks.erase(_tasks.begin() + index);
}

void Project::sort_tasks(){
    int size = _tasks.size();
    for(int i{1}; i < size; ++i){
        Task tmp{};
        tmp = _tasks[i];
        int j = i - 1;
        while(j >= 0 && _tasks[j].get_deadline().to_int() > tmp.get_deadline().to_int()){
            _tasks[j + 1] = _tasks[j];
            --j;
        }
        _tasks[j + 1] = tmp;
    }
}