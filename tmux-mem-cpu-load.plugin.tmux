 # vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap
 #
 # Copyright 2012 Matthew McCormick
 # Copyright 2015 Pawel 'l0ner' Soltys
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.

# The directory where this plugin is located.
CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! -f $CURRENT_DIR/tmux-mem-cpu-load ] && ! $(builtin type -P "tmux-mem-cpu-load" &> /dev/null) ; then
   tmux run-shell "echo \"tmux-mem-cpu-load not found. Attempting to build.
   \""

   pushd $CURRENT_DIR #Pushd to the directory where this plugin is located.

   # Attempt to rebuild the plugin and log any errors in the tmux display window.
   if output=$(cmake . 2>&1); then
      tmux run-shell "echo \"'cmake $CURRENT_DIR' completed successfully.
      \""
   else
      tmux run-shell "echo \"'cmake $CURRENT_DIR' failed. Error logged below.
      $output
      \""
      exit 1
   fi

   if output=$(make 2>&1); then 
      tmux run-shell "echo \"tmux-mem-cpu-load built successfully.
      \""
   else
      tmux run-shell "echo \"tmux-mem-cpu-load failed to build. Error logged below.
      $output
      \""
      exit 1
   fi
   popd
fi

