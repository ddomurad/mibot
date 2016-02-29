#ifndef __UTILS_H
#define __UTILS_H
#include "config.h"
#include "HardwareSerial.h"

#define ERR( code, err )  __handle_error( err,code, __LINE__, __FUNCTION__ );

#ifdef ENABLE_TRACE
  #define TRACE_PRT( msg )     __trace_prt(msg, __LINE__, __FUNCTION__ );
  #define TRACE_LINE( ... )    __trace_prt(#__VA_ARGS__, __LINE__, __FUNCTION__ );  __VA_ARGS__;
  #define TRACE_INT( ... )     __trace_expr<int>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE_STR( ... )     __trace_expr<char*>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE_FLOAT( ... )   __trace_expr<float>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE_CHAR( ... )    __trace_expr<char>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
#else
  #define TRACE_PRT( msg )
  #define TRACE_LINE( ... )   __VA_ARGS__;
  #define TRACE_INT( ... )    __VA_ARGS__;
  #define TRACE_STR( ... )    __VA_ARGS__;
  #define TRACE_FLOAT( ... )  __VA_ARGS__;
  #define TRACE_CHAR( ... )   __VA_ARGS__;
#endif

#ifdef ENABLE_TRACE2
  #define TRACE2_PRT( msg )     __trace_prt(msg, __LINE__, __FUNCTION__ );
  #define TRACE2_LINE( ... )    __trace_prt(#__VA_ARGS__, __LINE__, __FUNCTION__ );  __VA_ARGS__;
  #define TRACE2_INT( ... )     __trace_expr<int>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE2_STR( ... )     __trace_expr<char*>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE2_FLOAT( ... )   __trace_expr<float>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
#else
  #define TRACE2_PRT( msg )
  #define TRACE2_LINE( ... )   __VA_ARGS__;
  #define TRACE2_INT( ... )    __VA_ARGS__;
  #define TRACE2_STR( ... )    __VA_ARGS__;
  #define TRACE2_FLOAT( ... )  __VA_ARGS__;
#endif

#ifdef ENABLE_TRACE3
  #define TRACE3_PRT( msg )     __trace_prt(msg, __LINE__, __FUNCTION__ );
  #define TRACE3_LINE( ... )    __trace_prt(#__VA_ARGS__, __LINE__, __FUNCTION__ );  __VA_ARGS__;
  #define TRACE3_INT( ... )     __trace_expr<int>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE3_STR( ... )     __trace_expr<char*>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE3_FLOAT( ... )   __trace_expr<float>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
  #define TRACE3_CHAR( ... )    __trace_expr<char>(#__VA_ARGS__,__VA_ARGS__, __LINE__, __FUNCTION__ );
#else
  #define TRACE3_PRT( msg )
  #define TRACE3_LINE( ... )   __VA_ARGS__;
  #define TRACE3_INT( ... )    __VA_ARGS__;
  #define TRACE3_STR( ... )    __VA_ARGS__;
  #define TRACE3_FLOAT( ... )  __VA_ARGS__;
  #define TRACE3_CHAR( ... )   __VA_ARGS__;
#endif

void InitGSerial(HardwareSerial* s, int buad);
HardwareSerial* GSerial();


void __trace_header(int line, const char* funct);
void __handle_error(char * msg, int code, int line_num, const char* function);
void __trace_prt(char * msg, int line_num, const char* function);

template <typename T>
void __trace_expr(char * expr, T val, int line_num, const char* function)
{
     __trace_header(line_num, function);
     GSerial()->print(expr);
     GSerial()->print(" = '");
     GSerial()->print(val);
     GSerial()->println("'");
}

template <typename T>
void SendData(const char * n,T val)
{
  GSerial()->print(n);
  GSerial()->print(" ");
  GSerial()->println(val);
}

template <typename T>
void SendData(const char * n,T val1, T val2)
{
  GSerial()->print(n);
  GSerial()->print(" ");
  GSerial()->print(val1);
  GSerial()->print(":");
  GSerial()->println(val2);
}

template <typename T>
void SendData(const char * n,T val1, T val2, T val3)
{
  GSerial()->print(n);
  GSerial()->print(" ");
  GSerial()->print(val1);
  GSerial()->print(":");
  GSerial()->print(val2);
  GSerial()->print(":");
  GSerial()->println(val3);
}


// FLAGS


#define CMD_WRITE '>'
#define CMD_READ '<'
#define CMD_SYNC '!'


#endif
