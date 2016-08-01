/* vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 *
 * Copyright 2012 Matthew McCormick
 * Copyright 2015 Pawel 'l0ner' Soltys
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

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

enum BYTE_UNITS
{
  BYTES = 0,
  KILOBYTES = 1,
  MEGABYTES = 2,
  GIGABYTES = 3
};

template <class T>
inline T convert_unit( T num, int to, int from = BYTES)
{
  for( ; from < to; from++)
  {
    num /= 1024;
  }
  return num;
}

#endif
