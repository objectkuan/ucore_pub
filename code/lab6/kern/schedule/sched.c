#include <list.h>
#include <sync.h>
#include <proc.h>
#include <sched.h>
#include <stdio.h>
#include <assert.h>
#include <default_sched_stride.h>
//#include <default_sched.h>

// the list of timer
static list_entry_t timer_list;

static struct sched_class *sched_class;

static struct run_queue *rq;

static int swi = 0;
static void
cp(const char *fmt, ...) {
	if(swi) {
		va_list ap;
		
		va_start(ap, fmt);
		int cnt = vcprintf(fmt, ap);
		va_end(ap);
		
		return cnt;
	}
}

static void
print_p(struct proc_struct* p) {
	cprintf("|-process pid(%d) name(%s)\n", p->pid, p->name);
	cprintf("    |-state: %d\n", p->state);
	cprintf("    |-need_resched: %d\n", p->need_resched);
	cprintf("    |-parent: %d\n", p->parent ? p->parent->pid : -2);
	cprintf("    |-wait_state: %u\n", p->wait_state);
	cprintf("    |-time_slice: %d\n", p->time_slice);
	cprintf("    |-lab6_stride: %u\n", p->lab6_stride);
	cprintf("    |-lab6_priority: %u\n", p->lab6_priority);
}

static void 
print_sh(skew_heap_entry_t* root) {
	struct proc_struct* proc = le2proc(root, lab6_run_pool);
	cprintf("%d(%d)", proc->pid, proc->lab6_stride);
	cprintf("[");
	if(root->left != NULL) print_sh(root->left); else cprintf("-");
	cprintf(",");
	if(root->right != NULL) print_sh(root->right); else cprintf("-");
	cprintf("]");
	
}

void
print_heap() {
	if(!swi) return;
	if(rq->lab6_run_pool == NULL) {
		cprintf("EMPTY HEAP\n");
	} else {
		print_sh(rq->lab6_run_pool);
		cprintf("\n");
	}
}

static void
print_rq() {
	return;
	cprintf("list\n");
	list_entry_t *le = list_next(&(rq->run_list));
	while(le != &(rq->run_list)) {
		struct proc_struct *p = le2proc(le, run_link);
		print_p(p);
		le = list_next(le);
	}
}

static inline void
sched_class_enqueue(struct proc_struct *proc) {
    if (proc != idleproc) {
        sched_class->enqueue(rq, proc);
    }
}

static inline void
sched_class_dequeue(struct proc_struct *proc) {
    sched_class->dequeue(rq, proc);
}

static inline struct proc_struct *
sched_class_pick_next(void) {
    return sched_class->pick_next(rq);
}

static void
sched_class_proc_tick(struct proc_struct *proc) {
    if (proc != idleproc) {
		// cp("tick proc %d left %u\n", proc->pid, proc->time_slice);
        sched_class->proc_tick(rq, proc);
    }
    else {
        proc->need_resched = 1;
    }
}

static struct run_queue __rq;

void
sched_init(void) {
    list_init(&timer_list);
	
    sched_class = &default_sched_stride_class;

    rq = &__rq;
    rq->max_time_slice = MAX_TIME_SLICE;
    sched_class->init(rq);

    cprintf("sched class: %s\n", sched_class->name);
}

void
wakeup_proc(struct proc_struct *proc) {
    assert(proc->state != PROC_ZOMBIE);
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (proc->state != PROC_RUNNABLE) {
            proc->state = PROC_RUNNABLE;
            proc->wait_state = 0;
            if (proc != current) {
                sched_class_enqueue(proc);
            }
        }
        else {
            warn("wakeup runnable process.\n");
        }
    }
    local_intr_restore(intr_flag);
}

void
schedule(void) {
    bool intr_flag;
    struct proc_struct *next;
    local_intr_save(intr_flag);
    {
        current->need_resched = 0;
        if (current->state == PROC_RUNNABLE) {
            sched_class_enqueue(current);
        }
        if ((next = sched_class_pick_next()) != NULL) {
            sched_class_dequeue(next);
        }
        if (next == NULL) {
            next = idleproc;
        }
        next->runs ++;
        if (next != current) {
            proc_run(next);
        }
    }
    local_intr_restore(intr_flag);
}

// add timer to timer_list
void
add_timer(timer_t *timer) {
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        assert(timer->expires > 0 && timer->proc != NULL);
        assert(list_empty(&(timer->timer_link)));
        list_entry_t *le = list_next(&timer_list);
        while (le != &timer_list) {
            timer_t *next = le2timer(le, timer_link);
            if (timer->expires < next->expires) {
                next->expires -= timer->expires;
                break;
            }
            timer->expires -= next->expires;
            le = list_next(le);
        }
        list_add_before(le, &(timer->timer_link));
    }
    local_intr_restore(intr_flag);
}

// del timer from timer_list
void
del_timer(timer_t *timer) {
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (!list_empty(&(timer->timer_link))) {
            if (timer->expires != 0) {
                list_entry_t *le = list_next(&(timer->timer_link));
                if (le != &timer_list) {
                    timer_t *next = le2timer(le, timer_link);
                    next->expires += timer->expires;
                }
            }
            list_del_init(&(timer->timer_link));
        }
    }
    local_intr_restore(intr_flag);
}

// call scheduler to update tick related info, and check the timer is expired? If expired, then wakup proc
void
run_timer_list(void) {
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        list_entry_t *le = list_next(&timer_list);
        if (le != &timer_list) {
            timer_t *timer = le2timer(le, timer_link);
            assert(timer->expires != 0);
            timer->expires --;
            while (timer->expires == 0) {
                le = list_next(le);
                struct proc_struct *proc = timer->proc;
                if (proc->wait_state != 0) {
                    assert(proc->wait_state & WT_INTERRUPTED);
                }
                else {
                    warn("process %d's wait_state == 0.\n", proc->pid);
                }
                wakeup_proc(proc);
                del_timer(timer);
                if (le == &timer_list) {
                    break;
                }
                timer = le2timer(le, timer_link);
            }
        }
        sched_class_proc_tick(current);
    }
    local_intr_restore(intr_flag);
}
