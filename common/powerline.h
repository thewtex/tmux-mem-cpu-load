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

#ifndef POWERLINE_H
#define POWERLINE_H

#include <sstream>

enum POWERLINE_DIRECTION
{
  NONE,
  POWERLINE_LEFT,
  POWERLINE_RIGHT
};

/** Print out a powerline left character inverted version of the given
 * color. In the case of of using powerline left, the background color needs
 * to be inverted to the foreground before the powerline character is printed
 * in the next entr. */
void powerline( std::ostringstream & oss, const char color[],
  POWERLINE_DIRECTION direction, bool background_only = false );
void powerline_char( std::ostringstream & oss, const char dynamic_color[],
  short static_color, POWERLINE_DIRECTION direction, bool eol = false );

#endif // POWERLINE_H
