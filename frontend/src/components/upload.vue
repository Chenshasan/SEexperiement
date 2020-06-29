<template>
    <div>
        <div id="wrapper" v-if="!this.loading">
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
            <a id="link" style="color: black;font-weight: bold" @click="finish">已完成文件上传</a>
        </div>

        <div id="wrapper1" v-if="this.loading">
            <div class="jumbotron masthead">
                <div class="spinner">
                    <div class="spinner-container container1">
                        <div class="circle1"></div>
                        <div class="circle2"></div>
                        <div class="circle3"></div>
                        <div class="circle4"></div>
                    </div>
                    <div class="spinner-container container2">
                        <div class="circle1"></div>
                        <div class="circle2"></div>
                        <div class="circle3"></div>
                        <div class="circle4"></div>
                    </div>
                    <div class="spinner-container container3">
                        <div class="circle1"></div>
                        <div class="circle2"></div>
                        <div class="circle3"></div>
                        <div class="circle4"></div>
                    </div>
                </div>
                <h3 style="margin-top: 50px;font-size: 40px;font-weight: bold">Loading stuff...</h3>
            </div>
        </div>
    </div>
</template>

<script>
import { merge } from "../api/upload";

export default{
    name: "Upload",
    data() {
    return {
        loading:false,
      panelShow: false, //选择文件后，展示上传panel
      collapse: false,
      files: [],
      uploadOptions1: {
        target: "//localhost:8082/upload/single",//上传的接口
        testChunks: false, //是否开启服务器分验
        fileParameterName: "file",//默认的文件参数名
        headers: {},
        query() {},
        categaryMap: { //用于限制上传的类型
          image: ["gif", "jpg", "jpeg", "png", "bmp","cpp"],
        }
      },
      attrs: {
        accept:  ["gif", "jpg", "jpeg", "png", "bmp","cpp"],
      },
    }
  },

  methods: {
    onFileAdded1(file) {
      console.log(file);
    },
    onFileProgress1() {},
    onFileSuccess1() {
    },
    onFileError1() {
    },
    onFileAdded2(file) {
      console.log(file);
    },
    onFileProgress2() {},
    onFileSuccess2(file, response) {
      let res = JSON.parse(response);
      if (res.code == 1) {
        return;
      }
      if (res.code == 205) {
        const formData = new FormData();
        formData.append("identifier", file.uniqueIdentifier);
        formData.append("filename", file.name);
        merge(formData);
      }
    },
    onFileError2(response) {
      this.$message({
        message: response,
        type: "error"
      });
    },
      sleep (time) {
            return new Promise((resolve) => setTimeout(resolve, time));
      },
      finish() {
          this.loading = true;
          this.sleep(3000).then(() => {
              this.$router.push({name: 'Result'})
          })
      }
  }


}
</script>

<style>
    #wrapper{
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        padding: 100px 30px;
        margin-top: 200px;
    }

    #wrapper1{
        width: 100%;
        height: 800px;
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        margin-top: 50px;
        padding: 45px 35px;
    }

    .jumbotron {
        position: relative;
        padding: 200px 200px;
        height: 100%;
        color: #fff;
        text-align: center;
        text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
        background: #020031;
        background: linear-gradient(45deg,#43d8c9,#035aa6);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
    }

    #link{
        color: rgba(0, 0, 0, 0.99);
    }


    .spinner {
        margin: 30px auto;
        width: 130px;
        height: 130px;
        position: relative;
    }

    .container1 > div, .container2 > div, .container3 > div {
        width: 28px;
        height: 28px;
        background: #ffffff;

        border-radius: 100%;
        position: absolute;
        -webkit-animation: bouncedelay 1.2s infinite ease-in-out;
        animation: bouncedelay 1.2s infinite ease-in-out;
        -webkit-animation-fill-mode: both;
        animation-fill-mode: both;
    }

    .spinner .spinner-container {
        position: absolute;
        width: 100%;
        height: 100%;
    }

    .container2 {
        -webkit-transform: rotateZ(45deg);
        transform: rotateZ(45deg);
    }

    .container3 {
        -webkit-transform: rotateZ(90deg);
        transform: rotateZ(90deg);
    }

    .circle1 { top: 0; left: 0; }
    .circle2 { top: 0; right: 0; }
    .circle3 { right: 0; bottom: 0; }
    .circle4 { left: 0; bottom: 0; }

    .container2 .circle1 {
        -webkit-animation-delay: -1.1s;
        animation-delay: -1.1s;
    }

    .container3 .circle1 {
        -webkit-animation-delay: -1.0s;
        animation-delay: -1.0s;
    }

    .container1 .circle2 {
        -webkit-animation-delay: -0.9s;
        animation-delay: -0.9s;
    }

    .container2 .circle2 {
        -webkit-animation-delay: -0.8s;
        animation-delay: -0.8s;
    }

    .container3 .circle2 {
        -webkit-animation-delay: -0.7s;
        animation-delay: -0.7s;
    }

    .container1 .circle3 {
        -webkit-animation-delay: -0.6s;
        animation-delay: -0.6s;
    }

    .container2 .circle3 {
        -webkit-animation-delay: -0.5s;
        animation-delay: -0.5s;
    }

    .container3 .circle3 {
        -webkit-animation-delay: -0.4s;
        animation-delay: -0.4s;
    }

    .container1 .circle4 {
        -webkit-animation-delay: -0.3s;
        animation-delay: -0.3s;
    }

    .container2 .circle4 {
        -webkit-animation-delay: -0.2s;
        animation-delay: -0.2s;
    }

    .container3 .circle4 {
        -webkit-animation-delay: -0.1s;
        animation-delay: -0.1s;
    }

    @-webkit-keyframes bouncedelay {
        0%, 80%, 100% { -webkit-transform: scale(0.0) }
        40% { -webkit-transform: scale(1.0) }
    }

    @keyframes bouncedelay {
        0%, 80%, 100% {
            transform: scale(0.0);
            -webkit-transform: scale(0.0);
        } 40% {
              transform: scale(1.0);
              -webkit-transform: scale(1.0);
          }
    }
</style>
