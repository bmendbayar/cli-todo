#pragma once

#include <ncurses.h>

#include "view.h"

namespace Todo
{
class IView : public View
{
private:
  WINDOW *menu_win_;
  WINDOW *list_win_;

public:
  IView();

  virtual ~IView();

  virtual MenuOptions get_menu_opt() override;

  virtual std::string get_task_desc(const std::string &msg) override;

  virtual std::vector<size_t> get_path(const std::string &msg) override;

  virtual uint8_t get_status_change(const std::string &msg) override;

  virtual void display_list(const std::vector<Todo::Task> &todo_list,
                            size_t level = 0) override;

  virtual void display_msg(const std::string &msg) override;
};
}  // namespace Todo
