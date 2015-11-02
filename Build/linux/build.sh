CLANG_PATH="/opt/clamp/bin/clang++"

if [ -x $CLANG_PATH ]; then
       cmake ../../source -DCMAKE_C_COMPILER=/opt/clamp/bin/clang  -DCMAKE_CXX_COMPILER=/opt/clamp/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC
else
       cmake ../../source -DCMAKE_C_COMPILER=${MCWCPPAMPBUILD}/compiler/bin/clang  -DCMAKE_CXX_COMPILER=${MCWCPPAMPBUILD}/compiler/bin/clang++ -DCMAKE_CXX_FLAGS=-fPIC
fi
