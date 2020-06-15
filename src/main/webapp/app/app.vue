<template>
  <div id="app">
    <div id="app-header">
    </div>
    <div class="container-fluid">
      <div class="card jh-card">
        <router-view></router-view>
      </div>
      <b-modal id="login-page" hide-footer lazy>
        <span slot="modal-title" id="login-title" v-text="$t('login.title')">Sign in</span>
        <login-form></login-form>
      </b-modal>
    </div>
  </div>
</template>

<script lang="ts" src="./app.component.ts">
</script>

<style>
    .container-fluid{
        margin-top: 50px;
        margin-bottom: 50px;
    }

    #app{
        width: 1350px;
        margin-left: 70px;
        box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
    }

</style>
