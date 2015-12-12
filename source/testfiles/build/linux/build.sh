mkdir Test_Executables
cmake ../../ -DCMAKE_C_COMPILER=/opt/hcc/bin/clang  -DCMAKE_CXX_COMPILER=/opt/hcc/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC -DPROFILE=$1 
