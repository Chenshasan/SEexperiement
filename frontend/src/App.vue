<template>
    <div id="app">
        <div class="container-fluid">
            <div>
                <router-view></router-view>
            </div>
        </div>
    </div>
</template>

<script>
export default {
  name: 'App',
  components: {
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
}

.container-fluid{
    margin-top: 50px;
    margin-bottom: 50px;
}

#app{
    width: 1350px;
    margin-left: 70px;
    padding-left: 30px;
    padding-right: 30px;
    box-shadow: 0 3px 7px rgba(0,0,0,.75), 0 -3px 7px rgba(0,0,0,.2);
}
</style>
