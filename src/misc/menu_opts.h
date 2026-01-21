#pragma once

namespace todo {
enum class MenuOptions {
    EXIT = 0,
    ADD = 1,
    REMOVE = 2,
    CHANGE_DESC = 3,
    CHANGE_STATUS = 4,
    CHANGE_PRIO = 5,
    UNDO = 6,
    REDO = 7,
    CLEAR = 8,
    INVALID = 9
};
}  // namespace todo
