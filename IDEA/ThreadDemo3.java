public class ThreadDemo3 {
    public static void main (String[] args){
        Thread thread = new Thread(){ // 实现一个匿名类

            @Override
            public void run() {
                for(int i = 0 ; i< 1000 ; i ++){
                    System.out.println("我是Thread匿名内部类");
                }
            }
        };

        //实现Runnable的匿名内部类
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                for(int i = 0 ; i< 1000 ; i ++){
                    System.out.println("我是Runnable匿名内部类");
                }
            }
        };
        Thread thread1 = new Thread(runnable);
        // 启动两种
        thread.start();
        thread1.start();


    }
}
