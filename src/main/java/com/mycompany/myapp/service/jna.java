package com.mycompany.myapp.service;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class jna {
//+(Platform.isWindows() ? ".dll" : ".so")

    public interface CLibrary extends Library {
        CLibrary INSTANCE = (CLibrary)
                Native.loadLibrary("Hello.dll",
                        CLibrary.class);

        int inc(int a);
    }

    public static void main(String[] args) throws IOException {
        //Process process =Runtime.getRuntime().exec("E:\\work\\resource\\JNAtest\\src\\main\\java\\Hello.exe");
        try {
            Process pr  = Runtime.getRuntime().exec("E:\\work\\resource\\JNAtest\\src\\main\\java\\Hello.exe");//之用Rumtime执行一个cmd命令，返回一个Process对象
            //获取执行命令进程的响应信息
            BufferedReader br = new BufferedReader(new InputStreamReader(pr.getInputStream()));
            while(true){
                //读取响应信息
                String s = br.readLine();
                if(s==null){
                    break;
                }
                System.out.println(s);
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        //process.waitfor();
        //System.out.print(CLibrary.INSTANCE.inc(100));
//        for (int i=0;i < args.length;i++) {
//            CLibrary.INSTANCE.inc("Argument %d: %s\n", i, args[i]);
//        }
    }
    //Process process =Runtime.getRuntime().exec("E:\\work\\resource\\JNAtest\\src\\main\\java\\Hello.exe");
//        try {
//        Process pr  = Runtime.getRuntime().exec("E:\\work\\resource\\JNAtest\\src\\main\\java\\Hello.exe");//之用Rumtime执行一个cmd命令，返回一个Process对象
//        //获取执行命令进程的响应信息
//        BufferedReader br = new BufferedReader(new InputStreamReader(pr.getInputStream()));
//        while(true){
//            //读取响应信息
//            String s = br.readLine();
//            if(s==null){
//                break;
//            }
//            System.out.println(s);
//        }
//    } catch (IOException e) {
//        // TODO Auto-generated catch block
//        e.printStackTrace();
//    }
//
//    //process.waitfor();
//    //System.out.print(CLibrary.INSTANCE.inc(100));
////        for (int i=0;i < args.length;i++) {
////            CLibrary.INSTANCE.inc("Argument %d: %s\n", i, args[i]);
////        }
//}
}
