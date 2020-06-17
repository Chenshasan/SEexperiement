<template>
  <div id="app">
      <div id="app">
          <div class="container-fluid">
              <div>
                  <router-view></router-view>
              </div>
          </div>
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
        text-align: center;
        margin-left: 40px;
    }

</style>
