package com.mycompany.myapp.controller;

import com.mycompany.myapp.service.IUploadService;
//import com.mycompany.myapp.vo.Chunk;
import lombok.extern.slf4j.Slf4j;
import org.apache.tomcat.util.http.fileupload.FileUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;


@RequestMapping("/upload")
@RestController
@Slf4j
@CrossOrigin
public class UploadController {

    //private final static String CHUNK_FOLDER = "/Users/yangwei/resource/data/chunk";
    private final static String SINGLE_FOLDER = "E:\\work\\resource";

    @Autowired
    private IUploadService uploadService;

//    @PostMapping("single")
//    public void singleUpload(Chunk chunk) {
//        MultipartFile file = chunk.getFile();
//        String filename = chunk.getFilename();
//        try {
//            byte[] bytes = file.getBytes();
//            if (!Files.isWritable(Paths.get(SINGLE_FOLDER))) {
//                Files.createDirectories(Paths.get(SINGLE_FOLDER));
//            }
//            Path path = Paths.get(SINGLE_FOLDER,filename);
//            Files.write(path, bytes);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }

    @ResponseBody
    @RequestMapping("/single")
    public void uploadCategory(HttpServletRequest request,
                               @RequestParam("file") MultipartFile[] file){
    if (file != null && file.length > 0) {
        for (MultipartFile temp : file) {
            try {
                byte[] bytes = temp.getBytes();
                if (!Files.isWritable(Paths.get(SINGLE_FOLDER))) {
                    Files.createDirectories(Paths.get(SINGLE_FOLDER));
                }
                Path path = Paths.get(SINGLE_FOLDER,temp.getOriginalFilename());
                Files.write(path, bytes);
                Path solution_path=Paths.get(SINGLE_FOLDER,temp.getOriginalFilename()+"_solution.txt");
                Files.write(solution_path,bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
            }
        }
    }


    /**
    @GetMapping("chunk")
    public Map<String, Object> checkChunks(Chunk chunk) {
        return uploadService.checkChunkExits(chunk);
    }

    @PostMapping("chunk")
    public Map<String, Object> saveChunk(Chunk chunk) {
        MultipartFile file = chunk.getFile();
        Integer chunkNumber = chunk.getChunkNumber();
        String identifier = chunk.getIdentifier();
        byte[] bytes;
        try {
            bytes = file.getBytes();
            Path path = Paths.get(generatePath(CHUNK_FOLDER, chunk));
            Files.write(path, bytes);
        } catch (IOException e) {
            e.printStackTrace();
        }
        Integer chunks = uploadService.saveChunk(chunkNumber, identifier);
        Map<String, Object> result = new HashMap<>();
        if (chunks.equals(chunk.getTotalChunks())) {
            result.put("message","上传成功！");
            result.put("code", 205);
        }
        return result;
    }
    **/
}
