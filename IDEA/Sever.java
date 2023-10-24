//import java.io.*;
//import java.net.ServerSocket;
//import java.net.Socket;
//import java.util.Scanner;
//
//public class Sever {
//    ServerSocket socket;
//
//    public static void main(String[] agrs) throws IOException {
//        Sever sever = new Sever();
//        //将根据端口号申请一个socket 托管到一个类里面
//        System.out.println("端口号已申请重构");
//        sever.start();
//    }
//
//    public void start() throws IOException {
//        System.out.println("服务端已启动");
//        Socket socket =  socket.accept();
//        //获取数据的前提是获得IO流 输入流
//        InputStream inputStream = socket.getInputStream();
//        InputStreamReader inputStreamReader = new InputStreamReader(inputStream,"utf-8");
//        // 用于提高效率 服务端只能被动的听
//        BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
//        // 有一个内部的缓冲区 按行读取
//        String line = null;
//        while( (line = bufferedReader.readLine()) != null  ){
//            System.out.println("[Sever]" + line);
//        }
//    }
//
//}
