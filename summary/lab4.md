By Object Ho

# Lab 4: Kernel Thread Management #

> Lab 4 is relatively easy. The concepts on kernel thread is quite easy to understand and the doc took me just a little time. The hints are very useful too.

## IDLE process ##

> Inside the `proc_init` function, we can see the initialization of a process list. This lists all processes. Then a process called **idle** is created and the idle process will keeps running if there is no other process.

## INIT process ##

> After IDLE is created, another process call **init** follows, it just prints something before it finishes. 

## Process Allocating ##

> In order to make **idle** and **init** work, we should first finish the `alloc_proc` function. This is not hard, I just follow the hints and everything is done.

## Forking ##

> The `init` process is created using the `kernel_thread` function. But in fact `kernel_thread` works based on `do_fork`. And we should finish `do_fork` in Lab 4.

> Just like process allocating, all I did is following the hints. But I made a mistake this time. Remember

> > **When hashing the process (adding the process into the hash_proc list), the process id is needed.**

> After finishing fork, my codes crashed on an assertion in `check_swap`. I checked it and found it was caused by a mistake in Lab 2. 

> > **I forgot to set page property after creating a page in Lab2.**

## End ##

> I'm looking forward to your advice.
