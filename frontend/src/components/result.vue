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
                <h2>数组越界 line11</h2>
                <pre>
                using namespace std;
                #define mc memcpy
                int main()
                {
                    int a[10000];
                    int b[10000];
                    for(int i=0;i<10000;i++) a[i]=rand();    
                    //
                    a[10001]+=1;    //error：数组越界
                    return 0;
                }
                </pre>
                <hr>
            </div>
        </div>
    </div>
</template>

<script>
    export default {
        name: "Result"
    }
</script>

<style scoped>
        #result{
            box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
            padding: 40px 30px 20px 30px;
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
