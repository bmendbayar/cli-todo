#include <ncurses.h>

#include <cctype>
#include <string>
#include <vector>

#include "i_view.h"

namespace Todo
{
IView::IView()
{
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  int height = LINES;
  int width = COLS;

  int menu_height = 12;
  int list_height = height - menu_height;

  list_win_ = newwin(list_height, width, 0, 0);
  menu_win_ = newwin(menu_height, width, list_height, 0);
}

IView::~IView()
{
  delwin(list_win_);
  delwin(menu_win_);
  endwin();
}

MenuOptions IView::get_menu_opt()
{
  wclear(menu_win_);
  box(menu_win_, 0, 0);

  mvwprintw(menu_win_, 1, 2, "=== Todo Menu ===");
  mvwprintw(menu_win_, 3, 2, "1. Add task");
  mvwprintw(menu_win_, 4, 2, "2. Remove task");
  mvwprintw(menu_win_, 5, 2, "3. Change task status");
  mvwprintw(menu_win_, 6, 2, "4. Clear list");
  mvwprintw(menu_win_, 7, 2, "0. Exit");
  mvwprintw(menu_win_, 9, 2, "Choose an option: ");

  wrefresh(menu_win_);
  int ch = wgetch(menu_win_);
  switch (ch)
  {
    case '1':
      return MenuOptions::ADD;
    case '2':
      return MenuOptions::REMOVE;
    case '3':
      return MenuOptions::CHANGE_STATUS;
    case '4':
      return MenuOptions::CLEAR;
    case '0':
      return MenuOptions::EXIT;
    default:
      return MenuOptions::INVALID;
  }
}

std::string IView::get_task_desc(const std::string &msg)
{
  wclear(menu_win_);
  box(menu_win_, 0, 0);
  mvwprintw(menu_win_, 1, 2, "%s", msg.c_str());
  mvwprintw(menu_win_, 3, 2, "> ");
  wrefresh(menu_win_);

  char buffer[256];
  echo();
  curs_set(1);
  wgetnstr(menu_win_, buffer, 255);
  noecho();
  curs_set(0);

  return std::string(buffer);
}

std::vector<size_t> IView::get_path(const std::string &msg)
{
  std::vector<size_t> path;
  while (true)
  {
    wclear(menu_win_);
    box(menu_win_, 0, 0);
    mvwprintw(menu_win_, 1, 2, "%s", msg.c_str());
    mvwprintw(menu_win_, 3, 2, "> ");
    wrefresh(menu_win_);

    echo();
    curs_set(1);
    char buf[2];
    wgetnstr(menu_win_, buf, 1);
    noecho();
    curs_set(0);

    if (buf[0] == '\0')
    {
      break;
    }

    if (not isdigit(buf[0]))
    {
      std::exit(EXIT_FAILURE);
    }

    path.emplace_back(atoi(&buf[0]) - 1);
  }
  return path;
}

uint8_t IView::get_status_change(const std::string &msg)
{
  uint8_t index;
  while (true)
  {
    wclear(menu_win_);
    box(menu_win_, 0, 0);
    mvwprintw(menu_win_, 1, 2, "%s", msg.c_str());
    mvwprintw(menu_win_, 2, 2, "1 - Not Started");
    mvwprintw(menu_win_, 3, 2, "2 - In Progress");
    mvwprintw(menu_win_, 4, 2, "3 - Completed");
    mvwprintw(menu_win_, 5, 2, "> ");
    wrefresh(menu_win_);

    echo();
    curs_set(1);
    char buf[2];
    wgetnstr(menu_win_, buf, 1);
    noecho();
    curs_set(0);

    if (not isdigit(buf[0]))
    {
      wclear(menu_win_);
      box(menu_win_, 0, 0);
      mvwprintw(menu_win_, 1, 2, "Error: Not a number");
      wrefresh(menu_win_);
      continue;
    }

    index = atoi(&buf[0]);
    break;
  }
  return index;
}

void IView::display_list(const std::vector<Todo::Task> &todo_list, size_t level)
{
  if (level == 0)
  {
    wclear(list_win_);
    box(list_win_, 0, 0);
    mvwprintw(list_win_, 1, 2, "=== Todo List ===");
    wmove(list_win_, 3, 0);
  }

  for (size_t i = 0; i < todo_list.size(); ++i)
  {
    const auto &t = todo_list[i];
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

    int y = getcury(list_win_);
    int x = 2 + (level * 2);

    mvwprintw(list_win_, y, x, "%zu. %s %s", i + 1, status.c_str(),
              t.desc.c_str());
    wprintw(list_win_, "\n");

    if (!t.child_tasks.empty())
    {
      display_list(t.child_tasks, level + 1);
    }
  }

  if (todo_list.empty() && level == 0)
  {
    mvwprintw(list_win_, 3, 2, "(no tasks)");
  }

  if (level == 0)
  {
    wrefresh(list_win_);
  }
}

void IView::display_msg(const std::string &msg)
{
  wclear(menu_win_);
  box(menu_win_, 0, 0);
  mvwprintw(menu_win_, 2, 2, "%s", msg.c_str());
  mvwprintw(menu_win_, 4, 2, "press any key to continue ");
  wrefresh(menu_win_);
  wgetch(menu_win_);
}
}  // namespace Todo
