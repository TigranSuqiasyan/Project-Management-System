#include "project_management_system.hpp"
#include <iostream>
#include <fstream>

std::vector<std::string> lines;
std::vector<Project> projects;

// ---------------------------------- Date

Date::Date() : _day{}, _month{}, _year{}
{
}

Date::Date(int day, int month, int year)
    : _day{day}, _month{month}, _year{year}
{
}

Date::Date(std::string date)
{
    _day = std::stoi(date.substr(0, 2));
    _month = std::stoi(date.substr(3, 2));
    _year = std::stoi(date.substr(6, 4));
}

void Date::set_day(int day)
{
    _day = day;
}

int Date::get_day() const
{
    return _day;
}

void Date::set_month(int month)
{
    _month = month;
}

int Date::get_month() const
{
    return _month;
}

void Date::set_year(int year)
{
    _year = year;
}

int Date::get_year() const
{
    return _year;
}

std::string Date::to_string() const
{
    std::string result;
    result += _day / 10 + '0';
    result += _day % 10 + '0';
    result += ".";
    result += _month / 10 + '0';
    result += _month % 10 + '0';
    result += ".";
    result += _year / 1000 + '0';
    result += (_year / 100) % 10 + '0';
    result += (_year / 10) % 10 + '0';
    result += _year % 10 + '0';
    return result;
}

int Date::to_int() const
{
    return _day + _month * 100 + _year * 10'000;
}

// ------------------------------ Task

Task::Task() : _state{TODO}
{
}

Task::Task(const std::string &line)
{
    int size = line.size();
    int i[4]{}; // an array of seperaors' indexes
    for (int k{}, j{}; k < size; ++k)
    {
        if (line[k] == '|')
        {
            i[j++] = k;
        }
    }
    _description = line.substr(i[0] + 2, i[1] - i[0] - 3);
    _assignee = line.substr(i[1] + 2, i[2] - i[1] - 3);
    _deadline = Date(line.substr(i[2] + 2, i[3] - i[2] - 3));
    std::string state_in_string;
    state_in_string = line.substr(i[3] + 2, size - i[3] - 1);
    if (state_in_string == "TODO")
        _state = TODO;
    else if (state_in_string == "IN_PROGRESS")
        _state = IN_PROGRESS;
    else if (state_in_string == "IN_REVIEW")
        _state = IN_REVIEW;
    else
        _state = DONE;
}

void Task::set_description(const std::string &desc)
{
    _description = desc;
}

std::string Task::get_description() const
{
    return _description;
}

void Task::set_assignee(const std::string &as)
{
    _assignee = as;
}

std::string Task::get_assignee() const
{
    return _assignee;
}

void Task::set_deadline(Date date)
{
    _deadline = date;
}

Date Task::get_deadline() const
{
    return _deadline;
}

void Task::set_state(State state)
{
    _state = state;
}

State Task::get_state() const
{
    return _state;
}

std::string Task::to_string() const
{
    std::string result = "00 | ";
    result += _description;
    result += " | ";
    result += _assignee;
    result += " | ";
    result += _deadline.to_string();
    result += " | ";
    switch (_state)
    {
    case 0:
        result += "TODO";
        break;
    case 1:
        result += "IN_PROGRESS";
        break;
    case 2:
        result += "IN_REVIEW";
        break;
    case 3:
        result += "DONE";
        break;
    }
    return result;
}

// ----------------------------- Project

Project::Project() {}

Project::Project(const std::vector<std::string> &strings)
{
    int strings_size = strings.size();
    int title_size = strings[0].size();
    _title = strings[0].substr(8, title_size - 8);
    for (int i{1}; i < strings_size; ++i)
    {
        _tasks.push_back(Task(strings[i]));
    }
}

void Project::set_title(const std::string &title)
{
    _title = title;
}

std::string Project::get_title() const
{
    return _title;
}

std::vector<Task> &Project::get_tasks()
{
    return _tasks;
}

void Project::add_task(const Task &task)
{
    _tasks.push_back(task);
}

void Project::remove_task(unsigned index)
{
    _tasks.erase(_tasks.begin() + index);
}

