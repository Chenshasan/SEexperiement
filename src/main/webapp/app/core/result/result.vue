<template>
    <div id="result">
        <div class="jumbotron masthead">
            <div class="container">
                <h1>XAnalysis</h1>
                <h2>开始查看您的代码分析结果。</h2>
            </div>
        </div>
        <div class="jumbotron1 masthead1">
            <div class="container">
                <h2>数组越界</h2>

                <pre v-highlightjs=codeDate ><code class="c++">
{{this.codeContent}}
                </code></pre>
                <hr>
            </div>
        </div>
    </div>
</template>

<script>
    import $ from 'jquery'

    export default {
        name: "Result",
        data() {
            return{
                codeContent:'',
                warningList:[],
                warn:'',
                index:0,
            }
        },
        created(){
            this.codeContent=this.$store.getters.content
            this.warningList=this.$store.getters.warning.split("\n")
            console.log(this.warningList)
        },
        mounted() {
            $("code").each(function(){
                $(this).html("<ol>" + $(this).html().replace(/\n/g,"\n</li><li>") +"\n</ol>");
            });
            let i=0
            for(i=0;i<this.warningList.length;i++){
                this.warn=this.warningList[i].split(":")
                this.index=parseInt(this.warn[1])
                $("li:nth-child("+this.index+")").css({"background-color":"#dbe2ef"});
            }
    }
    }
</script>

<style scoped>
    #result{
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
        padding: 35px 30px 20px 30px;
    }

    .hljs {
        border: 0;
        font-size: 12px;
        display: block;
        padding: 1px;
        margin: 0;
        width: 100%;
        font-weight: 200;
        color: #333;
        white-space: pre-wrap
    }
    .hljs ol {
        list-style: decimal;
        margin: 0px 0px 0 40px !important;
        padding: 0px;
    }
    .hljs ol li {
        list-style: decimal-leading-zero;
        border-left: 1px solid #ddd !important;
        padding: 5px!important;
        margin: 0 !important;
        white-space: pre;
    }


    .masthead h1 {
        font-size: 70px;
        font-weight:bold;
        margin-bottom: 20px;
        color:#ffffff;
    }

    .btn{
        color: #ffffff;
    }
    .masthead h2 {
        color:#ffffff;
        font-weight:normal;
        margin-bottom:15px;
    }

    .masthead1 h2{
        color: #000000;
        font-size: 28px;
        font-weight: normal;
        margin-bottom: 12px;
    }
    .masthead1 p{
        color: #000000;
    }

    .masthead {
        margin-bottom: 30px;
        color: #fff;
    }

    .masthead1 {
        padding: 0px 0 0px;
        margin-bottom: 0px;
        margin-top: 0px;
        color: #fff;
    }

    .jumbotron {
        position: relative;
        padding: 25px 25px;
        color: #fff;
        text-align: left;
        text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
        border-radius: 0;
        background: #020031;
        background: linear-gradient(45deg,#43d8c9,#035aa6);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,0), inset 0 -3px 7px rgba(255,255,255,0);
    }
    .jumbotron1 {
        position: relative;
        padding: 10px 10px;
        color: #fff;
        text-align: left;
        background: #020031;
        background: linear-gradient(45deg,#fff,#fff);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
    }
</style>
