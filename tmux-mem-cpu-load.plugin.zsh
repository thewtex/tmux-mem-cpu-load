# vim: tabstop=2 shiftwidth=2 expandtab textwidth=80 linebreak wrap

pushd ${0:a:h} #Pushd to the directory where this file is located.
cmake .
make
sudo make install
popd
