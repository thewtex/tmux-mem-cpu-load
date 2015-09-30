# vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap

if [ ! -s /usr/local/bin/tmux-mem-cpu-load ];
then
  pushd ${0:a:h} #Pushd to the directory where this plugin is located.
  cmake .
  make
  sudo make install
  popd
fi
