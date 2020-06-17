import Component from 'vue-class-component';
import { Inject, Vue } from 'vue-property-decorator';
import LoginService from '@/account/login.service';

@Component
export default class Home extends Vue {
  data() {
    return {
      uploadOptions1: {
        target: '//localhost:8082/upload/single', //上传的接口
        testChunks: false, //是否开启服务器分片校验
        fileParameterName: 'file', //默认的文件参数名
        headers: {},
        query() {},
        categaryMap: {
          //用于限制上传的类型
          image: ['gif', 'jpg', 'jpeg', 'png', 'bmp'],
          document: ['cpp']
        }
      }
    };
  }

  @Inject('loginService')
  private loginService: () => LoginService;

  public openLogin(): void {
    this.loginService().openLogin((<any>this).$root);
  }

  public jump() {
    this.$router.push({ name: 'UploadComponent' });
  }

  public get authenticated(): boolean {
    return this.$store.getters.authenticated;
  }

  public get username(): string {
    return this.$store.getters.account ? this.$store.getters.account.login : '';
  }
}
