struct event_base {
	/** Function pointers and other data to describe this event_base's
	 * backend. */
	const struct eventop *evsel;
	/** Pointer to backend-specific data. */
	void *evbase;

	/** List of changes to tell backend about at next dispatch.  Only used
	 * by the O(1) backends. */
	struct event_changelist changelist;

	/** Function pointers used to describe the backend that this event_base
	 * uses for signals */
	const struct eventop *evsigsel;
	/** Data to implement the common signal handler code. */
	struct evsig_info sig;

	/** Number of virtual events */
	int virtual_event_count;
	/** Maximum number of virtual events active */
	int virtual_event_count_max;
	/** Number of total events added to this event_base */
	int event_count;
	/** Maximum number of total events added to this event_base */
	int event_count_max;
	/** Number of total events active in this event_base */
	int event_count_active;
	/** Maximum number of total events active in this event_base */
	int event_count_active_max;

	/** Set if we should terminate the loop once we're done processing
	 * events. */
	int event_gotterm;
	/** Set if we should terminate the loop immediately */
	int event_break;
	/** Set if we should start a new instance of the loop immediately. */
	int event_continue;

	/** The currently running priority of events */
	int event_running_priority;

	/** Set if we're running the event_base_loop function, to prevent
	 * reentrant invocation. */
	int running_loop;

	/** Set to the number of deferred_cbs we've made 'active' in the
	 * loop.  This is a hack to prevent starvation; it would be smarter
	 * to just use event_config_set_max_dispatch_interval's max_callbacks
	 * feature */
	int n_deferreds_queued;

	/* Active event management. */
	/** An array of nactivequeues queues for active event_callbacks (ones
	 * that have triggered, and whose callbacks need to be called).  Low
	 * priority numbers are more important, and stall higher ones.
	 */
	struct evcallback_list *activequeues;
	/** The length of the activequeues array */
	int nactivequeues;
	/** A list of event_callbacks that should become active the next time
	 * we process events, but not this time. */
	struct evcallback_list active_later_queue;

	/* common timeout logic */

	/** An array of common_timeout_list* for all of the common timeout
	 * values we know. */
	struct common_timeout_list **common_timeout_queues;
	/** The number of entries used in common_timeout_queues */
	int n_common_timeouts;
	/** The total size of common_timeout_queues. */
	int n_common_timeouts_allocated;

	/** Mapping from file descriptors to enabled (added) events */
	struct event_io_map io;

	/** Mapping from signal numbers to enabled (added) events. */
	struct event_signal_map sigmap;

	/** Priority queue of events with timeouts. */
	struct min_heap timeheap;

	/** Stored timeval: used to avoid calling gettimeofday/clock_gettime
	 * too often. */
	struct timeval tv_cache;

	struct evutil_monotonic_timer monotonic_timer;

	/** Difference between internal time (maybe from clock_gettime) and
	 * gettimeofday. */
	struct timeval tv_clock_diff;
	/** Second in which we last updated tv_clock_diff, in monotonic time. */
	time_t last_updated_clock_diff;

#ifndef EVENT__DISABLE_THREAD_SUPPORT
	/* threading support */
	/** The thread currently running the event_loop for this base */
	unsigned long th_owner_id;
	/** A lock to prevent conflicting accesses to this event_base */
	void *th_base_lock;
	/** A condition that gets signalled when we're done processing an
	 * event with waiters on it. */
	void *current_event_cond;
	/** Number of threads blocking on current_event_cond. */
	int current_event_waiters;
#endif
	/** The event whose callback is executing right now */
	struct event_callback *current_event;

#ifdef _WIN32
	/** IOCP support structure, if IOCP is enabled. */
	struct event_iocp_port *iocp;
#endif

	/** Flags that this base was configured with */
	enum event_base_config_flag flags;

	struct timeval max_dispatch_time;
	int max_dispatch_callbacks;
	int limit_callbacks_after_prio;

	/* Notify main thread to wake up break, etc. */
	/** True if the base already has a pending notify, and we don't need
	 * to add any more. */
	int is_notify_pending;
	/** A socketpair used by some th_notify functions to wake up the main
	 * thread. */
	evutil_socket_t th_notify_fd[2];
	/** An event used by some th_notify functions to wake up the main
	 * thread. */
	struct event th_notify;
	/** A function used to wake up the main thread from another thread. */
	int (*th_notify_fn)(struct event_base *base);

	/** Saved seed for weak random number generator. Some backends use
	 * this to produce fairness among sockets. Protected by th_base_lock. */
	struct evutil_weakrand_state weakrand_seed;

	/** List of event_onces that have not yet fired. */
	LIST_HEAD(once_event_list, event_once) once_events;

};