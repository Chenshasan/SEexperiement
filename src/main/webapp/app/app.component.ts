import Vue from 'vue';
import Component from 'vue-class-component';
import Ribbon from './core/ribbon/ribbon.vue';
import LoginForm from './account/login-form/login-form.vue';

@Component({
  components: {
    ribbon: Ribbon,
    'login-form': LoginForm
  }
})
export default class App extends Vue {}
