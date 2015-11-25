CLANG_PATH="/opt/hcc/bin/clang++"

if [ -x $CLANG_PATH ]; then
       cmake ../../source -DCMAKE_C_COMPILER=/opt/hcc/bin/clang  -DCMAKE_CXX_COMPILER=/opt/hcc/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC
else
       cmake ../../source -DCMAKE_C_COMPILER=${MCWCPPAMPBUILD}/compiler/bin/clang  -DCMAKE_CXX_COMPILER=${MCWCPPAMPBUILD}/compiler/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC
fi
