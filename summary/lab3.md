By Object Ho

# Lab 3: Virtual Memory Management #

> Lab 2 is confusing enough, and here comes Lab 3. Prof. Chen says Lab 2 and Lab 3 are the most difficult. But since I know the basic principle of virtual memory, I find it easier to understand the doc. The most important thing is to understand the two structures well - **struct vma_struct**, **struct mm_struct**. I'll talk about it later.

## Page Fault ##

> Although the doc introduces the two structures first, but I didn't understand the usage of them until I read to the last of the doc and the codes.

> The Page Fault part of the doc introduces when and how to invoke the fault handler. Based on the work in Lab 1, I can understand this easily when I saw the calling process:

> `trap` --> `trap_dispatch` --> `pgfault_handler` --> `do_pgfault`

## Page Swapping Algorithms ##

> After that the doc shows us several algorithms: **FIFO**, **Clock**, and **Enhanced Clock**. That's not so difficult and more details can be found in the doc.

## Page Swapping Details ##

> The page swapping details are quite easy too. The OS just keeps a buffered area storing pages. And when running, the OS swaps pages in and out. The doc makes the concepts very clear, and I can easily fill the codes in `do_pgfault` according to the comments, but when I started to code the part swapping in and out, I had to analyze the `vma_struct` and `mm_struct` carefully. 

> First I tried to figure out the relationship between them. That day at lunch, Prof. Chen told me that `mm_struct` manages `vma_struct`, then I gained insight. To finish the task, there are two important members in `mm_struct`, a list of `vma_struct` and a list of pages. The former controls access and the latter holds the buffer. And at that time, I have a question.

> > **VMA manages a list of virtual memory address intervals in order. Are they continuous?**

> Later Prof. Chen and Biye Jiang gave me answers about that.

> The `_fifo_map_swappable` function is quite easy, I just need to add the page to the tail of the list. And the in `_fifo_swap_out_victim` I just need to remove the first page from the list. And I find it strange that:

> > **I didn't use the `swap_in` parameter of `_fifo_map_swappable` or the `in_tick` in `_fifo_swap_out_victim`**

> These two parts are not difficult, but in order to master the principle, it's better to read other related codes.

## End ##

> I'm looking forward to your advice.
