/*
 *  INFO0064 - Embedded systems
 *
 *  Dummy functions for unimplemented system calls.
 *
 */

int _close(void);
int _fstat(void);
int _isatty(void);
int _lseek(void);
int _read(void);
int _write(void);
  
int _close(void)
{
  return -1;
}

int _fstat(void)
{
  return -1;
}

int _isatty(void)
{
  return -1;
}

int _lseek(void)
{
  return -1;
}

int _read(void)
{
  return -1;
}

int _write(void)
{
  return -1;
}
