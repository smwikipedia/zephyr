#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include "MockEngine.h"


#include "kernel.h"
#include "mocks.c"
#include "stack.c"


#define mock_stack_size 20
struct k_stack mock_stack;
stack_data_t _mock_stack_buffer[mock_stack_size];
stack_data_t *mock_stack_buffer = &_mock_stack_buffer[0];

static int TestSetup(void **state)
{
    k_stack_init(&mock_stack, mock_stack_buffer, mock_stack_size);
    return 0;
}

static int TestTearDown(void **state)
{
    //because the stack is backed by an array rather than allocated dynamically, it cannot be freed.
    //k_free(mock_stack.base);
    return 0;
}


static void test_z_impl_k_stack_push_no_pending_thread(void **state)
{
    //k_spin_lock is defined in a header, which is included by the stack.c
    //so no extra declaration is needed
    RegisterOneMock((void *)k_spin_lock, (void *)mock_k_spin_lock);
    RegisterOneMock((void *)k_spin_unlock, (void *)mock_k_spin_unlock);


    // all the immediate dependent functions by the target function should be properly setup for parameter checking and mock return value.
    // inluding both undefined mocks and substitutional mocks.

    /*
    *
    *  Step 1. prepare mocks
    * 
    */
    stack_data_t mock_pushedData = 123;
    int mock_key = 456;

    //mock_k_spin_lock()
    expect_not_value(mock_k_spin_lock, l, NULL);
    will_return(mock_k_spin_lock, mock_key);

    //mock_k_spin_unlock()
    expect_not_value(mock_k_spin_unlock, l, NULL);

    //z_unpend_first_thread()
    expect_not_value(z_unpend_first_thread, wait_q, NULL);
    will_return(z_unpend_first_thread, NULL);
  

    /*
    *
    *  Step 2. begin test
    * 
    */
    //pre-check of the stack status
    assert_int_equal(mock_stack.lock.locked, 0); // the stack should start as unlocked state
    assert_ptr_equal(mock_stack.base, mock_stack.next); // the stack should be empty
    assert_ptr_equal(mock_stack.base, mock_stack_buffer);
    assert_ptr_equal(mock_stack.top, mock_stack_buffer + mock_stack_size);    
    
    /* >>>>> TEST TARGET <<<<< */
    z_impl_k_stack_push(&mock_stack, mock_pushedData);
    

    //post-check of the stack status
    assert_int_equal(mock_stack.lock.locked, 0); // the stack should return to unlocked state
    stack_data_t topData = *((stack_data_t *)mock_stack.next - 1); // the stack should contain 1 element now
    assert_int_equal(topData, mock_pushedData);

    //because the waitq manipulation is not part of the test target, so it is not of interest to this test. 
    //thus we don't check the waitq status in THIS test.    
    //assert_ptr_equal(&mock_stack.wait_q.waitq, mock_stack.wait_q.waitq.head);
    //assert_ptr_equal(&mock_stack.wait_q.waitq, mock_stack.wait_q.waitq.next);
    
}

static void test_z_impl_k_stack_push_with_pending_thread(void **state)
{
    //k_spin_lock is defined in a header, which is included by the stack.c
    //so no extra declaration is needed
    RegisterOneMock((void *)k_spin_lock, (void *)mock_k_spin_lock);
    RegisterOneMock((void *)k_spin_unlock, (void *)mock_k_spin_unlock);
    RegisterOneMock((void *)z_ready_thread, (void *)mock_z_ready_thread);
    RegisterOneMock((void *)z_thread_return_value_set_with_data, (void *)mock_z_thread_return_value_set_with_data);



    // all the immediate dependent functions by the target function should be properly setup for parameter checking and mock return value.
    // inluding both undefined mocks and substitutional mocks.

    /*
    *
    *  Step 1. prepare mocks
    * 
    */
    stack_data_t mock_pushedData = 123;
    int mock_key = 456;

    //k_spin_lock()
    expect_not_value(mock_k_spin_lock, l, NULL);
    will_return(mock_k_spin_lock, mock_key);

    //k_spin_unlock()
    expect_not_value(mock_k_spin_unlock, l, NULL); 

    //z_unpend_first_thread()
    struct k_thread dummy_pending_thread;
    expect_not_value(z_unpend_first_thread, wait_q, NULL);
    will_return(z_unpend_first_thread, &dummy_pending_thread);

    assert_int_equal(mock_stack.lock.locked, 0);    
    //void z_impl_k_stack_push(struct k_stack *stack, stack_data_t data)

    //z_ready_thread()
    expect_not_value(mock_z_ready_thread, thread, NULL);

    //z_thread_return_value_set_with_data()
    expect_not_value(mock_z_thread_return_value_set_with_data, thread, NULL);
    expect_value(mock_z_thread_return_value_set_with_data, value, 0);
    will_return(mock_z_thread_return_value_set_with_data, mock_pushedData); // to check the "void* data" parameter, use the will_return rather than expect_* to pass in the expected data.

    //z_reschedule()
    expect_not_value(z_reschedule, lock, NULL);
    will_return(z_reschedule, mock_key);


    /*
    *
    *  Step 2. begin test
    * 
    */
    //pre-check of the stack status
    assert_int_equal(mock_stack.lock.locked, 0); // the stack should start as unlocked state
    assert_ptr_equal(mock_stack.base, mock_stack.next); // the stack should be empty
    assert_ptr_equal(mock_stack.base, mock_stack_buffer);
    assert_ptr_equal(mock_stack.top, mock_stack_buffer + mock_stack_size);     

    /* >>>>> TEST TARGET <<<<< */
    z_impl_k_stack_push(&mock_stack, mock_pushedData);
    
    //post-check of the stack status
    assert_int_equal(mock_stack.lock.locked, 0); // the stack should return to unlocked state
    assert_ptr_equal(mock_stack.base, mock_stack.next); // the stack should remain empty
    assert_ptr_equal(mock_stack.base, mock_stack_buffer);
    assert_ptr_equal(mock_stack.top, mock_stack_buffer + mock_stack_size);

    //because the waitq manipulation is not part of the test target, so it is not of interest to this test.
    //thus we don't check the waitq status in THIS test.
    //assert_ptr_equal(&mock_stack.wait_q.waitq, mock_stack.wait_q.waitq.head);
    //assert_ptr_equal(&mock_stack.wait_q.waitq, mock_stack.wait_q.waitq.next);
}


int main(void)
{
    printf("test started\n");

    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_z_impl_k_stack_push_no_pending_thread,
                                        TestSetup, TestTearDown),
        cmocka_unit_test_setup_teardown(test_z_impl_k_stack_push_with_pending_thread,
                                        TestSetup, TestTearDown)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}



