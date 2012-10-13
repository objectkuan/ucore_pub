By Object Ho

# Lab 5: User Process Management #

> Lab 5 is also very easy. User process is easier to understand compared to the kernel thread. This lab prepare the kernel, and then it will run a certain user application. All we concern about is how a user process is created and executed. The doc tells us a lot of details but I discovered that I can quickly get down to work with the comment hints.

## load_icode ##

> The `load_icode` function is used to load the ELF program to the process space. We need to set the trapframe at the end of the function. 

## Process and Process ##

> Since we have more than one process now. We need to consider two issues. The first one is the relations between them. The second one is how to schedule them.

> As for the first one. Lab 5 adds in `cptr`, `yptr`, `optr` and `parent`. One of our tasks is to initialize them.

> And the second one need a `wait_state`.

> After adding in the relation, we should set their value when foking a new process in `do_fork`. This can be done directly (for `parent`) and using `set_links` (for `cptr`, `yptr` and `optr`).

> The next step is about scheduling. Every TICK\_NUM circle, we should set the `need_resched` value. This simply contributes to the scheduling by time.

## Running the User App ##

> The user applications are used to check the correctness of the lab. My code couldn't pass **spin** and **waitkill** sometimes. And I think

> > **This may be caused by some timeout setting in the grade.sh**

## End ##

> I'm looking forward to your advice.
