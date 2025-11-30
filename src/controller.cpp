#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>

#include "basic_view.h"
#include "controller.h"
#include "i_view.h"

Todo::Controller::Controller(int argc, char **argv)
{
  if (argc == 1)
  {
    view_ = new BasicView();
  }
  else if (strcmp(argv[1], "-i") == 0)
  {
    view_ = new IView();
  }
}

void Todo::Controller::run()
{
  bool running = true;
  while (running)
  {
    handle_display();
    MenuOptions opt = view_->get_menu_opt();
    switch (opt)
    {
      case MenuOptions::ADD:
        handle_add();
        break;

      case MenuOptions::REMOVE:
        handle_remove();
        break;

      case MenuOptions::CHANGE_STATUS:
        handle_status_change();
        break;

      case MenuOptions::CLEAR:
        handle_clear();
        break;

      case MenuOptions::EXIT:
        running = false;
        delete view_;
        model_.save_file();
        break;

      case MenuOptions::INVALID:
      default:
        view_->display_msg("Invalid option. Please try again.");
        break;
    }
  }
}

void Todo::Controller::handle_add()
{
  std::string desc =
      view_->get_task_desc("Enter the description of your task: ");
  std::vector<size_t> path =
      view_->get_path("Enter path of new task (enter blank to stop): ");

  try
  {
    bool res = model_.add(desc, path);
    view_->display_msg(res ? "Success" : "Failure");
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Todo::Controller::handle_remove()
{
  try
  {
    bool res = model_.remove(view_->get_path(
        "Enter the path of the task to remove (enter blank to stop): "));
    view_->display_msg(res ? "Success" : "Failure");
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + static_cast<std::string>(e.what()));
  }
}

void Todo::Controller::handle_display()
{
  view_->display_list(model_.get_list());
}

void Todo::Controller::handle_clear()
{
  model_.clear();
}

void Todo::Controller::handle_status_change()
{
  try
  {
    model_.change_task_status(
        view_->get_path("Enter the path of the task to change the status of "
                        "(enter blank to stop): "),
        view_->get_status_change(
            "Which status would you like to change it to: "));
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + static_cast<std::string>(e.what()));
  }
}
