./cmake-build-debug-gcc12/generator/generator -o problems.json ./generator/tests/test0.cpp ./generator/tests/test1.cpp ./generator/tests/test2.cpp
./cmake-build-debug-gcc12/converter/converter --problems problems.json --header include/xitren/problems_pool_emb.hpp --output include/xitren/problems_gen.hpp
./cmake-build-debug-gcc12/converter/converter --short --problems problems.json --header include/xitren/problems_pool_emb.hpp --output include/xitren/problems_short_gen.hpp
