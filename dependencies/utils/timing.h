/*
* file created by    Markus Steinberger / steinberger ( at ) icg.tugraz.at
*
* modifications by
*/

#ifndef TIMEING_H
#define TIMEING_H

//#include <iostream>
//timing
#ifndef WIN32
	#include <sys/time.h>
#else
  #define NOMINMAX
  #define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <vector>

  class Timing
  {
    //timing

    size_t interpolation;
    size_t filllevel;
    size_t pointer;
    double lastbegin;
    double dts;
    double pausebegin;
    std::vector<double> lasttimings;

    void frame(double dt)
    {
      if(filllevel < interpolation)
        lasttimings[filllevel++] = dt;
      else
      {
        lasttimings[pointer++] = dt;
        pointer %= interpolation;
      }
      dts = 0.0;
      for(size_t i = 0; i < filllevel; ++i)
        dts += lasttimings[i];
      dts/= static_cast<double>(filllevel);
    }

    #if defined(WIN32) && !NO_PERFORMANCE_COUNTER
    static double perfCounterFreq(bool &hasPerf)
    {
      LONGLONG f;
      hasPerf = QueryPerformanceFrequency((LARGE_INTEGER*)&f) != 0;
      return static_cast<double>(f);
    }
    #endif
  public:

    #ifndef WIN32
	    static double gettime(void)
	    {
        struct timeval tv;
		    gettimeofday(&tv,0);
		    return tv.tv_sec*1.0 + (tv.tv_usec / 1000000.0);
	    }
    #else

	    static double gettime(void)
	    {
        #if NO_PERFORMANCE_COUNTER
		    return GetTickCount()/1000.0;
        #else
        static bool has_pc;
        static double pc_freq = perfCounterFreq(has_pc);
        if(has_pc)
        {
          LONGLONG val;
          QueryPerformanceCounter((LARGE_INTEGER*)&val);
          return static_cast<double>(val)/pc_freq;
        }
        else
          return GetTickCount()/1000.0;
        #endif
	    }
    #endif

    Timing(size_t interpolate = 100) : interpolation(interpolate), lasttimings(interpolate), filllevel(0), pointer(0)
    {
      lastbegin = gettime();
    }
    void begin()
    {
      lastbegin = gettime();
    }
    double intermediate()
    {
      return gettime() - lastbegin;
    }
    double end()
    {
      double dt = gettime();
      dt -= lastbegin;
      frame(dt);
      return dt;
    }
    void feedFrame(double time)
    {
      frame(time);
    }
    void pause_begin()
    {
      pausebegin = gettime();
    }
    void pause_end()
    {
      lastbegin += gettime() - pausebegin;
    }
    double frame() { end(); double t = 1.0/dts; begin(); return t; }
    double fps() const {return 1.0/dts; }
    double ms() const {return dts*1000.0; }
  };

#endif
