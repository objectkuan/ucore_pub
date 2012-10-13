By Object Ho

# Lab 7: Mutex/Sync #

> Lab 7 is to solve the **Dining Philosophers Problem** using **Monitor**. In fact I don't understand the concept of Monitor very well from the doc. But the comment hints are very useful and I pass this task following the hints.

## The Monitor ##

> As we can see in the codes, when we use Monitor and want to get access to the Critical Section, we should always remember to process the **into routine** before accessing and **leave routine** after accessing.

> If we follow the hints in monitor.c we can finish the lab in a short time.

> But my codes were stuck at an assertion in proc.c

	assert(nr_free_pages_store == nr_free_pages());

> This is because the condition variable in the monitor need one page of space. And it's initialization violates this assertion.

> > **We should remove this assertion, or free the space of the** `cv` **after all philosophers quit.**

## End ##

> I'm looking forward to your advice.
