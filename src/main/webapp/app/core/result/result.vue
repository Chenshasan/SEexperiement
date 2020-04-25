<template>
    <div>
        <div class="jumbotron masthead">
            <div class="container">
                <h1>XAnalysis</h1>
                <h2>开始查看您的代码分析结果。</h2>
                <hr>
            </div>
        </div>
        <div class="jumbotron1 masthead">
            <div class="container">
                <h2>这里是您的代码分析结果缺陷1</h2>
                <hr>
            </div>
        </div>
        <div class="jumbotron2 masthead">
            <div class="container">
                <h2>这里是您的代码分析结果缺陷2</h2>
                <hr>
            </div>
        </div>
        <div class="jumbotron3 masthead">
            <div class="container">
                <h2>这里是您的代码分析结果缺陷3</h2>
                <hr>
            </div>
        </div>
    </div>
</template>

<script>
    export default {
        name: "result"
    }
</script>

<style scoped>
    @media screen and (min-width: 768px){
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
        .masthead {
            padding: 0px 0 0px;
            margin-bottom: 30px;
            margin-top: 0px;
            color: #fff;
        }

        .jumbotron {
            position: relative;
            padding: 25px 25px;
            color: #fff;
            text-align: left;
            text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
            background: #020031;
            background: linear-gradient(45deg,#43d8c9,#035aa6);
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
            box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
        }
        .jumbotron1 {
            position: relative;
            padding: 25px 25px;
            color: #fff;
            text-align: left;
            text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
            background: #020031;
            background: linear-gradient(45deg,#dbe2ef,#3f72af);
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
            box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
        }

        .jumbotron2 {
            position: relative;
            padding: 25px 25px;
            color: #fff;
            text-align: left;
            text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
            background: #020031;
            background: linear-gradient(45deg,#eff3c6,#8ec6c5);
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
            box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
        }

        .jumbotron3 {
            position: relative;
            padding: 25px 25px;
            color: #fff;
            text-align: left;
            text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
            background: #020031;
            background: linear-gradient(45deg,#cae8d5,#3b6978);
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
            box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
        }
    }
</style>
