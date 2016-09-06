# Gripper-module

To get you started with C++ 11, required cmake 3.1 or later.
# Check version cmake
    $ cmake -version

# Unistall cmake 
    $ sudo apt-get remove cmake
    $ sudo apt-get autoremove
# Install cmake 3.4
    $ wget http://www.cmake.org/files/v3.4/cmake-3.4.1.tar.gz 
    $ tar -xvzf cmake-3.4.1.tar.gz 
    $ cd cmake-3.4.1/ 
    $ ./configure 
    $ make 
    $ sudo make install
    $ update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1 --force

# Install client c++ 11
	
	$ git clone https://github.com/AIO-Javeriana/gripper-module.git
	$ cd gripper-module
    $ wget https://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
    $ tar -xzvf boost_1_58_0.tar.gz
    $ rm boost_1_58_0.tar.gz
    $ cmake -DBOOST_INCLUDEDIR=./boost_1_58_0 -DBOOST_LIBRARYDIR=./boost_1_58_0/libs -DBOOST_VER:STRING=1.58 .
    $ make
    $ ./gripper_module

#Other option
	Other option is install library from repository
	$ sudo apt-get install cmake libblkid-dev e2fslibs-dev libboost-all-dev libaudit-dev

Happy coding!


