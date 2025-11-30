#include <curses.h>

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "basic_view.h"

void clear_input_buf()
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Todo::MenuOptions Todo::BasicView::get_menu_opt()
{
  std::cout << "1. Add a task\n"
            << "2. Remove a task\n"
            << "3. Change completion status of a task\n"
            << "0. Exit\n"
            << "Enter choice: ";

  size_t opt;
  std::cin >> opt;
  clear_input_buf();

  if (opt >= 0 && opt <= 5)
  {
    return static_cast<MenuOptions>(opt);
  }
  return MenuOptions::INVALID;
}

std::string Todo::BasicView::get_task_desc(const std::string &msg)
{
  std::cout << msg;
  std::string desc;
  std::getline(std::cin, desc);
  return desc;
}

std::vector<size_t> Todo::BasicView::get_path(const std::string &msg)
{
  std::string line;
  std::vector<size_t> path;
  while (true)
  {
    std::cout << msg;
    std::getline(std::cin, line);
    if (line.empty())
    {
      break;
    }

    try
    {
      size_t index = std::stoul(line);
      path.emplace_back(index);
    }
    catch (const std::exception &e)
    {
      std::cerr << "err: " << e.what();
    }
  }

  return path;
}

uint8_t Todo::BasicView::get_status_change(const std::string &msg)
{
  std::cout << msg;
  uint8_t index;
  std::cin >> index;
  clear_input_buf();
  return index;
}

void Todo::BasicView::display_list(const std::vector<Todo::Task> &todo_list,
                                   size_t level)
{
  size_t id = 1;
  for (const auto &t : todo_list)
  {
    std::string status = [&t]() -> std::string
    {
      switch (t.status)
      {
        case Status::NOT_STARTED:
          return "[ ]";
        case Status::IN_PROGRESS:
          return "[-]";
        case Status::COMPLETED:
          return "[X]";
        default:
          return "[404]";
      }
    }();

    std::cout << std::string(level, ' ') << id << ". " << status << ' '
              << t.desc << '\n';

    if (todo_list[id - 1].child_tasks.size() != 0)
    {
      display_list(todo_list[id - 1].child_tasks, level + 1);
    }

    ++id;
  }
  std::cout << '\n';
}

void Todo::BasicView::display_msg(const std::string &msg)
{
  std::cout << msg << '\n';
}
