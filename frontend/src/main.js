import App from './App.vue'
import Vue from 'vue'
import { BootstrapVue, IconsPlugin } from 'bootstrap-vue'
import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'

import uploader from 'vue-simple-uploader';

import axios from 'axios'
import VueAxios from 'vue-axios'
import router from "./router";

Vue.config.productionTip = false

// Install BootstrapVue
Vue.use(BootstrapVue)
// Optionally install the BootstrapVue icon components plugin
Vue.use(IconsPlugin)

Vue.use(VueAxios, axios)

Vue.use(uploader);

new Vue({
  render: h => h(App),  router
}).$mount('#app')
