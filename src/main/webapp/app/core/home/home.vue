<template>
    <div >
        <div class="jumbotron masthead">
            <div class="container">
                <h1>XAnalysis</h1>
                <h2>简洁、直观的静态分析工具，让代码更安全、简单。</h2>
                <h5>当前版本： v1.0.0 | 文档更新于：2020-04-25</h5>
                <hr>
<<<<<<< HEAD
                <a class="btn btn-outline-inverse btn-lg" v-on:click="jump()" target="_blank">点击开始进入静态分析之旅</a>
=======
                <router-link class="btn btn-lg" to="/upload">点击开始进入静态分析之旅</router-link>
>>>>>>> 7114a920dfef268c1a3443975be16e1d59e8ccc9
            </div>
        </div>
        <div class="col-md-9">
            <h5 v-if="username" v-text="$t('home.logged.message', { 'username': username})">欢迎，"{{username}}"</h5>

            <div>
                <div class="alert alert-warning" v-if="!authenticated">
                    <span>如果希望使用我们的静态分析工具，您可以</span>
                    <a class="alert-link" v-on:click="openLogin()" v-text="$t('global.messages.info.authenticated.link')">sign in</a><span v-html="$t('global.messages.info.authenticated.suffix')">, you can try the default accounts:<br/>- Administrator (login="admin" and password="admin") <br/>- User (login="user" and password="user").</span>
                </div>
                <div class="alert alert-warning" v-if="!authenticated">
                    <span v-text="$t('global.messages.info.register.noaccount')">You don't have an account yet?</span>&nbsp;
                    <router-link class="alert-link" to="/register" v-text="$t('global.messages.info.register.link')">Register a new account</router-link>
                </div>
            </div>
        </div>
    </div>
</template>

<script lang="ts" src="./home.component.ts">

</script>

<style>
    @media screen and (min-width: 768px){
        .masthead h1 {
            font-size: 110px;
            font-weight:bold;
            margin-bottom: 30px;
            color:#ffffff;
        }
        .btn{
            color: #ffffff;
        }
        .masthead h2 {
            color:#ffffff;
            font-weight:normal;
            margin-bottom:30px;
        }
    .masthead {
        padding: 0px 0 0px;
        margin-bottom: 50px;
        margin-top: 0px;
        color: #fff;
    }

    .jumbotron {
        position: relative;
        padding: 200px 200px;
        color: #fff;
        text-align: center;
        text-shadow: 0 1px 3px rgba(0,0,0,.4), 0 0 30px rgba(0,0,0,.075);
        background: #020031;
        background: linear-gradient(45deg,#43d8c9,#035aa6);
        filter: progid:DXImageTransform.Microsoft.gradient(startColorstr="#020031",endColorstr="#6d3353",GradientType=1);
        box-shadow: inset 0 3px 7px rgba(255,255,255,.75), inset 0 -3px 7px rgba(255,255,255,.2);
    }
    }
</style>
