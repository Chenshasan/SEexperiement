import Vue from 'vue'
import VueRouter from 'vue-router'
import HelloWorld from "../components/HelloWorld";
import Result from "../components/result";
import Upload from "../components/upload";
Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name : 'HelloWorld',
    component: HelloWorld
  },
  {
    path: '/result',
    name : 'Result',
    component: Result
  },
  {
    path:'/upload',
    name : 'Upload',
    component: Upload
  },
]

const router=new VueRouter({routes})

export default router
