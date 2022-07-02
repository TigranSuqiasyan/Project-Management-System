#ifndef PROJECT_MANAGEMENT_SYSTEM_H
#define PROJECT_MANAGEMENT_SYSTEM_H

#include <string>
#include <vector>

enum State{
    TODO, IN_PROGRESS, IN_REVIEW, DONE
};

class Date{
public:
    Date();
    Date(int, int, int);
    Date(std::string);
    void set_day(int);
    int get_day() const;
    void set_month(int);
    int get_month() const;
    void set_year(int);
    int get_year() const;
    std::string to_string() const;
    int to_int() const;
private:
    int _day;
    int _month;
    int _year;
};

class Task{
public:
    Task();
    explicit Task(const std::string&);
    void set_description(const std::string&);
    std::string get_description() const;
    void set_assignee(const std::string&);
    std::string get_assignee() const;
    void set_deadline(Date);
    Date get_deadline() const;
    void set_state(State);
    State get_state() const;
    std::string to_string() const;
private:
    std::string _description;
    std::string _assignee;
    Date _deadline;
    State _state;
};

class Project{
public:
    Project();
    Project(const std::vector<std::string>&);
    void set_title(const std::string&);
    std::string get_title() const;
    std::vector<Task>& get_tasks();
    void add_task(const Task&);
    void remove_task(unsigned);
    void sort_tasks();
    std::vector<std::string> to_strings() const;
private:
    std::string _title;
    std::vector<Task> _tasks;
};

//std::vector<std::string> lines;
//std::vector<Project> projects;

class Manager{
public:
    Manager(const std::string&);
    void from_file_to_lines() const;
    void from_lines_to_projects() const;
    void display() const;
    void edit() const;
    void from_projects_to_lines() const;
    void from_lines_to_file() const;
    void add_project() const;
    void edit_project(int) const;
    void delete_project(int) const;
    void add_task(int) const;
    void edit_task(int, int) const;
    void delete_task(int, int) const;
private:
    bool project_exists(std::string) const;
private:
    std::string _filename;
};

#endif // PROJECT_MANAGEMENT_SYSTEM