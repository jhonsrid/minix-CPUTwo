#ifndef _CPUTWO_VMPARAM_H_
#define _CPUTWO_VMPARAM_H_

/* VM parameters for CPUTwo Sv32 */
#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#define PAGE_MASK   (PAGE_SIZE - 1)

/* User virtual address space: 0 to 0x03EFFFFF (below MMIO) */
#define VM_MIN_ADDRESS      ((vaddr_t) 0x00000000)
#define VM_MAXUSER_ADDRESS  ((vaddr_t) 0x03E00000)
#define VM_MAX_ADDRESS      ((vaddr_t) 0x03EFFFFF)

#define USRSTACK            VM_MAXUSER_ADDRESS

#endif /* _CPUTWO_VMPARAM_H_ */
