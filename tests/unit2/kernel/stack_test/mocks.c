/*
Substitutional mocks
*/

static ALWAYS_INLINE k_spinlock_key_t mock_k_spin_lock(struct k_spinlock *l)
{
	//printf("in mock_k_spin_lock!\n");
	check_expected(l);
	l->locked = 1;

    k_spinlock_key_t retKey;
	retKey.key = mock();

	return retKey;
}

static ALWAYS_INLINE void mock_k_spin_unlock(struct k_spinlock *l,
					k_spinlock_key_t key)
{
	//printf("in mock_k_spin_lock!\n");
	check_expected(l);
	l->locked = 0;
}

static ALWAYS_INLINE void mock_z_ready_thread(struct k_thread *thread)
{
	check_expected(thread);
}

static ALWAYS_INLINE void
mock_z_thread_return_value_set_with_data(struct k_thread *thread,
				   unsigned int value,
				   void *data)
{
	check_expected(thread);
	check_expected(value);
	stack_data_t mock_pushedData = mock();
	assert_int_equal((stack_data_t)data, mock_pushedData);
}


/*
Undefined mocks
*/
void *z_thread_malloc(size_t size)
{

}

void k_free(void *ptr)
{
	free(ptr);
}

struct k_thread *z_unpend_first_thread(_wait_q_t *wait_q)
{
	check_expected(wait_q);
	return (struct k_thread *)mock();
}

void z_add_thread_to_ready_q(struct k_thread *thread)
{

}

void z_reschedule(struct k_spinlock *lock, k_spinlock_key_t key)
{
	check_expected(lock);
	int mock_key = mock();
	assert_int_equal(key.key, mock_key);

	k_spin_unlock(lock, key);// will jump to mock_k_spin_unlock() at runtime!
}

int z_pend_curr(struct k_spinlock *lock, k_spinlock_key_t key,
	       _wait_q_t *wait_q, s32_t timeout)
{
	
}


