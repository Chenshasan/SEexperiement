import request from '../util/request';

export function merge(form) {
  return request({
    url: '/upload/merge',
    method: 'post',
    data: form
  });
}

export function getResult(form) {
  return request({
    url: '/upload/merge',
    method: 'post',
    data: form
  });
}
