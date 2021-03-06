/**
 * Created by win7 on 2017/10/16.
 */
define(["angular","../../../client/js/common.e91a632e.js",'../../../iconfont/iconfont.2e8abf3f.js'], function (angular,common) {
    //读取cookies
    function getCookie(name) {
        var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");
        if(arr=document.cookie.match(reg))
            return unescape(arr[2]);
        else
            return null;
    }
    //设置cookies
    function setCookie(name,value,time) {
        var strsec = getsec(time);
        var exp = new Date();
        exp.setTime(exp.getTime() + strsec*1);
        document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString()+"; path=/";
    }
    function getsec(str) {
        var str1=str.substring(1,str.length)*1;
        var str2=str.substring(0,1);
        if (str2=="s") {
            return str1*1000;
        } else if (str2=="h") {
            return str1*60*60*1000;
        } else if (str2=="d") {
            return str1*24*60*60*1000;
        }
    }
    /*指令模块*/
    var directiveModule=angular.module('directiveModule', []);
    /*防止html转义*/
    directiveModule.filter("trustHtml",function($sce){
        return function (input){
            return $sce.trustAsHtml(input);
        }
    });
    /*金钱转换*/
    directiveModule.filter("moneyChange",function(){
        return function (input){
            /*读取cookie*/
            var output='';
            if(input!=undefined){
                if (getCookie('nowMoneyName') != null) {
                    output = getCookie('nowMoneyName') + (input * parseFloat(getCookie('nowExchangeRate'))).toFixed(2);
                }else{
                    output="$"+input;
                }
            }
            return output;
        }
    });
    /*修改时间*/
    directiveModule.filter("changeDate",function(){
        return function (input){
            var output='';
            if(input!=undefined&&input!=null){
                var date=new Date(input);
                var mont=(date.getMonth()+1)<10?('0'+(date.getMonth()+1)):(date.getMonth()+1);
                var dat=date.getDate()<10?('0'+date.getDate()):date.getDate();
                var hou=date.getHours()<10?('0'+date.getHours()):date.getHours();
                var minut=date.getMinutes()<10?('0'+date.getMinutes()):date.getMinutes();
                var secon=date.getSeconds()<10?('0'+date.getSeconds()):date.getSeconds();
                output=date.getFullYear()+'-'+mont+'-'+dat+" "+hou+":"+minut+":"+secon;
            }
            return output;
        }
    });
    /*修改时间*/
    directiveModule.filter("changeTime", function () {
        return function (input) {
            var output = '';
            if (input != undefined && input != null) {
                var date = new Date(input);
                var mont = (date.getMonth() + 1) < 10 ? ('0' + (date.getMonth() + 1)) : (date.getMonth() + 1);
                var dat = date.getDate() < 10 ? ('0' + date.getDate()) : date.getDate();
                var hou = date.getHours() < 10 ? ('0' + date.getHours()) : date.getHours();
                var minut = date.getMinutes() < 10 ? ('0' + date.getMinutes()) : date.getMinutes();
                output = date.getFullYear() + '-' + mont + '-' + dat + " " + hou + ":" + minut;
            }
            return output;
        }
    });
    /*修改时间1*/
    directiveModule.filter("dateFmt", function () {
        return function (input) {
            var output = '';
            if (input != undefined && input != null) {
                var date = new Date(input);
                var mont = (date.getMonth() + 1) < 10 ? ('0' + (date.getMonth() + 1)) : (date.getMonth() + 1);
                var dat = date.getDate() < 10 ? ('0' + date.getDate()) : date.getDate();
                output = date.getFullYear() + '/' + mont + '/' + dat;
            }
            return output;
        }
    });

    directiveModule.filter("changeTimeOther", function () {
        return function (input) {
            var output = '';
            if (input != undefined && input != null) {
                output = input.substring(0, 16);
            }
            return output;
        }
    });
    /*修改时间*/
    directiveModule.filter("batchNumChangeDate",function(){
        return function (input){
            var output='';
            if(input!=undefined&&input!=null){
                var year=input.substr(1,4);
                var mont=input.substr(5,2);
                var dat=input.substr(7,2);
                output=year+'-'+mont+'-'+dat;
            }
            return output;
        }
    });
    /*时间转换*/
    directiveModule.filter("timeFormat",function(){
        return function (time){
            var date = new Date(time);
            var D = (date.getDate() < 10 ? '0'+date.getDate() : date.getDate());
            var M = (date.getMonth()+1 < 10 ? '0'+(date.getMonth()+1) : date.getMonth()+1);
            var Y = date.getFullYear();
            var h = (date.getHours() < 10 ? '0'+date.getHours() : date.getHours());
            var m = (date.getMinutes() < 10 ? '0'+date.getMinutes() : date.getMinutes());
            var s = (date.getSeconds() < 10 ? '0'+date.getSeconds() : date.getSeconds());
            return (D + "/" + M + "/" + Y + " " + h + ":" + m + ":" + s + " GMT +8");
        }
    });
    /*物流转换*/
    directiveModule.filter("shippingMethodFormat",function(){
        return function (freightMode){
            if (freightMode == "dhl") {
                return "DHL Express";
            } else if (freightMode == "4px") {
                return "Registered Air Mail";
            } else {
                return "";
            }
        }
    });
    /*物流转换*/
    directiveModule.filter("testProductChange", function () {
        return function (freightMode) {
            if (freightMode == "0") {
                return "Not Test";
            } else if (freightMode == "1") {
                return "Random Test";
            } else if (freightMode == "2") {
                return "Fully Test";
            } else {
                return "";
            }
        }
    });
    /*数量四舍五入转换*/
    directiveModule.filter("radom",function(){
        return function (freightMode){
            return Math.round(freightMode);
        }
    });
    /*交期时间转换*/
    directiveModule.filter("achieveDateChange", function () {
        return function (hour,orderType,layer,achieveList) {
            if(orderType==1){
                orderType='examples';
            }else{
                orderType='batch';
            }
            var returnVal='';
            if(achieveList){
                achieveList.forEach(function(item){
                    if(item.urgentTime==hour&&item.orderType==orderType&&item.stencilLayer==layer){
                        returnVal=item.urgentFrontDesc;
                    }
                })
            }
            if(!returnVal)
                returnVal=hour;
            return returnVal;
        }
    });
    /*交期时间转换*/
    directiveModule.filter("achieveDateSteelChange", function () {
        return function (hour) {
            if (hour) {
                var days=(hour/24).toFixed(0);
                return days+"-"+(parseInt(days)+1)+" days";
             }else {
                return "";
             }
        }
    });
    /*层叠结构处理*/
    directiveModule.filter("cascadeStructureChange", function () {
        return function (cascadeStructure, stencilLayer) {
            if (cascadeStructure) {
                if (stencilLayer == 4 && cascadeStructure == 1) {
                    return "JLC7628";
                } else {
                    return "JLC2313";
                }
            } else {
                return "";
            }
        }
    });
    /*余额充值状态转换*/
    directiveModule.filter("balanceStatusChange", function () {
        return function (freightMode) {
            if (freightMode == "1") {
                return "Pending";
            } else if (freightMode == "2") {
                return "Completed";
            } else if (freightMode == "3") {
                return "Cancelled";
            } else {
                return "";
            }
        }
    });
    /*首字母大写转换*/
    directiveModule.filter("toUpperCase", function () {
        return function (txt) {
            if (txt) {
                switch (txt) {
                    case 'card':
                        txt = 'Credit Cart';
                        break;
                    case 'balance':
                        txt = 'JPAY';
                        break;
                    case 'Top-up':
                        txt = 'Offline Payment';
                        break;
                    case 'top-up':
                        txt = 'Offline Payment';
                        break;
                    case 'recharge':
                        txt = 'Top-up';
                        break;
                }
                return txt.substring(0, 1).toUpperCase() + txt.substring(1);
            }
        }
    });
    /*公共弹框*/
    directiveModule.directive('alertModal', function() {

        return {
            restrict : 'EA',
            replace : false,
            scope : {
                modalId:'@',
                modalWidth:'@'
            },
            transclude : true,
            template: '<div class="modal fade zIndex5000" data-backdrop="static" tabindex="-1" role="dialog" id="{{modalId}}">'+
            '<div class="modal-dialog" ng-class="modalWidth" role="document">'+
            '<div class="modal-content"><div ng-transclude></div></div>'+
            '</div>'+
            '</div>',
            link:function(scope){}
        }
    });
    /*头部header*/
    directiveModule.directive('headerSide', function() {
        return {
            restrict : 'EA',
            replace : false,
            scope : {
                /*            cartNumber : '@',
                 hasLogin: '=',
                 username: '@',
                 list: '='*/
            },
            templateUrl : '../client/template/common/head.48840434.html',
            controller:function($scope,myServer,httpApi,$timeout,$rootScope){
                $scope.rootUrl=httpApi.rootUrl;//根域名
                $scope.form={
                    userText:'',//发货弹出框用户搜索国家的内容
                    afterCountryCode: '',//当前选中的
                    afterCountryName: '',//当前选中的
                    currencyListShow: false,//是否显示单位列表
                    countryImg: '',//下拉选中的国家国旗
                    countryName: '',//下拉选中的国家
                    countryCode: ''//下拉选中的国家code
                };
                /*用户信息接口*/
                myServer.getList(httpApi.userInfo)
                    .then(function(data){
                        if(data){    //判断未登录进入短腿页面时头部显示控制
                            $scope.userInfo= $rootScope.userInfo=data;//全局变量
                            $scope.hasLogin=true;
                        }else{
                            $scope.hasLogin=false;
                            $scope.userInfo={};
                            $scope.userInfo.cartNum=0;
                        }
                        /*汇率列表*/
                        return myServer.getList(httpApi.lang)
                    },function(data){
                        //$timeout(function(){common.noticeModal("The network is busy. Please try again later.");},0);
                    })
                    .then(function(data){
                        //console.log(data);
                        $scope.langList=data.list;
                        $scope.langListJia=data.list;
                        /*读取cookie*/
                        if (getCookie('iconCountryExchangeRateFlag')) {
                            $scope.langCountryCode = getCookie('iconCountryExchangeRateFlag');//汇率国
                            //更新汇率
                            angular.forEach($scope.langList,function(dat,index){
                                if (dat.afterCountryCode == $scope.langCountryCode) {
                                    setCookie('nowMoneyName', dat.afterCountryName, 'd1000');//设置汇率金额单位cookie
                                    setCookie('nowExchangeRate',dat.exchangeRate,'d1000');//设置cookie
                                }
                            })
                        }else{
                            $scope.langCountryCode = $scope.langList[0].afterCountryCode;
                            /*设置cookie*/
                            setCookie('nowMoneyName', $scope.langList[0].afterCountryName, 'd1000');//设置汇率金额单位cookie
                            setCookie('iconCountryExchangeRateFlag', $scope.langList[0].afterCountryCode, 'd1000');//设置汇率国cookie
                            setCookie('nowExchangeRate', $scope.langList[0].exchangeRate, 'd1000');//设置汇率cookie
                        }
                        $scope.form.afterCountryCode = $scope.langCountryCode;
                        $scope.form.afterCountryName = getCookie('nowMoneyName') ? getCookie('nowMoneyName') : '$';
                        /*资源开关接口*/
                        return myServer.getList(httpApi.getWebSourceSwitch);
                    },function(data){
                        //$timeout(function(){common.noticeModal("The network is busy. Please try again later.");},0);
                    })
                    .then(function(data){
                        /*资源开关返回*/
                        //console.log(data);
                        $scope.webSourceSwitch=$rootScope.webSourceSwitch=data;
                        /*通知接口*/
                        return myServer.postList(httpApi.getWebNotice,{noticeId:1});
                    },function(){
                        //$timeout(function(){common.noticeModal("The network is busy. Please try again later.");},0);
                    })
                    .then(function(data){
                        /*通知接口返回*/
                        //console.log(data);
                        $scope.noticeMsg=data;
                        /*国家列表接口*/
                        return myServer.getList(httpApi.customerFlag);
                    },function(){
                        //$timeout(function(){common.noticeModal("The network is busy. Please try again later.");},0);
                    }).then(function (data) {
                    /*国家列表接口返回*/
                    //console.log(data)
                    $scope.shippingCountryList = data.list;
                    $scope.shippingCountryListJia = data.list;//用来做搜索功能的
                    /*读取cookie*/
                    if (getCookie('iconCountryFlag') != null) {
                        var CountryCode = getCookie('iconCountryFlag');//国家code
                        //更新国家
                        angular.forEach($scope.shippingCountryList, function (dat, index) {
                            if (dat.countryCode == CountryCode) {
                                $scope.form.countryCode = CountryCode;//国家code
                                $scope.countryName = dat.countryName;//国家
                                $scope.countryFlagIconFont = dat.iconFont;//国家国旗
                            }
                        })
                    } else {
                        $rootScope.noCountry = false;
                    }

                }, function () {
                    $timeout(function () {
                        //common.noticeModal("The network is busy. Please try again later.");
                    }, 0);
                });
                /*保存*/
                $scope.save=function (){
                    /*修改国家和汇率*/
                    myServer.postList(httpApi.changeCode,{
                        iconCountryExchangeRateFlag: $scope.form.afterCountryCode,
                        iconCountryFlag: $scope.form.countryCode
                    }).then(function (data) {
                        //console.log(data);
                        if (data == "true") {
                            /*恢复弹出框的ul字体的颜色*/
                            $("#countryExchangeRateList1 span").css({color: "#555"});
                            $(".mask").fadeOut(300);
                            $(".openDiv").fadeOut(300);
                            //更新汇率
                            angular.forEach($scope.langList, function (dat, index) {
                                if (dat.afterCountryCode == $scope.form.afterCountryCode) {
                                    setCookie('nowMoneyName', dat.afterCountryName, 'd1000');//设置汇率金额单位cookie
                                    setCookie('iconCountryExchangeRateFlag', dat.afterCountryCode, 'd1000');//设置汇率国code cookie
                                    setCookie('nowExchangeRate', dat.exchangeRate, 'd1000');//设置汇率cookie
                                }
                            });
                            setCookie('iconCountryFlag', $scope.form.countryCode, 'd1000');//设置国家code cookie
                            $scope.countryFlagIconFont = $scope.form.countryImg;//国旗
                            $scope.countryName = $scope.form.countryName;//国旗
                            $scope.langCountryCode = $scope.form.afterCountryCode;//国旗
                            window.location.reload();//刷新页面
                        } else {
                            common.alertModal("change fail")
                        }
                    });
                };
                /*国家弹出框*/
                $scope.showCountryModel = function () {
                    // 打开发货弹出框
                    $(".mask").fadeIn(300);
                    $(".openDiv").fadeIn(300);
                    $scope.form.afterCountryCode = $scope.langCountryCode;
                    $scope.form.afterCountryName = getCookie('nowMoneyName') ? getCookie('nowMoneyName') : '$';
                    /*读取cookie*/
                    if (getCookie('iconCountryFlag')) {
                        var CountryCode = getCookie('iconCountryFlag');//国家code
                        //更新国家
                        angular.forEach($scope.shippingCountryList, function (dat, index) {
                            if (dat.countryCode == CountryCode) {
                                $scope.form.countryCode = CountryCode;//国家code
                                $scope.form.countryName = dat.countryName;//国家
                                $scope.form.countryImg = dat.iconFont;//国家国旗
                            }
                        })
                    }
                };
                /*弹出Currency列表*/
                $scope.showCurrencyList = function (event) {
                    event.stopPropagation();
                    $scope.form.currencyListShow = !$scope.form.currencyListShow;
                };
                /*选择当前Currency*/
                $scope.curCountry = function (event, item) {
                    event.stopPropagation();
                    $scope.form.afterCountryCode = item.afterCountryCode;
                    $scope.form.afterCountryName = item.afterCountryName;
                    $scope.form.currencyListShow = false;
                };
                /*选择当前国家*/
                $scope.showCountryList = function (event) {
                    event.stopPropagation();
                    $scope.form.currencyListShow = false;
                    $scope.form.countryListShow = !$scope.form.countryListShow;
                };
                $scope.stopPro = function (event) {
                    event.stopPropagation();
                };
                /*修改语言*/
                $scope.changeLang = function (item) {
                    $scope.form.countryCode = item.countryCode;
                    $scope.form.countryName = item.countryName;
                    $scope.form.countryImg = item.iconFont;
                    $scope.form.countryListShow = false;
                };
                /*隐藏下拉框*/
                $scope.hideList = function () {
                    $scope.form.countryListShow = false;
                    $scope.form.currencyListShow = false;
                }
            },
            link: function (scope) {
                /*取消*/
                scope.cancel=function (){
                    $(".mask").fadeOut(1000);
                    $(".openDiv").fadeOut(1000);
                };
                /*搜索功能*/
                scope.index=-1;
                scope.showSearchList=function (e) {
                    if(scope.form.userText.length == 0) scope.shippingCountryList= scope.shippingCountryListJia;
                    scope.shippingCountryList = scope.shippingCountryList.filter(function (item) {
                        return item.countryName.toLowerCase().includes(scope.form.userText.toLowerCase());
                    });
                    /*键盘上下移动事件*/
                    scope.as=$("#countryExchangeRateList1 li");
                    scope.text=$('#nowFlag1');
                    scope.ul=$(".countryExchangeRateListDiv");
                    /* 获取弹出框的当前的选中的国家保存下来*/
                    scope.curCountrySrc1 = $("#nowFlag1>img");
                    scope.curCodeText1 = $("#nowFlag1 .countryCode");
                    /* 在键盘未上下移动前设置为统一的字体颜色*/
                    angular.forEach(scope.as, function (value, index){
                        scope.as.eq(scope.index).css({color:"#555"});

                    });
                    /*如果按下了向下方向键*/
                    if(e.keyCode==40){
                        scope.index++;
                        if(scope.index>=scope.as.length){
                            scope.index=0;
                        }
                        scope.as[scope.index].style.color="orange";
                    }
                    /*如果按下了向上方向键*/
                    if(e.keyCode==38){
                        if(scope.index<=0){
                            scope.index=scope.as.length;
                        }
                        scope.index--;
                        scope.as[scope.index].style.color="orange";
                    }
                    /*如果按下了回车键*/
                    if(e.keyCode==13 && scope.index!=-1){
                        scope.ulImgSrc=scope.as.eq(scope.index).find('img').attr("src");
                        scope.ulText=scope.as.eq(scope.index).find('span').text();
                        $(".curCountrySrc1").attr('src',scope.ulImgSrc);
                        $(".curCodeText1").text(scope.ulText);
                        angular.forEach(scope.as, function (value, index){
                            scope.as.eq(scope.index).css({color:"#555"});
                        });
                        scope.ul.removeClass('showList');
                        scope.index=-1;
                    }
                    if(e && e.preventDefault) {
                        /*阻止默认浏览器动作(W3C)*/
                        e.preventDefault();
                    } else {
                        /*IE中阻止函数器默认动作的方式*/
                        window.event.returnValue = false;
                    }
                    return false;

                }
            }
        }
    });
    /*底部footer*/
    directiveModule.directive('footerSide', function() {
        return {
            restrict : 'EA',
            replace : false,
            templateUrl : '../client/template/common/footer.e13a79e1.html',
            link:function(scope){
                var lang = (navigator.systemLanguage?navigator.systemLanguage:navigator.language);
                //获取浏览器配置语言 #括号内是个运算，运算过后赋值给lang，当?前的内容为true时把?后的值赋给lang，为False时把:后的值赋给lang
                var lang = lang.substr(0, 2);//获取lang字符串的前两位
                if(lang == 'zh'){
                    scope.chineseLang=true;
                }else{
                    scope.chineseLang=false;
                }
                if (getCookie('cookieNote')) {
                    scope.cookieNote = false;
                } else {
                    scope.cookieNote = true;
                }
                scope.accept = function () {
                    setCookie('cookieNote', "true", 'd1000');//设置cookie
                    scope.cookieNote = false;
                }
            }
        }
    });
    /*退出登录*/
    directiveModule.directive('clearLogin', function() {
        return {
            restrict : 'EA',
            replace : false,
            scope : true,
            template : '<a href="javascript:void(0);" ng-click="clearLogin()">Logout</a>',
            controller:function($scope,httpService,httpApi){
                /*退出登录*/
                $scope.clearLogin=function(){
                    httpService.getApi(httpApi.clearLogin,function(data){
                        if(data.status=="success"){
                            sessionStorage.clear("hasLogin");
                            window.location.href="https://passport.easyeda.com/cas/login";
                        }else{
                            common.alertModal(data.msg);
                        }
                    })
                }
            },
            link:function(scope){}
        }
    });
    /*nav导航*/
    directiveModule.directive('nav', function() {
        return {
            restrict : 'EA',
            scope : {
                one : '@',
                oneHref:'@',
                two : '@',
                twoHref:'@',
                three: '@',
                threeHref:'@',
                four: '@'
            },
            replace : false,
            template : '<ol class="breadcrumb mb10">'+
                '<li><a ng-href="{{oneHref==undefined?\'javascript:void(0);\':oneHref}}" ng-bind="one"></a></li>'+
                '<li><a ng-href="{{twoHref==undefined?\'javascript:void(0);\':twoHref}}" ng-bind="two"></a></li>'+
                '<li ng-if="three!=undefined"><a ng-href="{{threeHref==undefined?\'javascript:void(0);\':threeHref}}" ng-bind="three"></a></li>'+
                '<li ng-if="four!=undefined"><a href="javascript:void(0);" ng-bind="four"></a></li>'+
            '</ol>',
            link:function(){}
        }
    });
    /*灰色背景--标题*/
    directiveModule.directive('bgHead', function() {
        return {
            restrict : 'EA',
            replace : true,
            scope : {},
            transclude : true,
            template : '<h2 class="bged color0 font18 plr20 strong line45 mt5" ng-transclude></h2>'
        }
    });
    /*primary按钮*/
    directiveModule.directive('btnPrimaryW220', function() {
        return {
            restrict : 'EA',
            replace : true,
            priority:2,
            transclude : true,
            template : '<button class="btn btn-primary w220 h40 font16" ng-transclude></button>',
        }
    });
    /*inputbox输入框box*/
    directiveModule.directive('inputBox', function() {
        return {
            restrict : 'EA',
            replace : true,
            priority:2,
            scope : {
                model:'=',
                placeholder: '@',
                bindBlur:"&",
                bindKeyup:"&",
                bindMaxlength: "@",
                borderRed: "@"
            },
            transclude : true,
            template: '<div class="form-group border" ng-class="{\'borderRed\':borderRed==\'true\'}">' +
                            '<label class="normal colorc0 pl12 font12 mb0 mt5 block" ng-transclude for="{{randomVal}}"></label>'+
            '<input placeholder="{{placeholder}}" type="text" maxlength="{{bindMaxlength}}" ng-keyup="bindKeyup({model:model})" ng-blur="bindBlur({model:model})" ng-model="model" value="" class="form-control borderNone color0 font14 strong" id="{{randomVal}}">' +
                        '</div>',
            link:function(scope,elem,attr){
                scope.randomVal=(Math.random()*Math.pow(10,10)).toFixed(0);
            }
        }
    });
    /*selectbox国家选择框box*/
    directiveModule.directive('selectBox', function() {
        return {
            restrict : 'A',
            replace : true,
            priority:2,
            transclude : true,
            template : '<div class="form-group border afterDrop">'+
                            ' <label class="normal colorc0 pl12 font12 mb0 mt5" ng-transclude></label>'+
                            '<input ng-model="form.countryName" type="text" class="form-control borderNone color0 font14 strong bgfff hand" readonly autocomplete="off">'+
                            '<ul class="list-unstyled absList">'+
                            '<li><input ng-keyup="searchCountry()" ng-model="form.countryInput" class="form-control searchInput" autocomplete="off"></li>'+
                            '<li ng-click="takeVal(item)" ng-repeat="item in countryList track by $index">{{item.countryName}}</li>'+
                            '</ul>'+
                    '</div>',
            link:function(scope,elem,attr){
                /*下拉框显示*/
                elem.on("click",function(ev){
                    ev.stopPropagation();
                    elem.find("ul").toggle();
                });
                elem.find(".searchInput").on("click",function(ev){
                    ev.stopPropagation();
                });
                angular.element("body").on("click",function(){
                    elem.find("ul").hide();
                })
            }
        }
    });
    /*select选择框*/
    directiveModule.directive('select', function() {
        return {
            restrict : 'A',
            replace : true,
            priority:2,
            scope : {
                model:'=',
                list:"=",
                place:"@"
            },
            transclude : true,
            template : '  <div class="afterDrop">'+
                '<input ng-model="model" type="text" class="form-control radius0 h40 bgfff hand" readonly title="{{place}}" placeholder="{{place}}">'+
                '<ul class="list-unstyled absList">'+
                '<li ng-click="takeVal(item)" ng-repeat="item in list track by $index">{{item}}</li>'+
                '</ul>'+
            '</div>',
            controller:function($scope){
                $scope.takeVal=function(val){
                    $scope.model=val;
                }
            },
            link:function(scope,elem,attr){
                /*下拉框显示*/
                elem.on("click",function(ev){
                    ev.stopPropagation();
                    elem.find("ul").toggle();
                });
                angular.element("body").on("click",function(){
                    elem.find("ul").hide();
                })
            }
        }
    });
    /*商品列表显示大图*/
    directiveModule.directive('showBigPic', function() {
        return {
            restrict : 'A',
            scope : {},
            replace : false,
            link:function(scope,elem){
                /*获取offset*/
                function offset(elm) {
                    try {return elm.offset();} catch(e) {}
                    var rawDom = elm[0];
                    var _x = 0;
                    var _y = 0;
                    var body = document.documentElement || document.body;
                    var scrollX = window.pageXOffset || body.scrollLeft;
                    var scrollY = window.pageYOffset || body.scrollTop;
                    _x = rawDom.getBoundingClientRect().left + scrollX;
                    _y = rawDom.getBoundingClientRect().top + scrollY;
                    return { left: _x, top: _y };
                }
                elem.on("mouseover",function(){
                    if(elem.find(".img")[0].src.indexOf('noImg')==-1&&elem.find(".img")[0].src.indexOf('cart_steel')==-1){
                        //elem.find(".absSmall").show();
                        elem.parent().find(".absBigPic").show();
                        if(angular.element("body")[0].offsetWidth-offset(elem).left<=elem[0].offsetWidth+elem.parent().find(".absBigPic")[0].offsetWidth){
                            elem.parent().find(".absBigPic")[0].style.marginLeft=-elem.parent().find(".absBigPic")[0].offsetWidth+"px";
                        }else{
                            elem.parent().find(".absBigPic")[0].style.marginLeft=elem[0].offsetWidth+"px";
                        }
                    }
                });
                elem.on("mouseout",function(){
                    //elem.find(".absSmall").hide();
                    elem.parent().find(".absBigPic").hide();
                });
                elem.on("mousemove",function(ev){
                    var oEvent=ev||event;
                    var x=oEvent.pageX-offset(elem.find('.img')).left-elem.find(".absSmall")[0].offsetWidth/2;
                    var y=oEvent.pageY-offset(elem.find('.img')).top-elem.find(".absSmall")[0].offsetHeight/2;
                    if(x<0) {
                        x=0;
                    } else if(x>elem.parent().find(".absBigPic")[0].offsetWidth-elem.find(".absSmall")[0].offsetWidth) {
                        x=elem.parent().find(".absBigPic")[0].offsetWidth-elem.find(".absSmall")[0].offsetWidth;
                    }
                    if(y<0) {
                        y=0;
                    } else if(y>elem.parent().find(".absBigPic")[0].offsetHeight-elem.find(".absSmall")[0].offsetHeight) {
                        y=elem.parent().find(".absBigPic")[0].offsetHeight-elem.find(".absSmall")[0].offsetHeight;
                    }
                    elem.find(".absSmall")[0].style.left=x+'px';
                    elem.find(".absSmall")[0].style.top=y+'px';
                    var percentX=x/(elem.find('.img')[0].offsetWidth-elem.find(".absSmall")[0].offsetWidth);
                    var percentY=y/(elem.find('.img')[0].offsetHeight-elem.find(".absSmall")[0].offsetHeight);
                    elem.parent().find(".absBigPic img")[0].style.left=-percentX*(elem.parent().find(".absBigPic img")[0].offsetWidth-elem.parent().find(".absBigPic")[0].offsetWidth)+'px';
                    elem.parent().find(".absBigPic img")[0].style.top=-percentY*(elem.parent().find(".absBigPic img")[0].offsetHeight-elem.parent().find(".absBigPic")[0].offsetHeight)+'px';
                });
            }
        }
    });
    return directiveModule;
});
