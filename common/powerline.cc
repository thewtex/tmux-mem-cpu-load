#include "powerline.h"

#include <cstring>
#include <sstream>

#define PWL_RIGHT_FILLED ""

const char * bg2fg(const char s[])
{
  static char buf[40] = {0};
  const char *substr = index(s, ',');
  buf[0] = '#';
  buf[1] = '[';
  buf[2] = 'f';
  strcpy(buf+3, substr+2);
  return buf;
}

void powerline(std::ostringstream &oss, const char s[], bool use_powerline)
{
    if (use_powerline)
    oss << bg2fg(s)
        << PWL_RIGHT_FILLED
        << s << ' ';
    else
        oss << s;
}
