#include "SpiderProgramScheduler.h"
#include "spider.h"
#include "confparser.h"
#include "dso.h"
#include "socket.h"
ConfigParser* ConfigParser::__self = NULL;//放在confparser.h会出现重复定义，注意，static是在编译阶段就分配内存
SpiderProgramScheduler::SpiderProgramScheduler()
{

}

SpiderProgramScheduler::~SpiderProgramScheduler()
{

}

/*
        3.1、通过参数控制是否以守护进程模式运行
//      3.2、提供帮助信息暂时不做
		3.3、读取配置文件，提取参数
		3.4、载入程序动态库
	*/

int SpiderProgramScheduler::init(int argc,char* argv[])
{
	//处理参数,例如守护进程标识,赋值到isDaemon
	handleArgv(argc,argv);
	//读取配置文件
	ConfigParser* cp = ConfigParser::instance();
	cp->loader(CONF_PATH);

	//处理是否以守护进程运行
	if (isDaemon == true)
	{
		this->daemon();
	}

	//载入动态模块,完成
	char *module_path = cp->getModulePath();
    	DsoManager *dso_manager = new DsoManager();


	list<string> module_name = cp->getModuleNames();
	itor = module_name.begin();
	while(itor!=module_name.end())
	{
		dso_manager->load(module_path,*itor)
		itor++;
	}
}

/*开始主业务流程*/
/*
    4.1、将URL种子交给URL管理器。
    4.2、分析种子，得到URL IP地址（DNS解析）
    4.3、根据URL获取第一个页面资源
    4.4、对页面进行处理
        4.4.1、解析页面取得URL
        4.4.2、生成URL列表，交给URL管理器
        4.4.3、对页面进行持久化操作
    4.5、
    4.6、通过调用epoll框架产生新任务（先检测是否到达最大任务数，功能封装在独立的函数，循环添加，直到epoll监控的最大值）
        4.6.1、从URL管理器中取出一个URL
        4.6.2、利用URL的ip地址请求资源，将返回的socket句柄加入epoll监控树
    4.7、通过epoll_wait()监控事件，触发则，创建线程处理接收的数据（对页面进行处理4.4）	一直在这个地方循环，直到url处理队列已经清空
*/

int SpiderProgramScheduler::run()
{
	
	//等待相关管理类的实现
	//4.1、将URL种子交给URL管理器。
	ConfigParser* cp = ConfigParser::instance();
	char *seed = cp->getUrlSeed();
	UrlManager *url_manager = new UrlManager();
	url_manager->addUrl(seed);
	StructUrl *p_url = NULL;
	p_url = url_manager->getUrlFromQuque();
	Socket *socket = new Socket();
	socket->bulidConnect(p_url,PORT);
	socket->setNonblocking(socket->m_socket_handle);
	socket->request(socket->m_socket_handle,p_url);



	socket->responce(socket->m_socket_handle);
	socket->disConnect(socket->m_socket_handle);
}

int SpiderProgramScheduler::daemon()
{

}

