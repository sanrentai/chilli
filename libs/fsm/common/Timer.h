#ifndef _COMMON_TIMER_HEADER_
#define _COMMON_TIMER_HEADER_
#include <queue>
#include <sys/timeb.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <map>

#ifndef INFINITE
#define  INFINITE  (0xFFFFFFFF)
#endif

//thread data Including thread handle and thread id

namespace helper{

	//定时器超时通知接口

	typedef void(*OnTimerExpiredFunc)(unsigned long timerId, const std::string & attr, void * userdata);
	// 定时器
	class TimerServer{
	private:
		//定时器对象。
		class  Timer{
		public:
			//生成一个定时器对象，时间间隔，定时器id，
			Timer(unsigned long timerId, unsigned long interval, const std::string & attr, OnTimerExpiredFunc fun, void * userdata) :
				m_nTimerId(timerId), m_attr(attr), m_interval(interval), m_TimeOutFuc(fun), m_userdata(userdata)
			  {
				  ftime(&m_startTime);
			  }
			  virtual ~Timer(){};

			  time_t getInterval() const{
				  struct timeb currTime;
				  ftime(&currTime);
				  time_t  i = m_interval - ((currTime.time-m_startTime.time)*1000 + currTime.millitm-m_startTime.millitm);
				  return i > 0?i:0;
			  };
			unsigned long getTimerId(){return m_nTimerId;};
			const std::string & getAttr(){ return m_attr;};
			void * getUserData() { return m_userdata; };
			OnTimerExpiredFunc getFunction() { return m_TimeOutFuc; };
		private:
			const unsigned long m_nTimerId;
			const std::string m_attr;
			const unsigned long m_interval;
			struct timeb m_startTime;
			OnTimerExpiredFunc m_TimeOutFuc = nullptr;
			void * m_userdata;
			Timer & operator=( const Timer & ) = delete;
		};
		class TimerComp{
		public:
			bool operator() (const Timer * const A, const Timer * const B)
			{
				return (A->getInterval()  > B->getInterval());
			}

		};
	private:
		//定时器队列
		typedef std::priority_queue<Timer *,std::vector<Timer *>, TimerComp> TIMER_QUEUE;
		TIMER_QUEUE m_timer; 
		std::map<unsigned long, unsigned long> m_rvtimer;

		std::mutex m_mtx;
		std::atomic_bool m_Running;
		std::condition_variable m_cv;
		std::thread m_thread;
	public:
		TimerServer():m_Running(false){
			
		}

		void Start()
		{
			m_Running = true;
			this->m_thread = std::thread(&TimerServer::run, this);
		}

		void Stop() {
			m_Running = false;
			if (m_thread.joinable()) {
				m_cv.notify_one();
				m_thread.join();
			}
		}
		virtual ~TimerServer(){
			Stop();
		}
	private:

		unsigned long GenerateTimerID()
		{
			static unsigned long index = 0;
			index++;
			if (index == 0 || index == 0xFFFFFFFF)
				index = 1;
			return index;
		}

		void run(){
			time_t millisec = INFINITE;

			while(m_Running)
			{
				std::unique_lock<std::mutex> lck(m_mtx);

				std::cv_status stauts = this->m_cv.wait_for(lck, std::chrono::milliseconds(millisec));

				if (stauts == std::cv_status::timeout) {
					while (!this->m_timer.empty()) {
						Timer * timer = this->m_timer.top();

						millisec = timer->getInterval();
						//LOG4CPLUS_DEBUG(log,"timer event . interval=" << millisec );
						if (millisec > 0) {
							break;
						}

						if (m_rvtimer.find(timer->getTimerId()) == m_rvtimer.end()) {
							if (timer->getFunction() != nullptr)
								timer->getFunction()(timer->getTimerId(), timer->getAttr(), timer->getUserData());
						}
						else
							m_rvtimer.erase(timer->getTimerId());

						this->m_timer.pop();
						delete timer;

						millisec = INFINITE;
					}
				}
				else {
					if (!this->m_timer.empty()){
						millisec = this->m_timer.top()->getInterval();
					}
					else {
						millisec = INFINITE;
					}
				}
			}
		}

	public:
		unsigned long SetTimer(unsigned long delay, const std::string & attr, OnTimerExpiredFunc func, void * userdata){

			unsigned long timerId = GenerateTimerID();
			Timer * _timer = new Timer(timerId, delay, attr, func, userdata);
			std::unique_lock<std::mutex> lck(m_mtx);
			m_timer.push(_timer);
			m_cv.notify_one();
			return timerId;
		}

		void RemoveTimer(unsigned long timerID){

			std::unique_lock<std::mutex> lck(m_mtx);
			m_rvtimer.insert(std::make_pair(timerID, timerID));
		}
	};

}//end namespace helper
#endif//end timer header