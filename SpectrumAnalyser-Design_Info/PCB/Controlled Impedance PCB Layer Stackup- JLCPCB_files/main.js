/**
 * Created by win7 on 2017/10/12.
 */
/**
 * main.js这个文件完成的事情简单来说就是：载入所有文件，然后在document上运行Angular并将ng-app属性设置为’app’。
 * 这些文件因为是由RequireJS异步载入，因此我们需要来“手动启动”Angular应用。
 *
 * */

require.config({
    baseUrl: 'js/',//路径
    urlArgs: "ver=1.671",//版本号,防止读取缓存，调试用
    //urlArgs: "ver=" +  (new Date()).getTime(),//版本号,防止读取缓存，调试用
    paths: {
        //库文件
        "angular": "lib/angular.min.js?ver=1",
        "bootstrap": "lib/bootstrap/js/bootstrap.min.js?ver=1",
        'angularRoute': './lib/angular-route.min.js?ver=1',
        'angular-async-loader': 'lib/angular-async-loader.min.js?ver=1',
        "jquery": "lib/jquery-1.9.1.min.js?ver=1",
        "echarts": "lib/echarts/echarts.min.js?ver=1",//圆形图
        /*        "knob": "lib/jsKnob/jquery.knob",*///圆形图
        "upload": "lib/ng-file-upload/ng-file-upload.min.js?ver=1",//上传文件
        "shim": "lib/ng-file-upload/ng-file-upload-shim.min.js?ver=1",
        'datetimepicker': 'lib/bootstrap-datetimepicker/bootstrap-datetimepicker.min.js?ver=1',//时间选择器
        'datetimepickerLang': 'lib/bootstrap-datetimepicker/locales/bootstrap-datetimepicker.zh-CN.js?ver=1',//时间选择器语言包
        'highcharts': 'lib/highcharts.js?ver=1',//图标插件
        'mock': 'lib/mock/mock-min.js?ver=1',//mock模拟数据接口
        "slide": "lib/jquery.SuperSlide.2.1.1.js?ver=1",//slide插件
        "dot": "lib/dotdotdot/jquery.dotdotdot.min.js?ver=1",//dotdotdot插件
        "html2canvas": "lib/html2canvas/html2canvas.min.js?ver=1",//html2canvas插件
        "jsPDF": "lib/html2canvas/jspdf.min.js?ver=1",//jsPDF插件
        "md5": "lib/md5.js?ver=1"//md5
    },
    waitSeconds: 0,
    /*css引入配置*/
    map: {
        '*': {
            'css': 'lib/css.min'
        }
    },
    shim: {
        angular: {
            exports: "angular",
            deps: ['jquery']
        },
        angularRoute: {
            deps: ["angular"]
        },
        bootstrap: {
            deps: ['jquery']
        },
        upload: {
            deps: ['angular','shim']
        },
        'angular-messages': {
            deps: ['angular']
        },
        'datetimepicker': {
            deps: ['css!lib/bootstrap-datetimepicker/bootstrap-datetimepicker.min.css','bootstrap']
        },
        'datetimepickerLang': {
            deps: ['datetimepicker']
        },
        'province': {
            exports: "pro",
            deps: ['jquery']
        },
        'city': {
            exports: "cit",
            deps: ['jquery']
        },
        'zygAddress': {
            deps: ['css!lib/citySelect/zygAddress.css','jquery']
        },
        'highcharts': {
            exports: "highcharts",
            deps: ['jquery']
        },
 /*       'knob': {
            exports: "knob",
                deps: ['jquery']
        },*/
        'echarts': {
            exports: "echarts",
        },
        'dot': {
            exports: "dot",
            deps: ['jquery']
        },
        'html2canvas': {
            exports: "html2canvas",
        },
        'jsPDF': {
            exports: "jsPDF",
            deps: ['html2canvas']
        },
        'md5': {
            exports: "md5",
        }
    }
});
//手动启动ng
require(["../client/js/routes.c63f64cf.js?ver=4"], function () {
    //使用bootstrap方法启动Angular应用
    angular.element(document).ready(function() {

        angular.bootstrap(document, ['myApp']);
        angular.element(document).find('html').attr('ng-app','myApp');
    });
});
