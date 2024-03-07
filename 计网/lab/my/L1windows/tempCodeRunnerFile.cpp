#include <iostream>
#include <process.h>
#include <thread>
using namespace std;

void makethread1(){
	for(int i = 0 ; i < 100 ; i++){
		cout << "t1" << endl;
	}
}

void makethread2(){
	for(int i = 0 ; i < 100 ; i++){
		cout << "t2" << endl;
	}
}

void makethread3(int x){
	for(int i = 0 ; i < 100 ; i++){
		cout << "t3" << x << endl;
	}
}
int main(){
	//当线程启动后，一定要在和线程相关联的thread销毁前，确定以何种方式等待线程执行结束。
	thread t1(makethread1);
	thread t2(makethread2);

	t1.join();
	t2.join();
	
	return 0;
}