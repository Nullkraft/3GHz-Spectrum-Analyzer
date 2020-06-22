/**
 * app.js
 */
define(
    [
        'angular',
        'angular-async-loader',
        '../../client/js/services/servicesModule.700379ff.js',
        'angularRoute',
        'upload',
        '../../client/js/directive/directiveModule.5dc53cec.js',
        '../../client/js/directive/orderDirective.5c333429.js?ver=2',
        '../../client/js/common.e91a632e.js'
    ], function (angular,asyncLoader) {
        if(!window.console){
            window.console = {};
        }
        if(!window.console.log){
            window.console.log = function(msg){};
        }
        var app=angular.module('myApp', [ 'ngRoute',  'servicesModule','ngFileUpload','directiveModule']);
        asyncLoader.configure(app);
        return app;
    }
);