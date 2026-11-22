#ifndef SBI_H
#define SBI_H

#define SBI_BASE_EXTENTION 0x10 //EID

#define DBCN 0x4442434E //EID

#define SUSP 0x53555350 //EID

#define HSM  0x48534D  //EID

#define SBI_TIME 0x54494D45 //EID

#define UART_PUTC_EID 0x01
#define UART_GETC_EID 0x02


#define SBI_SHUTDOWN_EID 0x08

typedef enum : long {
  SBI_SUCCESS                  =  0,
  SBI_ERR_FAILED               = -1,
  SBI_ERR_NOT_SUPPORTED        = -2,
  SBI_ERR_INVALID_PARAM        = -3,
  SBI_ERR_DENIED               = -4,
  SBI_ERR_INVALID_ADDRESS      = -5,
  SBI_ERR_ALREADY_AVAILABLE    = -6,
  SBI_ERR_ALREADY_STARTED      = -7,
  SBI_ERR_ALREADY_STOPPED      = -8,
  SBI_ERR_NO_SHMEM             = -9
} sbi_error_t;

typedef struct {
  unsigned long a0;
  unsigned long a1;
  unsigned long a2;
  unsigned long a3;
  unsigned long a4;
  unsigned long a5;
  unsigned long FID;
  unsigned long EID; 
} sbi_args_t;

typedef struct{
  long error;
  long value;
} sbi_ret_t;

inline long return_val(sbi_ret_t sbi_ret_t);
inline long return_error(sbi_ret_t sbi_ret_t);
extern sbi_ret_t sbi_call(sbi_args_t* a);


#endif