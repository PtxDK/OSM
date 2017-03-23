

// Create thread with NULL(default thread attributes)
thread_identifier_t pthread_create(&thread ID , NULL, thr e ad func t i on , NULL);


// Releases one thread waiting on a condition
pthread_cond_signal();

// Releases all threads waiting on a condition
pthread_cond_broadcast();



// Collect the exit status of a thread
pthread_join();

// Force thread to stop. May cause memory leak
pthread_cancel();



// Blocks thread
pthread_cond_wait();



// Initialises / creates a mutex
pthread_mutex_init();

// Attemt to gain exclusive access to the shared data object (mutex), and therefore lock it
pthread_mutex_lock();

// Release the exclusive access
pthread_mutex_unlock();

// Destroys the mutex
pthread_mutex_destroy();


// 



