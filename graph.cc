#include <string>
#include <cstring>

#include "graph.h"

std::string get_graph_by_percentage(unsigned value, unsigned len) 
{
  unsigned step = 0;
  std::string bars;

  unsigned bar_count = (static_cast<float>(value) / 99.9 * len);

  for(step; step < bar_count; step++)
  {
    bars.append("|");
  }
  for(step; step < len; step++)
  {
    bars.append(" ");
  }

  return bars;
}


std::string get_graph_by_value(unsigned value, unsigned max, unsigned len)
{
  unsigned step = 0;
  std::string bars;

  unsigned bar_count = (static_cast<float>(value / (max - 0.1)) * len);

  for(step; step < bar_count; step++)
  {
    bars.append("|");
  }
  for(step; step < len; step++)
  {
    bars.append(" ");
  }

  return bars;
}

