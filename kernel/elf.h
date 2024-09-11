// Format of an ELF executable file
//nick: xv6 applications are described in the widely-used 
//ELF format described in this file

//nick: an ELF binary starrts with four-byte 
//magic number: 0x7F'E''L''F'
#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

//nick: a ELF binary consists of an ELF header (struct elfdr)
//followed by a sequence of program section headers (struct proghdr)

// File header
struct elfhdr {
  uint magic;  // must equal ELF_MAGIC
  uchar elf[12];
  ushort type;
  ushort machine;
  uint version;
  uint64 entry;
  uint64 phoff;
  uint64 shoff;
  uint flags;
  ushort ehsize;
  ushort phentsize;
  ushort phnum;
  ushort shentsize;
  ushort shnum;
  ushort shstrndx;
};

// Program section header
struct proghdr {
  uint32 type;
  uint32 flags;
  uint64 off;
  uint64 vaddr;
  uint64 paddr;
  uint64 filesz;
  uint64 memsz;
  uint64 align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4