void Project::sort_tasks()
{
    int size = _tasks.size();
    for (int i{1}; i < size; ++i)
    {
        Task tmp{};
        tmp = _tasks[i];
        int j = i - 1;
        while (j >= 0 && _tasks[j].get_deadline().to_int() > tmp.get_deadline().to_int())
        {
            _tasks[j + 1] = _tasks[j];
            --j;
        }
        _tasks[j + 1] = tmp;
    }
}

std::vector<std::string> Project::to_strings() const
{
    std::vector<std::string> strings;
    strings.push_back("project " + _title);
    int size = _tasks.size();
    for (int i{}; i < size; ++i)
    {
        strings.push_back(_tasks[i].to_string());
    }
    return strings;
}

// ------------------------------------ Manager

Manager::Manager(const std::string &filename)
    : _filename{filename}
{
}

void Manager::from_file_to_lines() const
{
    std::ifstream fin(_filename, std::ios::in);
    std::string line;
    if (fin.is_open())
    {
        while (!fin.eof())
        {
            std::getline(fin, line);
            lines.push_back(line);
        }
    }
    fin.close();
    if (lines.size())
        lines.pop_back();
}

void Manager::from_lines_to_projects() const
{
    projects.clear();
    std::vector<int> indexes;
    for (int i{}; i < lines.size(); ++i)
    {
        if (lines[i][0] == 'p')
        {
            indexes.push_back(i);
            // std::cout << i;
        }
    }
    for (int i{}; i < indexes.size() - 1; ++i)
    {
        std::vector<std::string> project;
        for (int j = indexes[i]; j < indexes[i + 1]; ++j)
        {
            project.push_back(lines[j]);
        }
        projects.push_back(Project(project));
    }
    std::vector<std::string> last_project;
    for (int i = indexes.back(); i < lines.size(); ++i)
    {
        last_project.push_back(lines[i]);
    }
    projects.push_back(Project(last_project));
}

