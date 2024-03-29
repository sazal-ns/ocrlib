#ifndef CONFIG_H
#define CONFIG_H
#include <time.h>


//#define MACOS
#define WIN32


//#define WIN_VS
//Rem by Vic
#define WIN_BS //BorlandStudio

#define MACPRO

#ifdef MACPRO
//#define HASH_PATH "/Volumes/GOMDE_500Gb/_HASH/"	
#define HASH_PATH "_HASH/"	
#else
//#define HASH_PATH "/Volumes/A_DATA_64Gb/_HASH/"
#endif


#ifdef MACOS
  #undef WIN32_DRAW
  #undef WIN32
  #define LOAD_PTR  LDDQU    //on Mac 
  #define MOV_PTR MOVAPS
#else
  #define LOAD_PTR  MOVDQU    //on Mac LDDQU
  #define MOV_PTR MOVDQU
#endif

#ifdef MACPRO 
#define SITE_ROOT  "/Volumes/DHARMA_750Gb/__LIBRARY/"
#define SITE_NAME "http://localhost/lib/"
#else
#define SITE_ROOT  ""
#define SITE_NAME "http://localhost/"
#endif



#undef OS2
#undef UNIX
#undef DRAW1  // #undef
#undef DRAW_PC

#define DEBUGLVL
//#define DEBUGLVL_GREY
//#define DEBUGLV_RGB
#define PRINT
#define REPORT
//#define REPORT_DRAW



#ifdef REPORT
#define DM(x) //cout<<x;
#define DP(x) //out<<x;
#define DL(x) out<<x;
#define DC(x) //cout<<x;
#define DT(x) if(print)cout<<x;
#define DR(x) cout<<x;
#define DS(x) //cout<<x;
#define DA(x) //if(print)c_out_<<x


#else
#define DM(x)
#define DP(x)
#define DC(x)
#define DT(x)
#define DS(x)
#define DA(x)
#endif

#define END "<br>"<<endl
#define SH(x) ShowMessage(x)
#define OUT_S(_X_) ofstream  out; out.setf(ios::unitbuf); out.open(_X_.c_str());
#define OUT_C(_X_) ofstream  out; out.setf(ios::unitbuf); out.open(_X_);
#define TIME_START clock_t  tm_start, tm_end; double time;  tm_start=clock();
#define TIME_PRINT tm_end=clock(); tm_end-=tm_start; time=tm_end/CLOCKS_PER_SEC;DC("time="<<time<<END);tm_start=clock();
#define TIME_PRINT_ tm_end=clock(); tm_end-=tm_start; time=tm_end/100;cout<<"time="<<(float)time<<END;tm_start=clock();

#define TIFF_FILE 1
#define PICT_SIZE 150

#endif
