#pragma once

#include <vector>

#include "model.h"
#include "types.h"

namespace Todo {
class Action {
protected:
  Model *model_{nullptr};
  std::vector<u16> path_;

public:
  virtual ~Action() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
};

class RemoveAction : public Action {
private:
  Task task_;

public:
  RemoveAction(Model &model, Task task, std::vector<u16> path);
  virtual void execute() override;
  virtual void undo() override;
};

class AddAction : public Action {
private:
  Task task_;

public:
  AddAction(Model &model, Task task, std::vector<u16> path);
  virtual void execute() override;
  virtual void undo() override;
};

class StatusChangeAction : public Action {
private:
  Status new_st_;
  Status old_st_;

public:
  StatusChangeAction(Model &model, Status new_st, Status old_st, std::vector<u16> path);
  virtual void execute() override;
  virtual void undo() override;
};

class PriorityChangeAction : public Action {
private:
  u16 new_p_;
  u16 old_p_;

public:
  PriorityChangeAction(Model &model, u16 new_p, u16 old_p, std::vector<u16> path);
  virtual void execute() override;
  virtual void undo() override;
};

}  // namespace Todo