void Manager::display() const
{
    int size = projects.size();
    for (int i{}; i < size; ++i)
    {
        std::cout << "project " << projects[i].get_title() << "\n";
        std::vector<Task> tasks = projects[i].get_tasks();
        int tasks_size = tasks.size();
        for (int j{}; j < tasks_size; ++j)
        {
            if (j < 9)
                std::cout << 0;
            std::cout << j + 1;
            std::cout << " | ";
            if (tasks[j].get_description().size() < 30)
            {
                std::cout << tasks[j].get_description();
                for (int i{}; i < 30 - tasks[j].get_description().size(); ++i)
                {
                    std::cout << ' ';
                }
            }
            else
            {
                for (int i{}; i < 27; ++i)
                {
                    std::cout << tasks[j].get_description()[i];
                }
                std::cout << "...";
            }
            std::cout << " | ";
            // std::cout << tasks[j].get_assignee();
            if (tasks[j].get_assignee().size() < 10)
            {
                std::cout << tasks[j].get_assignee();
                for (int i{}; i < 10 - tasks[j].get_assignee().size(); ++i)
                {
                    std::cout << ' ';
                }
            }
            else
            {
                for (int i{}; i < 7; ++i)
                {
                    std::cout << tasks[j].get_assignee()[i];
                }
                std::cout << "...";
            }
            std::cout << " | ";
            std::cout << tasks[j].get_deadline().to_string();
            std::cout << " | ";
            switch (tasks[j].get_state())
            {
            case 0:
                std::cout << "TODO";
                break;
            case 1:
                std::cout << "IN_PROGRESS";
                break;
            case 2:
                std::cout << "IN_REVIEW";
                break;
            case 3:
                std::cout << "DONE";
                break;
            }
            std::cout << std::endl;
        }
        for (int i{}; i < 70; ++i)
        {
            std::cout << '-';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "[A] to add a project\n";
    std::cout << "[E] to edit a project\n";
    std::cout << "[D] to delete a project\n";
    std::cout << "[S] to save and quit\n\n\n\n";
}

void Manager::edit() const
{
    char choice{};
    while (true)
    {
        std::cin >> choice;
        if (choice == 'A')
            this->add_project();
        else if((choice == 'E') || choice == 'D')
        {
            std::string title;
            std::cout << "title : ";
            std::cin >> title;
            if (this->project_exists(title))
            {
                int index;
                for (int i{}; i < projects.size(); ++i)
                {
                    if (projects[i].get_title() == title)
                    {
                        index = i;
                        break;
                    }
                }
                if (choice == 'E')
                    this->edit_project(index);
                else if (choice == 'D')
                    this->delete_project(index);
            }
        }
        else
        {
            this->from_projects_to_lines();
            this->from_lines_to_file();
            break;
        }
        this->display();
    }
}

void Manager::from_projects_to_lines() const
{
    lines.clear();
    int size = projects.size();
    for (int i{}; i < size; ++i)
    {
        int project_size = projects[i].to_strings().size();
        for (int j{}; j < project_size; ++j)
        {
            lines.push_back(projects[i].to_strings()[j]);
        }
    }
}

void Manager::from_lines_to_file() const
{
    std::ofstream fout(_filename, std::ios::out);
    if (fout.is_open())
    {
        int size = lines.size();
        for (int i{}; i < size; ++i)
        {
            fout << lines[i] << std::endl;
        }
    }
    fout.close();
}

void Manager::add_project() const
{
    std::string title{};
    do
    {
        std::cout << "enter project's name : ";
        std::cin >> title;
    } while (this->project_exists(title));
    Project new_project;
    new_project.set_title(title);
    projects.push_back(new_project);
}

void Manager::edit_project(int x) const
{
    std::cout << "[C] to change title\n";
    std::cout << "[A] to add a task\n";
    std::cout << "[E] to edit a task\n";
    std::cout << "[D] to delete a task\n\n";

    char choice{};
    std::cin >> choice;
    if (choice == 'C')
    {
        std::string title;
        std::cout << "new title : ";
        std::cin >> title;
        projects[x].set_title(title);
    }
    else if (choice == 'A')
    {
        this->add_task(x);
    }
    else if (choice == 'E')
    {
        int y;
        std::cout << "number : ";
        std::cin >> y;
        std::cin.get();
        this->edit_task(x, y - 1);
    }
    else if (choice == 'D')
    {
        int y;
        std::cout << "number : ";
        std::cin >> y;
        this->delete_task(x, y - 1);
    }
}

void Manager::delete_project(int x) const
{
    std::vector<Project>::iterator it = projects.begin();
    projects.erase(it + x);
}

void Manager::add_task(int index) const
{
    Task new_task;
    std::string temp{};
    std::cin.get();

    std::cout << "description : ";
    std::getline(std::cin, temp);
    new_task.set_description(temp);

    std::cout << "assignee : ";
    std::getline(std::cin, temp);
    new_task.set_assignee(temp);

    std::cout << "deadline : ";
    std::cin >> temp;
    Date deadline(temp);
    new_task.set_deadline(deadline);

    std::cout << "state : ";
    std::cin >> temp;
    if (temp == "TODO")
        new_task.set_state(TODO);
    else if (temp == "IN_PROGRESS")
        new_task.set_state(IN_PROGRESS);
    else if (temp == "IN_REVIEW")
        new_task.set_state(IN_REVIEW);
    else if (temp == "DONE")
        new_task.set_state(DONE);
    else
        new_task.set_state(TODO);
    projects[index].add_task(new_task);
}

void Manager::edit_task(int x, int y) const
{
    std::string temp;
    std::cout << "description : ";
    std::getline(std::cin, temp);
    projects[x].get_tasks()[y].set_description(temp);

    std::cout << "assignee : ";
    std::getline(std::cin, temp);
    projects[x].get_tasks()[y].set_assignee(temp);

    std::cout << "deadline : ";
    std::cin >> temp;
    Date date(temp);
    projects[x].get_tasks()[y].set_deadline(date);

    std::cout << "state : ";
    std::cin >> temp;
    if (temp == "TODO")
        projects[x].get_tasks()[y].set_state(TODO);
    else if (temp == "IN_PROGRESS")
        projects[x].get_tasks()[y].set_state(IN_PROGRESS);
    else if (temp == "IN_REVIEW")
        projects[x].get_tasks()[y].set_state(IN_REVIEW);
    else if (temp == "DONE")
        projects[x].get_tasks()[y].set_state(DONE);
}

void Manager::delete_task(int x, int y) const
{
    std::vector<Task>::iterator it = projects[x].get_tasks().begin();
    projects[x].get_tasks().erase(it + y);
}

bool Manager::project_exists(std::string title) const
{
    for (int i{}; i < projects.size(); ++i)
        if (projects[i].get_title() == title)
            return true;
    return false;
}
