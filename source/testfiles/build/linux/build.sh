CLANG_PATH="/opt/hcc/bin/clang++"
mkdir Test_Executables
if [ -x $CLANG_PATH ]; then
       cmake ../../ -DCMAKE_C_COMPILER=/opt/hcc/bin/clang  -DCMAKE_CXX_COMPILER=/opt/hcc/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC
else
       cmake ../../ -DCMAKE_C_COMPILER=${MCWCPPAMPBUILD}/compiler/bin/clang  -DCMAKE_CXX_COMPILER=${MCWCPPAMPBUILD}/compiler/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC
fi
