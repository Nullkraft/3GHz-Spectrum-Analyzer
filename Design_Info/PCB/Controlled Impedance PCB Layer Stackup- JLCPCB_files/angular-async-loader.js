!function(){function e(e){function r(e){return"string"==typeof e&&(e=[e]),["$q",function(r){var n=r.defer();return t(e,function(){n.resolve(arguments)}),n.promise}]}function n(e){function n(e,r){e.controllerUrl&&r.push(e.controllerUrl),e.dependencies&&("string"==typeof e.dependencies?r.push(e.dependencies):[].push.apply(r,e.dependencies))}var o=[];if(n(e,o),e.hasOwnProperty("views")&&Object.keys(e.views).forEach(function(r){n(e.views[r],o)}),o.length>0){var t=e.resolve||{};t.$dummy=r(o),e.resolve=t}return e}var o="1.3.2",t=function(){if("function"==typeof require)return"function"==typeof require.async?require.async:require;if("object"==typeof seajs&&"function"==typeof seajs.use)return seajs.use;if("object"==typeof System&&"function"==typeof System.amdRequire)return System.amdRequire;throw new Error("No amd/cmd module loader found.")}();return{VERSION:o,configure:function(r){r.provider("ngProviders",["$controllerProvider","$compileProvider","$filterProvider","$provide","$injector",function(e,r,n,o,t){this.$get=function(){return{$controllerProvider:e,$compileProvider:r,$filterProvider:n,$provide:o,$injector:t}}}]),r.run(["ngProviders","$injector",function(n,o){var t=n.$controllerProvider,i=n.$compileProvider,u=n.$filterProvider,c=n.$provide;r.useModule=function(t){var i=e.module(t);if(i.requires)for(var u=0;u<i.requires.length;u++)r.useModule(i.requires[u]);return e.forEach(i._invokeQueue,function(e){var r=n[e[0]]||o.get(e[0]);r[e[1]].apply(r,e[2])}),e.forEach(i._configBlocks,function(e){var r=n.$injector.get(e[0]);r[e[1]].apply(r,e[2])}),e.forEach(i._runBlocks,function(e){o.invoke(e)}),r},r.value=function(e,n){return c.value(e,n),r},r.constant=function(e,n){return c.constant(e,n),r},r.factory=function(e,n){return c.factory(e,n),r},r.service=function(e,n){return c.service(e,n),r},r.filter=function(e,n){return u.register(e,n),r},r.directive=function(e,n){return i.directive(e,n),r},r.controller=function(e,n){return t.register(e,n),r},r.decorator=function(e,n){return c.decorator(e,n),r},r.provider=function(e,n){return c.provider(e,n),r},r.get=function(e){return o.get(e)}}]),r.requires&&-1!==r.requires.indexOf("ngRoute")&&r.config(["$routeProvider",function(e){var r=e.when;e.when=function(o,t){return r.call(e,o,n(t))}}]),r.requires&&-1!==r.requires.indexOf("ui.router")&&r.config(["$stateProvider",function(e){var r=e.state;e.state=function(o,t){return r.call(e,o,n(t))}}])}}}"function"==typeof define&&define.amd?define(["angular"],function(r){return e(r)}):window.asyncLoader=e(window.angular)}();
//# sourceMappingURL=angular-async-loader.min.js.map