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

#include <string>
#include <cstring>

#include "graph.h"

std::string get_graph_by_percentage( unsigned value, unsigned len ) 
{
  unsigned step = 0;
  std::string bars;

  unsigned bar_count = ( static_cast<float>(value) / 99.9 * len );

  for( ; step < bar_count; step++ )
  {
    bars.append( "|" );
  }
  for( ; step < len; step++ )
  {
    bars.append( " " );
  }

  return bars;
}


std::string get_graph_by_value( unsigned value, unsigned max, unsigned len )
{
  unsigned step = 0;
  std::string bars;

  unsigned bar_count = ( static_cast<float>( value / ( max - 0.1 ) ) * len );

  for( ; step < bar_count; step++ )
  {
    bars.append( "|" );
  }
  for( ; step < len; step++ )
  {
    bars.append( " " );
  }

  return bars;
}

