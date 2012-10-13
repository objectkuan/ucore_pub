By Object Ho

# Lab 6: Scheduling #

> Lab 6 is quite interesting because we just need to focus on the algorithm. The **Timer** part in the doc may be useless in this lab if you only want to finish it. But we do need to concern about the **Run Queue** and the structured scheduler. The RR scheduler can work once we add in the codes from Lab 1~4. And we need to implement the **Stride Scheduler**

## Stride Scheduler ##

> We should just read the referenced paper and know how how **Stride Scheduler** works. We can also just read the contents in the doc.

> > **The comparision and overflow issue on the priority are very important. And we can archieve it in many ways.**

> After finishing this, Stride Scheduler based on Priority Queued is quite easy to implemented.

## Mistakes Discovered ##

> The task is quite easy but I came across two mistakes. The first one is that I couldn't cound any code to wake up a process.

> > **This is because in proc.h in Lab 4 I use** `proc->state = PROC_RUNNABLE` **but I should use** `wakeup_proc(proc)` **.**

> Another one is that I cannot get the ratio 1:2:3:4:5 in the priority.c applicaiton.

> I check this issue for long and found that 

> > **I should set a larger running time to make the ratio converge more accurate.**

## End ##

> I'm looking forward to your advice.
