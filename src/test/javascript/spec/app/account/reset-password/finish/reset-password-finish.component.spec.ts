import axios from 'axios';
import * as config from '@/shared/config/config';
import { createLocalVue, shallowMount, Wrapper } from '@vue/test-utils';
import ResetPasswordFinish from '@/account/reset-password/finish/reset-password-finish.vue';
import ResetPasswordFinishClass from '@/account/reset-password/finish/reset-password-finish.component';
import LoginService from '@/account/login.service';

const localVue = createLocalVue();
const mockedAxios: any = axios;

config.initVueApp(localVue);
const i18n = config.initI18N(localVue);

jest.mock('axios', () => ({
  get: jest.fn(),
  post: jest.fn()
}));

describe('Reset Component Finish', () => {
  let wrapper: Wrapper<ResetPasswordFinishClass>;
  let resetPasswordFinish: ResetPasswordFinishClass;

  beforeEach(() => {
    mockedAxios.post.mockReset();
    wrapper = shallowMount<ResetPasswordFinishClass>(ResetPasswordFinish, {
      i18n,
      localVue,
      provide: {
        loginService: () => new LoginService()
      }
    });
    resetPasswordFinish = wrapper.vm;
  });

  it('should be a Vue instance', () => {
    expect(wrapper.isVueInstance()).toBeTruthy();
  });

  it('should reset finish be a success', async () => {
    // Given
    mockedAxios.post.mockReturnValue(Promise.resolve());

    // When
    await resetPasswordFinish.finishReset();

    // Then
    expect(resetPasswordFinish.success).toBeTruthy();
  });

  it('should reset request fail as an error', async () => {
    // Given
    mockedAxios.post.mockReturnValue(
      Promise.reject({
        response: {
          status: null,
          data: {
            type: null
          }
        }
      })
    );

    // When
    resetPasswordFinish.finishReset();
    await resetPasswordFinish.$nextTick();

    // Then
    expect(resetPasswordFinish.success).toBeNull();
    expect(resetPasswordFinish.error).toEqual('ERROR');
  });
});
