#pragma once

#include <boost/describe.hpp>

namespace Todo
{
enum class Status : uint8_t
{
  NOT_STARTED = 1,
  IN_PROGRESS = 2,
  COMPLETED = 3
};
BOOST_DESCRIBE_ENUM(Status, NOT_STARTED, IN_PROGRESS, COMPLETED);
}  // namespace Todo
