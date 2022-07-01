#include "project_management_system.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

extern std::vector<std::string> lines;
extern std::vector<Project> projects;

int main(){
    Manager manager("projects.txt");
    manager.from_file_to_lines();
    manager.from_lines_to_projects();
    //std::cout << projects[0].get_tasks()[0].get_deadline().get_year();
    manager.display();
}