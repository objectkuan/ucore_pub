By Object Ho
# Lab 2 - Physical Memory Management #

> This is the second lab. I was told that physical memory management is an extremely important part in OS. And the mechanism is very complex and difficult to understand. When I began to read the doc and the codes, I found that's true. In a word, Lab 2 is a tough task.

## Instruction to Physical Memory Management ##
> First we can learn from the doc that the OS detects the structure of memory with e820h interrupt when it's in Real Mode after booted. Then the result was store in physical address 0x8000. The doc tells us a lot about this issue. But now I forget everything and only know we can get the total memory from the result.

> This is our first step because we can do nothing until we know which part in the memory we can use. Then the doc describes what we use to represent the memory and the useful values related to memory management. The doc introduces the page mechanism. We use a **Page** structure to represent every 4KB of memory which is aligned to 4KB meanwhile. The structure holds the reference counter and the status flags of a page. And according to the `page_link`, we know that the OS has a linked list of Page structures. 

> > **Here I suppose that if a structure has a `list_entry_t` member, we will have a corresponding linked list of such a structure.**

> Later we have a `ppm_manager` (Physical Memory Manager). It is the very tool we adopt to manage the physical memory, which holds the functions like creating and freeing the memory pages. Until now, everything is so easy and I can understand most of the details.

## Paging ##

> Here comes the sexiest part of memory management - the **Paging Mechanism**.

> The mechanism has three addresses which confused me a long time. They are **Virtual Address**, **Linear Address**, and **Physical Address**. **Virtual Address** is also called **Logical Address**.

> The three addresses are very essential to implement combined paging and segmentation. 

> ![Segmentation And Paging Graph](https://raw.github.com/objectkuan/ucore_pub/master/image/wiki-labs/s-p.png)

> I'm not good at comprehending knowledge by reading, but once I see a graph I can quickly figure out what it means. 

> * **Virtual Address** is divided into **Segment Selector** and **Offset**
> * **Segment Selector** and the base address of GDT determine the **Segment Descriptor**
> * The content of **Segment Descriptor** and **Offset** determine the **Linear Address**

> That's the first step, below is the second step:

> * **Linear Address** is divided into **Directory Index**, **Table Index** and **Offset**
> * **Directory Index** and the base address of PDT determine the **Directory Entry**
> * The content of **Directory Entry** and the **Table Index** determine the **Page-Table Entry**
> * The content of **Page-Table Entry** and the **Offset** determine the **Physical Address**

> So now we can get the page content with the **Virtual Address**(**Logical Address**)

> All these can be easily understood. It's just a mapping system. But after that, I reached a disgusting stage. The kernel starts from **Virtual Address** 0xC0100000. While in Lab 1, the kernel started from **Virtual Address** 0x100000. But in fact the kernel in Lab 2 is still stored in **Physical Address** 0x00100000, just the same as Lab 1. (So we know that in Lab 1 all **Virtual Address**es are equal to **Physical Address**es. While in Lab 2, all **Virtual Address**es are equal to **Physical Address**es + 3G). 

> > **After finishing the lab, I find this is very important to understand the following details. Every time we use variables in our codes, they're virtual addresses. But the contents in the table entries are all physical addresses. I name this "VP Principle"**

> > **Before I realized the above principle, I made several mistakes. And after Junjie Mao and Biye Jiang's instruction, they're finally solved.**
> > * **I tried to assign values to variables but failed. (The variables were below the 3G address.)**
> > * **I tried to `memset` a section of memory to zero but failed. (The same cause.)**

> As for the contents of page table directory entries and page table entries, I can imitate what the doc says.

> > **But I don't know why the [Page Table Entry] = pa | PTE_P | PTE_W isn't set the PTE_U bit.**

> After learning how to set the table entries, I thought I'd finished everything. But the most annoying was to come. The function `kern_entry` set a temporary segment mapping. And that makes **Virtual Address** not equal to **Linear Address**. So we should reload GDT.

> > **Then I come up with a questions, why should we have such an annoying period? Can we finish all the jobs without setting such a disgusting mapping?**

> > Anyway, following the **VP Principle** above, I can finish the tasks in Lab 2. And with Lab 2, I have a deeper understanding of the usage of linked lists in Linux.

> > Another interesting part is **Self-Mapping Mechanism**. But we don't need it to finish Lab 2

## End ##
> I'm looking forward to your advice.
