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

#include <iostream>
#include <sys/errno.h>
#include <cerrno>

#include "error.h"

void error( const char * error )
{
  using std::cerr;
  using std::endl;

  cerr << error;
  
  switch( errno )
  {
    case EFAULT:
      cerr << "Bad address";
      break;
    case EINVAL:
      cerr << "Invalid argument";
      break;
    case ENOMEM:
      cerr << "Cannot allocate memory";
      break;
    case ENOENT:
      cerr << "No such file or directory";
      break;
    case ENXIO:
      cerr << "Device not configured";
      break;
    case ENOTDIR:
      cerr << "Not a directory";
      break;
    case EOPNOTSUPP:
      cerr << "Operation not supported";
      break;
    case EPERM:
      cerr << "Operation not permitted";
      break;
    case ESRCH:
      cerr << "No such process";
      break;
    default:
      cerr << "Unknown error code: " << errno;
      break;
  }
  
  cerr << endl;

  exit( 23 );
}

