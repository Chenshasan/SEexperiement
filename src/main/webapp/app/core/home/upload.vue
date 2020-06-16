<template>
    <div>
        <div class="uploadDiv1">
      <uploader
        ref="uploader"
        :options="uploadOptions1"
        :autoStart="true"
        @file-added="onFileAdded1"
        @file-success="onFileSuccess1"
        @file-progress="onFileProgress1"
        @file-error="onFileError1"
        class="uploader-app"
      >
        <uploader-unsupport></uploader-unsupport>
        <uploader-drop>
          <uploader-btn style="margin-right:20px;" :attrs="attrs">选择文件</uploader-btn>

          <uploader-btn :attrs="attrs" directory>选择文件夹</uploader-btn>
        </uploader-drop>
        <uploader-list></uploader-list>
      </uploader>
    </div>
    </div>
</template>

<script>
import { merge } from "../api/upload";

export default{
    name: "Upload",
    data() {
    return {
      panelShow: false, //选择文件后，展示上传panel
      collapse: false,
      files: [],
      uploadOptions1: {
        target: "//localhost:8080/upload/single",//上传的接口
        testChunks: false, //是否开启服务器分验
        fileParameterName: "file",//默认的文件参数名
        headers: {},
        query() {},
        categaryMap: { //用于限制上传的类型
          document: ["gif", "jpg", "jpeg", "png", "bmp","cpp"],
        }
      },
      attrs: {
      },
    }
  },

  methods: {
    onFileAdded1(file) {
      console.log(file);
    },
    onFileProgress1(rootFile, file, chunk) {},
    onFileSuccess1(rootFile, file, response, chunk) {
    },
    onFileError1(rootFile, file, response, chunk) {
    },
    onFileAdded2(file) {
      console.log(file);
    },
    onFileProgress2(rootFile, file, chunk) {},
    onFileSuccess2(rootFile, file, response, chunk) {
      let res = JSON.parse(response);
      if (res.code == 1) {
        return;
      }
      if (res.code == 205) {
        const formData = new FormData();
        formData.append("identifier", file.uniqueIdentifier);
        formData.append("filename", file.name);
        merge(formData).then(response => {});
      } else {
      }
    },
    onFileError2(rootFile, file, response, chunk) {
      this.$message({
        message: response,
        type: "error"
      });
    }
  }


}
</script>
