#define INTERVAL 1000

class Timer
{
  private:
    unsigned long previousMillis;
    int interval;
    
  public:
    Timer();
    Timer(int);
    bool DoneYet();
    void Mark();
};
