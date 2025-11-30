#pragma once

#include <string>
#include <vector>

#include "model.h"

namespace Todo
{
enum class MenuOptions
{
  EXIT = 0,
  ADD = 1,
  REMOVE = 2,
  CLEAR = 3,
  CHANGE_STATUS = 4,
  INVALID = 5
};

class View
{
protected:
  View() = default;

public:
  virtual ~View() = default;

  virtual MenuOptions get_menu_opt() = 0;

  virtual std::string get_task_desc(const std::string &msg) = 0;

  virtual std::vector<size_t> get_path(const std::string &msg) = 0;

  virtual uint8_t get_status_change(const std::string &msg) = 0;

  virtual void display_list(const std::vector<Todo::Task> &todo_list,
                            size_t level = 0) = 0;

  virtual void display_msg(const std::string &msg) = 0;
};
}  // namespace Todo
