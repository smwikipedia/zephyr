To build:
cd zephyr/tests/unit2
mkdir build
cd build
cmake .. -GNinja -Dzephyr_dir=<zephyr folder of this repo>
ninja

To run:
cd kernel/stack_test/
./stack_test

The result looks like this:

test started
[==========] Running 2 test(s).
[ RUN      ] test_z_impl_k_stack_push_no_pending_thread
[       OK ] test_z_impl_k_stack_push_no_pending_thread
[ RUN      ] test_z_impl_k_stack_push_with_pending_thread
[       OK ] test_z_impl_k_stack_push_with_pending_thread
[==========] 2 test(s) run.
[  PASSED  ] 2 test(s).

