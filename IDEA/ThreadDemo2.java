public class ThreadDemo2 {
    public static void main(){

        // 创建线程对象
        Thread thread1 = new Thread(new Thread1());
        Thread thread2 = new Thread(new Thread2());

        thread1.start();;
        thread2.start();
    }
}
/*
    implements Runnable
    Runnable 是一个执行接口 该类实现之后需要实现抽象方法
 */
class Thread1 implements Runnable{
    @Override
    public void run() {
        for(int i = 0 ; i< 1000 ; i ++){
            System.out.println("我是线程1");
        }
    }
}

class Thread2 implements Runnable{
    public void run(){
        for(int i = 0 ; i< 1000 ; i ++){
            System.out.println("我是线程2");
        }
    }
}
