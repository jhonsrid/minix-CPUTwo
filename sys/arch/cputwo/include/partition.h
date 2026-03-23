/* Description of entry in partition table. */
#ifndef _PARTITION_H
#define _PARTITION_H

#include <stdint.h>

struct part_entry {
  uint8_t bootind;
  uint8_t start_head;
  uint8_t start_sec;
  uint8_t start_cyl;
  uint8_t sysind;
  uint8_t last_head;
  uint8_t last_sec;
  uint8_t last_cyl;
  uint32_t lowsec;
  uint32_t size;
};

#define ACTIVE_FLAG	0x80
#define NR_PARTITIONS	4
#define PART_TABLE_OFF	0x1BE

/* Partition types */
#define NO_PART		0x00
#define MINIX_PART	0x81

#endif /* _PARTITION_H */
