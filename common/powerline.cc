/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
 * Copyright 2012 Matthew McCormick
 * Copyright 2016 Michał Goliński
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
    if( use_powerline )
    {
        oss << bg2fg(s)
            << PWL_RIGHT_FILLED
            << s << ' ';
    }
    else
    {
        oss << s;
    }
}
