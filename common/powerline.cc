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

#include <string>
#include <cstring>
#include <sstream>

#define PWL_LEFT_FILLED ""
#define PWL_RIGHT_FILLED ""

const char * bg2fg( const char s[] )
{
  static char buf[40] = {0};
  const char *substr = strchr(s, ',');
  buf[0] = '#';
  buf[1] = '[';
  buf[2] = 'f';
  strcpy(buf+3, substr+2);
  return buf;
}


void powerline( std::ostringstream & oss, const char color[],
  POWERLINE_DIRECTION direction, bool background_only )
{
  switch( direction )
    {
  case NONE:
    oss << color;
    break;
  case POWERLINE_LEFT:
    if( background_only )
      {
      oss << ' ' << bg2fg( color );
      }
    else
      {
      std::string colorstr( color );
      oss << "#[" << colorstr.substr( colorstr.find( "," ) + 1 )
          << PWL_LEFT_FILLED
          << color;
      }
    break;
  case POWERLINE_RIGHT:
    oss << ' '
        << bg2fg( color )
        << PWL_RIGHT_FILLED
        << color;
    break;
    };
}

void powerline_char( std::ostringstream & oss, const char dynamic_color[],
  short static_color, POWERLINE_DIRECTION direction, bool eol )
{
  char write_color[7];
  sprintf(write_color, "%d", static_color);
  switch( direction )
  {
    case POWERLINE_LEFT:
      if ( eol )
      {
      oss << bg2fg( dynamic_color ) << "#[bg=colour" << write_color << "]";
      }
      else
      {
      oss << dynamic_color << "#[fg=colour" << write_color << "]";
      }
      oss << PWL_LEFT_FILLED << dynamic_color;
    break;
    case POWERLINE_RIGHT:
      if ( eol )
      {
        oss << dynamic_color << "#[fg=colour" << write_color << "] ";
      }
      else
      {
        oss << bg2fg(dynamic_color) << " #[bg=colour" << write_color << "]";
      }
      oss << PWL_RIGHT_FILLED << dynamic_color;
    break;
  }
}
