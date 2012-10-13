By Object Ho
# Lab 1 - Boot/Protect Mode/Stack/Interrupt #

> This is the first lab that I should read the Makefile and the codes of ucore. Since I have blank knowledge of how to write, compile and run an OS, I'm quite cautious about everything related to the lab. I read through almost every line of the Makefile and every other file.

## Booting ##

> The first part is ucore's booting. Before I read the doc, I knew that the OS is a piece of codes stored inside the machine. And I knew that after powered on the CPU will get the codes from the addresses. Also in the process, I knew the BIOS may do something too.

> After reading all the Makefile, I have some questions but it doesn't matter because I can still understand how the OS is compiled and linked.

> And I tried to trace the codes, I learned the basic knowledge of gdb and qemu monitor. It's quite easy in fact.

> In Secion 3 of the doc, I get more details about the process above. This part tells me after powered on, the CS:IP is OxF000:0xFFF0, so the CPU starts processing from the address 0xFFFF0.

> > **Here I have a question, based on CS:IP I get the address 0xFFFF0, but the doc says the CPU starts from 0xFFFFFFF0. And when I traced into the code, I found the two addresses are not the same.**

> Anyway, there's no doubt that the CPU starts from an address with many Fs. And stored in that address there's a jumping instruction, which leads the CPU to the beginning of the BIOS routine. The BIOS finishes its tasks and then processes on from 0x7C00, where the bootloader was stored when the OS source codes were linked.

> Here comes the bootloader. It starts in 16-bit real mode and then enables address line 20. And it then load GDT, and switches to 32-bit protected mode.

> > **Then I have a question, is there 16-bit protected mode or 32-bit real mode?**

> After switching to protected mode, it enters the function bootmain in C codes. It reads ucore kernel from disk and stores it in RAM. There is a piece of codes like that in the function readsect() in bootmain.c:

	// wait for disk to be ready
	waitdisk();`
	
	outb(0x1F2, 1);                         // count = 1
	outb(0x1F3, secno & 0xFF);
	outb(0x1F4, (secno >> 8) & 0xFF);
	outb(0x1F5, (secno >> 16) & 0xFF);
	outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
	outb(0x1F7, 0x20);                      // cmd 0x20 - read sectors
	
	// wait for disk to be ready
	waitdisk();

> First booloader waits for the disk until it's free. Then it set the parameters.

> > **Then I have a question, once the disk is free, `waitdisk()` returns and it reaches outb. Can such situation exist?**


> > * The disk is free, then `waidisk()` returns.
> > * Before reaching `outb(0x1F2, 1);` the disk is busy again.
> > * When reaching `outb(0x1F2, 1);` the disk is still busy now.

> I can suppose this never appears. And I can understand that it reads from disk section after section.

> Then the doc tells us the histories and differences between Real Mode and Protected Mode. I think in the Segment Storage Management, the most important concepts are the segment registers, and the content inside them, which is calld a segment desciptor. The descriptor is seperated into two parts - the selector and the offset. One more important thing in this mechanism is the segment descriptor table. With the selector, we can get the base address of a segment. And then with the offset, we can locate the memory.

> Also inside the desciptor, there is not only the address information, it also has many other values, but I hardly use them when I coded in the lab. It seems that I didn't use anything related to the Priviledge Level or the ELF format either.

> The above may be all that I concern when I study the booting process of ucore.

## Stack ##

> After studying the booting, the next task is something about stack. According to the hint comments in the function `print_stackframe()`, it's quite easy to finish this part. But I need to understand the structure of the function calling stack well. I did it, but after I finish the task, I forget it soon.

## Interupt ##

> This part is quite difficult for me because I always think it involves many issues on hardware.

> To finish this part, the most important is to study the use of a macro `SETGATE`, and I should make clear how the arrays `idt` and `__vectors` work. It's good to analyze the structure of IDT gate descriptors before reading the codes of SETGATE. But after I finish this part, I still can't understand the second parameter of `SETGATE`.

> > **I tried to change the parameter `istrap` at will, and the OS runs as normal.**

> This is why I still don't understand what are the differences among the three types of interrupts.

> Also, my codes set up the IDT and load it, but I still don't know how IDT will be used, of course I'm not familiar with the issues on CPL and DPL.


## END ##

> I'm looking forward to your advice.
