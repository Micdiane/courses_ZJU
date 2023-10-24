public class ThreadDemo1 {
    public static void main(String[] args)
    {
        Thread t1 = new MyThread1();
        Thread t2 = new MyThread2();

        // 线程并发
//        t1.start();
//        t2.start();

        // 严格的顺序执行
        t2.run();
        t1.run();


    }
}

// 定一个类 继承thread 重写run方法
// run方法中定义并发执行的代码
class MyThread1 extends Thread{
    @Override
    public void run() {
        for(int i = 0 ; i< 1000 ; i ++){
            System.out.println("我是线程1");
        }
    }
}

class MyThread2 extends Thread{
    @Override
    public void run() {
        for(int i = 0 ; i< 1000 ; i ++){
            System.out.println("我是线程2");
        }
    }
}

