#ifndef __SPARC_PCI_H
#define __SPARC_PCI_H

/* Can be used to override the logic in pci_scan_bus for skipping
 * already-configured bus numbers - to be used for buggy BIOSes
 * or architectures with incomplete PCI setup by the loader.
 */
#define pcibios_assign_all_busses()	0

#define PCIBIOS_MIN_IO		0UL
#define PCIBIOS_MIN_MEM		0UL

#endif /* __SPARC_PCI_H */
