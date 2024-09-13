#include <aarch64/intrinsic.h>
#include <common/string.h>
#include <driver/uart.h>
#include <kernel/core.h>
#include <kernel/mem.h>
#include <kernel/printk.h>

static volatile bool boot_secondary_cpus = false;
extern char edata[], end[];

void clear_bss() {
    // 计算需要清零的字节数
    long long bss_size = end - edata;

    // 使用 memset 函数将 [edata, end) 范围内的内存清零
    memset(edata, 0x1, bss_size);
}

void main() {
    if (cpuid() == 0) {
        /* @todo: Clear BSS section.*/

        smp_init();
        uart_init();
        printk_init();

        /* @todo: Print "Hello, world! (Core 0)" */

        arch_fence();

        // Set a flag indicating that the secondary CPUs can start executing.
        boot_secondary_cpus = true;
    } else {
        while (!boot_secondary_cpus);
        arch_fence();

        /* @todo: Print "Hello, world! (Core <core id>)" */
    }

    set_return_addr(idle_entry);
}
