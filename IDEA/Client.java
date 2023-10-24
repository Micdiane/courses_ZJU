//import javax.naming.ldap.SortKey;
//import java.io.*;
//import java.net.ServerSocket;
//import java.net.Socket;
//import java.util.Scanner;
//
//public class Client {
//    public Sever(){
//        try {
//            serverSocket = new ServerSocket(8088);
//        }catch (IOException e){
//            throw new RuntimeException(e);
//        }
//    }
//
//    public Client() throws IOException {
//        Socket socket = new Socket("localhost",8088);
//    }
//
//    public void start() throws IOException { // 就是一个线程 CPU执行
//        OutputStream outputStream = socket.getOutputStream();
//        OutputStreamWriter outputStreamWriter = new OutputStreamWriter(outputStream,"utf-8");
//        BufferedWriter bufferedWriter = new BufferedWriter(outputStreamWriter);
//        // 自动输出且按行刷新,
//        PrintWriter printWriter = new PrintWriter(bufferedWriter,true);
//         // printWriter 写的内容来自于键盘
//        Scanner scanner = new Scanner(System.in);
//        while (true){
//            String message = scanner.nextLine();
//            printWriter.println(message);
//        }
//    }
//
//    public static void main(String[] agrs){ // 主线程 其他线程都是在主线程基础之上生产
//        Sever sever = new Sever();
//        //将根据端口号申请一个socket 托管到一个类里面
//        System.out.println("端口号已申请重构");
//        sever.start();
//    }
//}
