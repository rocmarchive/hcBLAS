# bash script to install the library 

#invoke build script with install argument 

#also do a preliminary basic testing

sh build.sh --test=basic 

# Use dpkg package installer to install the generated debian package

sudo dpkg -i $PWD/build/hcblas*.deb

sudo ldconfig
